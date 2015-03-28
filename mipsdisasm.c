#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <capstone/capstone.h>

#include "utils.h"

#include "known_addresses.h"

// typedefs

typedef struct _local_table
{
   unsigned int *offsets;
   int count;
} local_table;

#define MAX_PROCEDURES 4096
typedef struct _procedure
{
   unsigned int start;
   unsigned int end;
   local_table locals;
} procedure;

typedef struct _proc_table
{
   procedure procedures[MAX_PROCEDURES];
   int count;
} proc_table;

// static functions
static unsigned int ram_table[] =
{
   // start     end         ram-to-rom
   0x80064040, 0x800646CF, 0x7FB78040, // 4EC000-4EC68F    690 
   0x8007EF50, 0x8008460F, 0x7FE65150, // 219E00-21F4BF   56C0
   0x80169F70, 0x8016EFEF, 0x7FEFFBD0, // 26A3A0-26F41F   5080
   0x8016F000, 0x801B99DF, 0x7FF4FB40, // 21F4C0-269E9F  4A9E0
   0x801B52E0, 0x801C0FEF, 0x800AC8A0, // 108A40-11474F   BD10
   0x801EE0A0, 0x801F153F, 0x7FA3D720, // 7B0980-7B3E1F   34A0
   0x80246000, 0x80345FFF, 0x80245000, // 001000-100FFF 100000
   0x80378800, 0x8038BC8F, 0x80283280, // 0F5580-108A0F  13490
};

static unsigned int ram_to_rom(unsigned int ram_addr)
{
   unsigned i;
   for (i = 0; i < DIM(ram_table)/3; i++) {
      if (ram_addr >= ram_table[i*3] && ram_addr <= ram_table[i*3+1]) {
         return ram_addr - ram_table[i*3+2];
      }
   }
   return ram_addr;
}

static int known_index(unsigned int ram_addr)
{
   unsigned i;
   for (i = 0; i < DIM(known_labels); i++) {
      if (ram_addr == known_labels[i].ram_addr) {
         return i;
      }
   }
   return -1;
}

static int find_local(local_table *locals, unsigned int offset)
{
   int i;
   for (i = 0; i < locals->count; i++) {
      if (locals->offsets[i] == offset) {
         return i;
      }
   }
   return -1;
}

static void add_local(local_table *locals, unsigned int offset)
{
   if (find_local(locals, offset) >= 0) {
      return;
   }
   locals->offsets[locals->count] = offset;
   locals->count++;
}

static int cmp_local(const void *a, const void *b)
{
   const unsigned int *uia = a;
   const unsigned int *uib = b;
   return (*uia - *uib);
}

static int find_proc(proc_table *procs, unsigned int addr)
{
   int i;
   for (i = 0; i < procs->count; i++) {
      if (procs->procedures[i].start == addr) {
         return i;
      } else if (addr > procs->procedures[i].start && addr < procs->procedures[i].end) {
         ERROR("%X in middle of proc %X - %X\n", addr, procs->procedures[i].start, procs->procedures[i].end);
      }
   }
   return -1;
}

static void add_proc(proc_table *procs, unsigned int addr)
{
   if (find_proc(procs, addr) >= 0) {
      return;
   }
   procs->procedures[procs->count].start = addr;
   procs->count++;
}

// collect JALs for a given procedure
static void collect_proc_jals(unsigned char *data, long datalen, proc_table *ptbl, int p)
{
#define MAX_LOCALS 128
#define MAX_BYTES_PER_CALL 1024
   unsigned int local_offsets[MAX_LOCALS];
   local_table locals;
   csh handle;
   cs_insn *insn;
   unsigned int ram_address;
   unsigned int rom_offset;
   unsigned int alloc_size;
   unsigned int last_label;
   unsigned int processed;
   int remaining;
   int cur_amount;
   int count;
   int disassembling;
   procedure *proc;

   // open capstone disassembler
   if (cs_open(CS_ARCH_MIPS, CS_MODE_MIPS64 + CS_MODE_BIG_ENDIAN, &handle) != CS_ERR_OK) {
      ERROR("Error initializing disassembler\n");
      exit(EXIT_FAILURE);
   }
   cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
   cs_option(handle, CS_OPT_SKIPDATA, CS_OPT_ON);

   proc = &ptbl->procedures[p];
   ram_address = proc->start;
   rom_offset = ram_to_rom(ram_address);

   // find referenced JAL, local labels, and find end marked by last JR or ERET
   locals.offsets = local_offsets;
   locals.count = 0;
   disassembling = 1;
   processed = 0;
   remaining = 0;
   last_label = 0;
   while (disassembling) {
      cur_amount = MIN(MAX_BYTES_PER_CALL, datalen - rom_offset - processed);
      count = cs_disasm(handle, &data[rom_offset + processed], cur_amount, ram_address + processed, 0, &insn);
      if (count > 0) {
         int i, o;
         for (i = 0; i < count && disassembling; i++) {
            if (cs_insn_group(handle, &insn[i], MIPS_GRP_JUMP)) {
               // all branches and jumps
               cs_mips *mips = &insn[i].detail->mips;
               for (o = 0; o < mips->op_count; o++) {
                  if (mips->operands[o].type == MIPS_OP_IMM)
                  {
                     unsigned int sec_offset = (unsigned int)mips->operands[o].imm - ram_address;
                     add_local(&locals, sec_offset);
                     if (sec_offset > last_label) {
                        last_label = sec_offset;
                     }
                     if (locals.count > MAX_LOCALS) {
                        ERROR("Need more than %d locals\n", MAX_LOCALS);
                     }
                  }
               }
            } else if (insn[i].id == MIPS_INS_JAL) {
               unsigned int addr;
               cs_mips *mips = &insn[i].detail->mips;
               addr = (unsigned int)mips->operands[0].imm;
               add_proc(ptbl, addr);
            }

            // if we encounter B, JR, or ERET past last local lable, end disassembly
            if (processed >= last_label) {
               switch (insn[i].id) {
                  case MIPS_INS_B:    remaining = 2; break;
                  case MIPS_INS_JR:   remaining = 2; break;
                  case MIPS_INS_ERET: remaining = 1; break;
                  default: break;
               }
            }

            if (remaining > 0) {
               remaining--;
               if (remaining == 0) {
                  disassembling = 0;
               }
            }
            processed += 4;
         }
         cs_free(insn, count);
      } else {
         fprintf(stderr, "ERROR: Failed to disassemble given code!\n");
         disassembling = 0;
      }
   }

   // sort labels
   qsort(locals.offsets, locals.count, sizeof(locals.offsets[0]), cmp_local);
   // copy lables to procedure
   alloc_size = locals.count * sizeof(*locals.offsets);
   proc->locals.offsets = malloc(alloc_size);
   memcpy(proc->locals.offsets, locals.offsets, alloc_size);
   proc->locals.count = locals.count;
   proc->end = ram_address + processed;

   cs_close(&handle);
}

// call this function with at least one procedure in procs
static void collect_jals(unsigned char *data, long datalen, proc_table *procs)
{
   int proc_idx;
   for (proc_idx = 0; proc_idx < procs->count; proc_idx++) {
      collect_proc_jals(data, datalen, procs, proc_idx);
   }
}

static int proc_cmp(const void *a, const void *b)
{
   const procedure *proca = a;
   const procedure *procb = b;
   return (proca->start - procb->start);
}

static unsigned int disassemble_proc(FILE *out, unsigned char *data, long datalen, procedure *proc)
{
   char sec_name[64];
   csh handle;
   cs_insn *insn;
   unsigned int ram_address;
   unsigned int rom_offset;
   unsigned int length;
   unsigned int processed;
   unsigned int cur_amount;
   int count;
   int disassembling;
   int known_idx;

   // open capstone disassembler
   if (cs_open(CS_ARCH_MIPS, CS_MODE_MIPS64 + CS_MODE_BIG_ENDIAN, &handle) != CS_ERR_OK) {
      ERROR("Error initializing disassembler\n");
      exit(EXIT_FAILURE);
   }
   cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
   cs_option(handle, CS_OPT_SKIPDATA, CS_OPT_ON);

   ram_address = proc->start;
   rom_offset = ram_to_rom(ram_address);
   length = proc->end - proc->start;

   // construct procedure label
   known_idx = known_index(ram_address);
   if (known_idx < 0) {
      sprintf(sec_name, "proc_%08X", ram_address);
   } else {
      sprintf(sec_name, "%s", known_labels[known_idx].label);
   }

   // perform disassembly
   disassembling = 1;
   processed = 0;
   fprintf(out, "\n# begin %08X (%06X)\n%s:\n", ram_address, rom_offset, sec_name);
   while (disassembling) {
      cur_amount = MIN(MAX_BYTES_PER_CALL, length - processed);
      cur_amount = MIN(cur_amount, datalen - rom_offset - processed);
      count = cs_disasm(handle, &data[rom_offset + processed], cur_amount, ram_address + processed, 0, &insn);
      if (count > 0) {
         const char *spaces[] = {"      ", "     ", "    ", "   ", "  ", " "};
         int o;
         int i;

         for (i = 0; i < count && disassembling; i++) {
            // handle redirect jump instruction immediates
            // TODO: memory operations
            // TODO: pseudo-instruction detection: li, la, bgt, blt
            int inslen;
            int ll = find_local(&proc->locals, processed);
            if (ll >= 0) {
               fprintf(out, "L%s_%X: # %X\n", sec_name, processed, ram_address + processed);
            }
            if (insn[i].id == MIPS_INS_INVALID) {
               unsigned char *in = &data[rom_offset+processed];
               fprintf(out, ".byte 0x%02X, 0x%02X, 0x%02X, 0x%02X # Invalid: %X",
                     in[0], in[1], in[2], in[3], ram_address + processed);
            } else {
               inslen = MIN(strlen(insn[i].mnemonic), DIM(spaces) - 1);
               fprintf(out, "  %s%s", insn[i].mnemonic, spaces[inslen]);
               if (cs_insn_group(handle, &insn[i], MIPS_GRP_JUMP)) {
                  cs_mips *mips = &insn[i].detail->mips;
                  for (o = 0; o < mips->op_count; o++) {
                     if (o > 0) {
                        fprintf(out, ", ");
                     }
                     switch (mips->operands[o].type) {
                        case MIPS_OP_REG:
                           fprintf(out, "$%s", cs_reg_name(handle, mips->operands[o].reg));
                           break;
                        case MIPS_OP_IMM:
                           {
                              unsigned int sec_offset = (unsigned int)mips->operands[o].imm - ram_address;
                              fprintf(out, "L%s_%X", sec_name, sec_offset);
                              break;
                           }
                        default:
                           break;
                     }
                  }
               } else if (insn[i].id == MIPS_INS_JAL) {
                  unsigned int addr;
                  cs_mips *mips = &insn[i].detail->mips;
                  addr = (unsigned int)mips->operands[0].imm;
                  known_idx = known_index(addr);
                  if (known_idx < 0) {
                     fprintf(out, "proc_%08X", addr);
                  } else {
                     fprintf(out, "%s", known_labels[known_idx].label);
                  }
               } else if (insn[i].id == MIPS_INS_MTC0 || insn[i].id == MIPS_INS_MFC0) {
                  // workaround bug in capstone/LLVM
                  unsigned char *in = &data[rom_offset+processed];
                  unsigned char rd;
                  cs_mips *mips = &insn[i].detail->mips;
                  // 31-24 23-16 15-8 7-0
                  // 0     1     2    3
                  //       31-26  25-21 20-16 15-11 10-0
                  // mfc0: 010000 00000   rt    rd  00000000000
                  // mtc0: 010000 00100   rt    rd  00000000000
                  // rt = in[1] & 0x1F;
                  rd = (in[2] & 0xF8) >> 3;
                  fprintf(out, "$%s, $%d # ", cs_reg_name(handle, mips->operands[0].reg), rd);
                  fprintf(out, "%02X%02X%02X%02X", in[0], in[1], in[2], in[3]);
                  fprintf(out, " %s", insn[i].op_str);
               } else {
                  fprintf(out, "%s", insn[i].op_str);
               }
            }
            fprintf(out, "\n");
            processed += 4;
         }
         cs_free(insn, count);
      } else {
         fprintf(stderr, "ERROR: Failed to disassemble given code!\n");
         disassembling = 0;
      }

      if (processed >= length) {
         disassembling = 0;
      }
   }

   fprintf(out, "# end %s %08X (%06X)\n", sec_name, ram_address + processed, rom_offset + processed);

   cs_close(&handle);

   return ram_address + processed;
}


// mipsdisasm binary [offset] [length]
// offset default: 0
// length default: disassemble through instruction after next jr
int main(int argc, char *argv[])
{
   long file_len;
   unsigned char *data;
   unsigned int ram_address = 0;
   unsigned int rom_offset = 0;
   unsigned int last_end;
   unsigned int offset;
   proc_table procs;
   int proc_idx;
   int i;

   if (argc < 2) {
      ERROR("usage: mipsdisasm BINARY [OFFSET ...]\n");
      return EXIT_FAILURE;
   }

   file_len = read_file(argv[1], &data);

   if (file_len <= 0) {
      return EXIT_FAILURE;
   }

   memset(&procs, 0, sizeof(procs));
   procs.count = 0;

   if (argc > 2) {
      for (i = 2; i < argc; i++) {
         offset = strtoul(argv[i], NULL, 0);
         if (offset >= 0x80000000) {
            ram_address = offset;
            if (offset >= 0x80246000 && offset <= 0x80340fff) {
               rom_offset = offset - 0x80245000;
            } else if (offset >= 0x80378800 && offset <= 0x8038bc90) {
               rom_offset = offset - 0x80283280;
            } else {
               ERROR("Warning: offset %08X not in RAM range\n", offset);
               return EXIT_FAILURE;
            }
         } else {
            rom_offset = offset;
            if (offset >= 0x001000 && offset <= 0x100FFF) {
               ram_address = offset + 0x80245000;
            } else if (offset >= 0x0F5580 && offset <= 0x108A10) {
               ram_address = offset + 0x80283280;
            } else {
               ERROR("Warning: offset %08X not in ROM range\n", offset);
               return EXIT_FAILURE;
            }
         }
         add_proc(&procs, ram_address);
      }
   } else {
      // populate procedure list from list of known addresses
      unsigned i;
      for (i = 0; i < DIM(known_labels); i++) {
         offset = known_labels[i].ram_addr;
         ram_address = offset;
         rom_offset = 0;
         if (offset >= 0x80246000 && offset <= 0x80340fff) {
            rom_offset = offset - 0x80245000;
         } else if (offset >= 0x80378800 && offset <= 0x8038bc90) {
            rom_offset = offset - 0x80283280;
         }
         if (rom_offset != 0 && rom_offset < file_len) {
            add_proc(&procs, offset);
         }
      }
   }

   // collect all JALs
   collect_jals(data, file_len, &procs);

   // sort procedures
   qsort(procs.procedures, procs.count, sizeof(procs.procedures[0]), proc_cmp);

   fprintf(stdout, ".set noat      # allow manual use of $at\n");
   fprintf(stdout, ".set noreorder # don't insert nops after branches\n\n");

   // disassemble all the procedures
   proc_idx = 0;
   last_end = 0xFFFFFFFF;
   while (proc_idx < procs.count) {
      ram_address = procs.procedures[proc_idx].start;
      rom_offset = ram_to_rom(ram_address);
      assert(rom_offset < file_len);
      if (ram_address > last_end) {
         fprintf(stdout, "\n# missing section %X-%X (%06X-%06X) [%X]\n",
               last_end, ram_address, ram_to_rom(last_end), ram_to_rom(ram_address), ram_address - last_end);
      }
      disassemble_proc(stdout, data, file_len, &procs.procedures[proc_idx]);
      last_end = procs.procedures[proc_idx].end;
      proc_idx++;
   }

   free(data);

   return EXIT_SUCCESS;
}

