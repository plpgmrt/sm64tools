// 8019C450 -> 24C910
// ROM = RAM - 7FF4FB40

typedef struct
{
   unsigned int ram_addr;
   char *label;
} label;

label known_labels[] = {
   {0x80173c6c, "PutString1"},
   {0x80173d64, "PutString2"},
   {0x80173fd4, "SaveMenu"},
   {0x80174324, "ClearFile"},
   {0x801743ac, "SaveMenu2"},
   {0x801746f8, "PutMessage"},
   {0x801749b0, "SaveMenu3"},
   {0x80175238, "FileConfirmations"},
   {0x801755a8, "SaveMenu4"},
   {0x801758a0, "SoundSelect"},
   {0x801764e0, "MenuSelect"},
   {0x80176fc4, "ShowSomeNum"},
   {0x80177710, "__main__"},
   {0x801779dc, "MakeMemBlock"},
   {0x80177bb8, "Free"},
   {0x801780b0, "MemStats"},
   {0x801785dc, "Draw_shape"},
   {0x80178c5c, "draw_material"},
   {0x80179120, "draw_face"},
   {0x801799ac, "Draw_Camera"},
   {0x8017a010, "drawscene"},
   {0x8017a900, "Draw_group"},
   {0x8017b3dc, "find_thisface_verts"},
   {0x8017b764, "UpdateView"},
   {0x8017c300, "make_object"},
   {0x8017da04, "make_group"},
   {0x8017dc14, "addto_group"},
   {0x8017de80, "show_details"},
   {0x80184828, "dMakeNetFromShape"},
   {0x80188738, "dSetWorldPos"},
   {0x80184bf8, "dMakeobj"},
   {0x80184efc, "dAttach"},
   {0x80184fc4, "dAttachTo"},
   {0x80185a18, "chk_shapegen"},
   {0x801861b0, "dSetNodeGroup"},
   {0x80186bfc, "dUseObj"},
   {0x80186cdc, "dEndGroup"},
   {0x80186e74, "dSetInitPos"},
   {0x8018710c, "dSetVelocity"},
   {0x80187794, "dSetRelPos"},
   {0x801892d0, "dSetScale"},
   {0x8018973c, "dAddValPtr"},
   {0x8018a358, "dSetParmp"},
   {0x8018a590, "dSetType"},
   {0x8018a828, "dSetColNum"},
   {0x8018a9ec, "dSetMaterial"},
   {0x8018b0fc, "dSetMatrix"},
   {0x8018b210, "dSetRMatrix"},
   {0x8018b2e8, "dGetRMatrixPtr"},
   {0x8018b3a3, "dSetIMatrix"},
   {0x8018b4d4, "dGetMatrixPtr"},
   {0x8018b5e8, "dGetIMatrixPtr"},
   {0x8018b758, "dSetSkinWeight"},
   {0x8018b830, "get_objvalue"},
   {0x8018bc9c, "set_objvalue"},
   {0x8018be40, "adjust_gadget"},
   {0x8018cc54, "get_timernum"},
   {0x8018cea0, "start_timer"},
   {0x8018cf70, "restart_timer"},
   {0x8018d1f8, "myPrint1"},
   {0x8018d298, "myPrintf"},
   {0x8018d5f0, "imout"},
   {0x8018e16c, "gd_fopen"},
   {0x8017e370, "gd_loadtexture"},
   {0x8019abf8, "load_shapes2"},
   {0x8019b0d0, "dumpDispList"},
   {0x8019b158, "nextDisplayList"},
   {0x8019b1e4, "nextLightList"},
   {0x8019b278, "nextMtxList"},
   {0x8019b304, "nextVtxList"},
   {0x8019b390, "nextVpList"},
   {0x8019b53c, "printf"},
   {0x8019bb90, "gd_allocblock"},
   {0x8019bc88, "gd_malloc"},
   {0x8019c240, "setup_timers"},
   {0x8019c450, "gdm_init"},
   {0x8019c4ec, "gdm_setup"},
   {0x8019c684, "gdm_maketestdl"},
   {0x8019c9f8, "gdm_gettestdl"},
   {0x8019ca58, "yoshi_scene"},
   {0x8019cd88, "gdm_getpos"},
   {0x8019cf44, "alloc_displaylist"},
   {0x8019d3b8, "InitRSP"},
   {0x8019d42c, "InitRDP"},
   {0x8019e9f4, "gd_startdisplist"},
   {0x8019eb44, "gd_enddisplist"},
   {0x801a36b4, "gd_shading"},
   {0x801a371c, "gd_getproperty"},
   {0x801a3788, "gd_setproperty"},
   {0x801a4468, "_InitControllers"},
   {0x801a45e0, "gd_gentexture"},
   {0x801a5538, "gd_init"},
   {0x801a6430, "PutSprite"},
   {0x801a676c, "proc_dyn_list"},
   {0x801a7830, "stageName"},
   {0x801a7d40, "IsStereo"},
   {0x801a7d80, "sndQ"},
   {0x801a888c, "myWorld"},
   {0x801b5300, "rspinit_dl"},
   {0x801b5318, "rdpinit_dl"},
   {0x801b9ca0, "PermFreeBlocks"},
   {0x801b9ca4, "PermUsedBlocks"},
   {0x801b9ca8, "EmptyBlocks"},
   {0x802461cc, "dummy"},
   {0x802461fc, "SetupMessageQueues"},
   {0x802462e0, "AllocPool"},
   {0x80246338, "CreateThread"},
   {0x802465ec, "KickTask"},
   {0x80246648, "SendSPTaskDone"},
   {0x802469b8, "Thread3_Main"},
   {0x80246b74, "SendMessage"},
   {0x80246c10, "SendDisplayList"},
   {0x80246cf0, "Thread1_Idle"},
   {0x80246e70, "myRdpInit"},
   {0x802471a4, "myRspInit"},
   {0x80247284, "ClearZBuffer"},
   {0x802473c8, "DisplayFrameBuffer"},
   {0x802473b8, "ClearFrameBuffer"},
   {0x8024777c, "DisplayInit"},
   {0x80247b3c, "CreateTaskStructure"},
   {0x80247d14, "CleanupDisplayList"},
   {0x80248304, "XformPadValues"},
   {0x80248638, "DealWithPads"},
   {0x80248824, "InitController"},
   {0x80248af0, "Thread5_Debug"},
   {0x80249500, "Thread4"},
   {0x80277ee0, "SetSegmentBase"},
   {0x80277f20, "GetSetmentBase"},
   {0x80277f50, "SegmentedToVirtual"},
   {0x80277fa8, "MakePtr"},
   {0x80277ff0, "MovePtrTbl2Dmem"},
   {0x80278074, "InitMemPool"},
   {0x80278120, "_pool_alloc"},
   {0x80278238, "_pool_free"},
   {0x80278358, "_pool_realloc"},
   {0x802783c8, "PoolAvailable"},
   {0x802783e8, "PushPoolState"},
   {0x80278498, "PopPoolState"},
   {0x80278504, "DmaCopy"},
   {0x80278610, "DynamicCopy"},
   {0x8027868c, "DynamicIndexCopy"},
   {0x802786f0, "FixedCopy"},
   {0x802787d8, "UncIndexCopy"},
   {0x80278974, "CopyScriptInterpreter"},
   {0x80278fa0, "dobjCopy"},
   {0x80278c58, "subPrint"},
   {0x80279028, "DynamicObjectCopy"},
   {0x8027a1c8, "get_game_data"},
   {0x8027a8b0, "Menu"},
   {0x8027f4e0, "uncompress"},
   {0x80288e68, "ChangeCameraStatus"},
   {0x802ca618, "AnotherPrint"},
   {0x802d3ee4, "Print1"},
   {0x802d404c, "Print2"},
   {0x802d62d8, "PrintInt"},
   {0x802d6554, "PrintStr"},
   {0x802d66c0, "PrintXY"},
   {0x802d69f8, "addchar_displist"},
   {0x802d75dc, "addmini_displist"},
   {0x802d7b84, "PutString"},
   {0x802d82d4, "PutMiniString"},
   {0x802d89b8, "ShowCoins"},
   {0x802d8b34, "Int2Str"},
   {0x802d8a80, "ShowStars"},
   {0x802dbe68, "PauseScreen2"},
   {0x802dc478, "PauseScreen1"},
   {0x802dd838, "Save"},
   {0x802e3b1c, "SetCameraStatus"},
   {0x802e3b3c, "ShowCameraStatus"},
   {0x80317040, "soundAlloc"},
   {0x80318040, "BlockDmaCopy"},
   {0x8031950c, "InitAudioSystem"},
   {0x8031eb00, "SetSound"},
   {0x803223b0, "osSetTime"},
   {0x803223e0, "osMapTLB"},
   {0x803224a0, "osMapTLBRdb"},
   {0x803224f0, "proutSprintf"},
   {0x8032255c, "sprintf"},
   {0x803225a0, "osCreateMesgQueue"},
   {0x803225d0, "osSetEventMsg"},
   {0x80322640, "osViSetEventMsg"},
   {0x803226b0, "osCreateThread"},
   {0x80322800, "osRecvMesg"},
   {0x80322940, "osSpTaskDunno"},
   {0x80322a5c, "osSpTaskLoad"},
   {0x80322bbc, "osSpTaskStartGo"},
   {0x80322c00, "osSpTaskYield"},
   {0x80322c20, "osSendMsg"},
   {0x80322df0, "osStartThread"},
   {0x80322f40, "osWriteBackDCacheAll"},
   {0x80322f70, "osCreateViManager"},
   {0x803230f4, "__osViDevMgrMain"},
   {0x803232d0, "osViSetMode"},
   {0x80323340, "osViBlack"},
   {0x803233b0, "osViSetSpecialFeatures"},
   {0x80323570, "osCreatePiManager"},
   {0x803236f0, "osSetThreadPri"},
   {0x803237d0, "osInitialize"},
   {0x80323a00, "osViSwapBuffer"},
   {0x80323a60, "osContStartReadData"},
   {0x80323b24, "osContGetReadData"},
   {0x80323cc0, "osContInit"},
   {0x80323ebc, "__osContGetInitData"},
   {0x80323f8c, "__osPackRequestData"},
   {0x80324080, "osEepromProbe"},
   {0x803240f0, "__ull_rshift"},
   {0x8032411c, "__ull_rem"},
   {0x80324158, "__ull_div"},
   {0x80324194, "__ll_lshift"},
   {0x803241c0, "__ll_rem"},
   {0x803241fc, "__ll_div"},
   {0x80324258, "__ll_mul"},
   {0x80324288, "__ull_divremi"},
   {0x803242e8, "__ll_mod"},
   {0x80324384, "__ll_rshift"},
   {0x803243b0, "osInvalDCache"},
   {0x80324460, "osPiStartDma"},
   {0x80324570, "blkclr"},
   {0x80324610, "osInvalCache"},
   {0x80324690, "osEepromLongRead"},
   {0x803247d0, "osEepromLongWrite"},
   {0x80324910, "bcopy"},
   {0x80324c20, "guOrthoF"},
   {0x80324d74, "guFrustum"},
   {0x80324de0, "guPerspectiveF"},
   {0x80325010, "guPerspective"},
   {0x80325070, "osGetTime"},
   {0x80325310, "cosf"},
   {0x80325480, "sinf"},
   {0x803258d0, "guRotateRPYF"},
   {0x80325924, "guRotateRPY"},
   {0x80325970, "osAiSetFrequency"},
   {0x80325bd4, "alBnkfNew"},
   {0x80325cd8, "alSetFileNew"},
   {0x80325d20, "osWriteBackDCache"},
   {0x80325da0, "osAiGetLength"},
   {0x80325db0, "osAiSetNextBuffer"},
   {0x80325e60, "__osTimerServicesInit"},
   {0x80325eec, "__osTimerInterrupt"},
   {0x80326064, "__osSetTimerIntr"},
   {0x803260d8, "__osInsertTimer"},
   {0x80326260, "_Printf"},
   {0x803273f0, "memcpy"},
   {0x803274d0, "__osDisableInt"},
   {0x803274f0, "__osRestoreInt"},
   {0x80327510, "__osViInit"},
   {0x80327640, "__osExceptionPreamble"},
   {0x80327c80, "__osEnqueueAndYield"},
   {0x80327d10, "__osEnqueueThread"},
   {0x80327d58, "__osPopThread"},
/*   {0x80327d69, "__osDispatchThread"},*/
   {0x80327ea8, "__osCleanupThread"},
   {0x80327eb0, "osVirtualToPhysical"},
   {0x80327f30, "__osSpSetStatus"},
   {0x80327f40, "__osSpSetPc"},
   {0x80327f80, "__osSpRawStartDma"},
   {0x80328010, "__osSpDeviceBusy"},
   {0x80328040, "__osSpGetStatus"},
   {0x80328050, "osGetThreadPri"},
   {0x803283e0, "osGetCount"},
   {0x803283f0, "__osPiCreateAccessQueue"},
   {0x80328440, "__osPiGetAccess"},
   {0x80328484, "__osPiRelAccess"},
   {0x803284b0, "osPiRawStartDma"},
   {0x80328590, "__osDevMgrMain"},
   {0x80328720, "__osGetSR"},
   {0x80328730, "__osSetFpcCsr"},
   {0x80328740, "__osSiRawReadIo"},
   {0x80328790, "__osSiRawWriteIo"},
/*   {0x803287e0, "osMapTLBRdb"}, TODO: duplicate symbol name */
   {0x80328840, "osEPiRawReadIo"},
   {0x803288a0, "__osSiCreateAccessQueue"},
   {0x803288f0, "__osSiGetAccess"},
   {0x80328934, "__osSiRelAccess"},
   {0x80328960, "__osSiRawStartDma"},
   {0x80328a10, "osSetTimer"},
   {0x80328af0, "osEepromWrite"},
   {0x80328dac, "__osEepStatus"},
   {0x80328fd0, "osJamMesg"},
   {0x80329150, "osEepromRead"},
   {0x80329450, "guMtxF2L"},
   {0x80329550, "guMtxIdentF"},
   {0x80329750, "__osAiDeviceBusy"},
   {0x80329780, "__osSetCompare"},
   {0x8032af70, "__osProbeTLB"},
   {0x8032ace0, "__osSyncPutChars"},
   {0x8032ae10, "osSetIntMask"},
   {0x8032b030, "__osSiDeviceBusy"},
   {0x8032b200, "__osAtomicDec"},
/*   {0x8032b264, "rspbootTextStart"},*/
/*   {0x8032c740, "aspMain"},*/
/*   {0x8032d568, "loadTask"},*/
/*   {0x8032d56c, "AudTask"},*/
/*   {0x8032d570, "GfxTask"},*/
/*   {0x8032d574, "audioTask"},*/
/*   {0x8032d578, "gfxTask"},*/
/*   {0x80335010, "osViModeTable"},*/
/*   {0x803358d0, "__osViDevMgr"},*/
/*   {0x803358f0, "__osPiDevMgr"},*/
/*   {0x803359a8, "__osRunQueue"},*/
/*   {0x803359ac, "__osActiveQueue"},*/
/*   {0x803359b0, "__osRunningThread"},*/
/*   {0x80335a24, "__osViNext"},*/
/*   {0x80335a2c, "osViClock"},*/
   {0x80380014, "script_29"},
   {0x8038007c, "script_2a"},
   {0x803800bc, "script_2b"},
   {0x80380160, "script_2c"},
   {0x803801a0, "script_2d"},
   {0x8037fe94, "script_2e"},
   {0x8037ff14, "script_2f"},
   {0x80380274, "script_30"},
   {0x8037f920, "script_31"},
   {0x8038024c, "script_32"},
   {0x803801e0, "script_33"},
   {0x8037fde4, "script_34"},
   {0x8037fe2c, "script_35"},
   {0x80380300, "script_36"},
   {0x8038039c, "script_37"},
   {0x803803ec, "script_38"},
   {0x8037ff94, "script_39"},
   {0x8037fb18, "script_3a"},
   {0x8037fc38, "script_3b"},
   {0x80380434, "script_3c"},
   {0x092384+0x80245000, "RenderHud_TODO" }, // TODO: don't know
   {0x09608C+0x80245000, "RenderHud_CannonReticle" },
   {0x09DCF0+0x80245000, "RenderHud_Camera" },
   {0x09DE58+0x80245000, "RenderHud_CButtons" },
   {0x09E654+0x80245000, "RenderHud_Hp" },
   {0x09E744+0x80245000, "RenderHud_MarioLives" },
   {0x09E7A8+0x80245000, "RenderHud_Coins" },
   {0x09E80C+0x80245000, "RenderHud_Stars" },
   {0x09E8E4+0x80245000, "RenderHud_CannonReticle2" }, // TODO: why 2?
   {0x09E95C+0x80245000, "RenderHud_Timer" },
   {0x09EB3C+0x80245000, "ShowCameraStatus" },
   {0x09ED2C+0x80245000, "RenderHud" },
   {0x8037CD60, "geo_layout_00"},
   {0x8037CE24, "geo_layout_01"},
   {0x8037CEE8, "geo_layout_02"},
   {0x8037CF70, "geo_layout_03"},
   {0x8037CFC0, "geo_layout_04"},
   {0x8037D018, "geo_layout_05"},
   {0x8037D050, "geo_layout_06"},
   {0x8037D0D0, "geo_layout_07"},
   {0x8037D1D0, "geo_layout_08"},
   {0x8037D328, "geo_layout_09"},
   {0x8037D3A4, "geo_layout_0A"},
   {0x8037D48C, "geo_layout_0B"},
   {0x8037D500, "geo_layout_0C"},
   {0x8037D55C, "geo_layout_0D"},
   {0x8037D5D4, "geo_layout_0E"},
   {0x8037D640, "geo_layout_0F"},
   {0x8037D6F0, "geo_layout_10"},
   {0x8037D8D4, "geo_layout_11"},
   {0x8037D998, "geo_layout_12"},
   {0x8037DB74, "geo_layout_13"},
   {0x8037DC10, "geo_layout_14"},
   {0x8037DCD4, "geo_layout_15"},
   {0x8037DD4C, "geo_layout_16"},
   {0x8037DDDC, "geo_layout_17"},
   {0x8037DE34, "geo_layout_18"},
   {0x8037DE94, "geo_layout_19"},
   {0x8037DEF8, "geo_layout_1A"},
   {0x8037DF1C, "geo_layout_1B"},
   {0x8037DFD4, "geo_layout_1C"},
   {0x8037DA5C, "geo_layout_1D"},
   {0x8037DB50, "geo_layout_1E"},
   {0x8037D4DC, "geo_layout_1F"},
   {0x8037e2c4, "level_command_00"},
   {0x8037e388, "level_command_01"},
   {0x8037e404, "level_command_02"},
   {0x8037e47c, "level_command_03"},
   {0x8037e4fc, "level_command_04"},
   {0x8037e580, "level_command_05"},
   {0x8037e5b8, "level_command_06"},
   {0x8037e620, "level_command_07"},
   {0x8037e650, "level_command_08"},
   {0x8037e6d4, "level_command_09"},
   {0x8037e780, "level_command_0a"},
   {0x8037e7f8, "level_command_0b"},
   {0x8037e878, "level_command_0c"},
   {0x8037e8e8, "level_command_0d"},
   {0x8037e988, "level_command_0e"},
   {0x8037ea18, "level_command_0f"},
   {0x8037ea70, "level_command_10"},
   {0x8037ea98, "level_command_11"},
   {0x8037eb04, "level_command_12"},
   {0x8037eb98, "level_command_13"},
   {0x8037ebd4, "level_command_14"},
   {0x8037ec14, "level_command_15"},
   {0x8037ec54, "level_command_16"},
   {0x8037eca4, "level_command_17"},
   {0x8037ecf8, "level_command_18"},
   {0x8037ed48, "level_command_19"},
   {0x8037edf8, "level_command_1a"},
   {0x8037ee48, "level_command_1b"},
   {0x8037eea8, "level_command_1c"},
   {0x8037ef00, "level_command_1d"},
   {0x8037ef70, "level_command_1e"},
   {0x8037f010, "level_command_1f"},
   {0x8037f130, "level_command_20"},
   {0x8037f164, "level_command_21"},
   {0x8037f214, "level_command_22"},
   {0x8037f2a4, "level_command_23"},
   {0x8037f36c, "level_command_25"},
   {0x8037f45c, "level_command_24"},
   {0x8037f790, "level_command_28"},
   {0x8037f67c, "level_command_26"},
   {0x8037f994, "level_command_27"},
   {0x803805c8, "level_command_loading"},
};
