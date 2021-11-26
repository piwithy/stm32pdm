#ifndef PDM_FIR__H
#define PDM_FIR__H
/**
 * @file pdm_fir_.h
 * @brief File containing Generated LUT of the FIR filter
 * @date Generation: 26-11-2021 17:21:14
 */
/*
 +--------------------------------------------------+
 |                                                  |
 |         Generated by generate_pdm_fir.py         |
 |               26-11-2021 17:21:14                |
 |                                                  |
 +--------------------------------------------------+
*/

#define PDM_FTL_SCALE_BITS  30
#define PDM_FTL_TAPS        4

static const int byte_coeff[PDM_FTL_TAPS*2][256] = {
	{ // [0]
		-0x000171f4,  0x0000329e, -0x0000aff6,  0x0000f49c, -0x00012030,  0x00008462, -0x00005e32,  0x00014660,
		-0x0001533a,  0x00005158, -0x0000913c,  0x00011356, -0x00010176,  0x0000a31c, -0x00003f78,  0x0001651a,
		-0x00016810,  0x00003c82, -0x0000a612,  0x0000fe80, -0x0001164c,  0x00008e46, -0x0000544e,  0x00015044,
		-0x00014956,  0x00005b3c, -0x00008758,  0x00011d3a, -0x0000f792,  0x0000ad00, -0x00003594,  0x00016efe,
		-0x00016f6e,  0x00003524, -0x0000ad70,  0x0000f722, -0x00011daa,  0x000086e8, -0x00005bac,  0x000148e6,
		-0x000150b4,  0x000053de, -0x00008eb6,  0x000115dc, -0x0000fef0,  0x0000a5a2, -0x00003cf2,  0x000167a0,
		-0x0001658a,  0x00003f08, -0x0000a38c,  0x00010106, -0x000113c6,  0x000090cc, -0x000051c8,  0x000152ca,
		-0x000146d0,  0x00005dc2, -0x000084d2,  0x00011fc0, -0x0000f50c,  0x0000af86, -0x0000330e,  0x00017184,
		-0x00017188,  0x0000330a, -0x0000af8a,  0x0000f508, -0x00011fc4,  0x000084ce, -0x00005dc6,  0x000146cc,
		-0x000152ce,  0x000051c4, -0x000090d0,  0x000113c2, -0x0001010a,  0x0000a388, -0x00003f0c,  0x00016586,
		-0x000167a4,  0x00003cee, -0x0000a5a6,  0x0000feec, -0x000115e0,  0x00008eb2, -0x000053e2,  0x000150b0,
		-0x000148ea,  0x00005ba8, -0x000086ec,  0x00011da6, -0x0000f726,  0x0000ad6c, -0x00003528,  0x00016f6a,
		-0x00016f02,  0x00003590, -0x0000ad04,  0x0000f78e, -0x00011d3e,  0x00008754, -0x00005b40,  0x00014952,
		-0x00015048,  0x0000544a, -0x00008e4a,  0x00011648, -0x0000fe84,  0x0000a60e, -0x00003c86,  0x0001680c,
		-0x0001651e,  0x00003f74, -0x0000a320,  0x00010172, -0x0001135a,  0x00009138, -0x0000515c,  0x00015336,
		-0x00014664,  0x00005e2e, -0x00008466,  0x0001202c, -0x0000f4a0,  0x0000aff2, -0x000032a2,  0x000171f0,
		-0x000171f0,  0x000032a2, -0x0000aff2,  0x0000f4a0, -0x0001202c,  0x00008466, -0x00005e2e,  0x00014664,
		-0x00015336,  0x0000515c, -0x00009138,  0x0001135a, -0x00010172,  0x0000a320, -0x00003f74,  0x0001651e,
		-0x0001680c,  0x00003c86, -0x0000a60e,  0x0000fe84, -0x00011648,  0x00008e4a, -0x0000544a,  0x00015048,
		-0x00014952,  0x00005b40, -0x00008754,  0x00011d3e, -0x0000f78e,  0x0000ad04, -0x00003590,  0x00016f02,
		-0x00016f6a,  0x00003528, -0x0000ad6c,  0x0000f726, -0x00011da6,  0x000086ec, -0x00005ba8,  0x000148ea,
		-0x000150b0,  0x000053e2, -0x00008eb2,  0x000115e0, -0x0000feec,  0x0000a5a6, -0x00003cee,  0x000167a4,
		-0x00016586,  0x00003f0c, -0x0000a388,  0x0001010a, -0x000113c2,  0x000090d0, -0x000051c4,  0x000152ce,
		-0x000146cc,  0x00005dc6, -0x000084ce,  0x00011fc4, -0x0000f508,  0x0000af8a, -0x0000330a,  0x00017188,
		-0x00017184,  0x0000330e, -0x0000af86,  0x0000f50c, -0x00011fc0,  0x000084d2, -0x00005dc2,  0x000146d0,
		-0x000152ca,  0x000051c8, -0x000090cc,  0x000113c6, -0x00010106,  0x0000a38c, -0x00003f08,  0x0001658a,
		-0x000167a0,  0x00003cf2, -0x0000a5a2,  0x0000fef0, -0x000115dc,  0x00008eb6, -0x000053de,  0x000150b4,
		-0x000148e6,  0x00005bac, -0x000086e8,  0x00011daa, -0x0000f722,  0x0000ad70, -0x00003524,  0x00016f6e,
		-0x00016efe,  0x00003594, -0x0000ad00,  0x0000f792, -0x00011d3a,  0x00008758, -0x00005b3c,  0x00014956,
		-0x00015044,  0x0000544e, -0x00008e46,  0x0001164c, -0x0000fe80,  0x0000a612, -0x00003c82,  0x00016810,
		-0x0001651a,  0x00003f78, -0x0000a31c,  0x00010176, -0x00011356,  0x0000913c, -0x00005158,  0x0001533a,
		-0x00014660,  0x00005e32, -0x00008462,  0x00012030, -0x0000f49c,  0x0000aff6, -0x0000329e,  0x000171f4
	},
	{ // [1]
		-0x00990558, -0x002aaa32, -0x004dfc98,  0x00205e8e, -0x0067a67e,  0x0006b4a8, -0x001c9dbe,  0x0051bd68,
		-0x0079ab48, -0x000b5022, -0x002ea288,  0x003fb89e, -0x00484c6e,  0x00260eb8,  0x0002bc52,  0x00711778,
		-0x0085de2c, -0x00178306, -0x003ad56c,  0x003385ba, -0x00547f52,  0x0019dbd4, -0x00097692,  0x0064e494,
		-0x0066841c,  0x0007d70a, -0x001b7b5c,  0x0052dfca, -0x00352542,  0x003935e4,  0x0015e37e,  0x00843ea4,
		-0x008dcf3a, -0x001f7414, -0x0042c67a,  0x002b94ac, -0x005c7060,  0x0011eac6, -0x001167a0,  0x005cf386,
		-0x006e752a, -0x00001a04, -0x00236c6a,  0x004aeebc, -0x003d1650,  0x003144d6,  0x000df270,  0x007c4d96,
		-0x007aa80e, -0x000c4ce8, -0x002f9f4e,  0x003ebbd8, -0x00494934,  0x002511f2,  0x0001bf8c,  0x00701ab2,
		-0x005b4dfe,  0x00130d28, -0x0010453e,  0x005e15e8, -0x0029ef24,  0x00446c02,  0x0021199c,  0x008f74c2,
		-0x0092c396, -0x00246870, -0x0047bad6,  0x0026a050, -0x006164bc,  0x000cf66a, -0x00165bfc,  0x0057ff2a,
		-0x00736986, -0x00050e60, -0x002860c6,  0x0045fa60, -0x00420aac,  0x002c507a,  0x0008fe14,  0x0077593a,
		-0x007f9c6a, -0x00114144, -0x003493aa,  0x0039c77c, -0x004e3d90,  0x00201d96, -0x000334d0,  0x006b2656,
		-0x0060425a,  0x000e18cc, -0x0015399a,  0x0059218c, -0x002ee380,  0x003f77a6,  0x001c2540,  0x008a8066,
		-0x00878d78, -0x00193252, -0x003c84b8,  0x0031d66e, -0x00562e9e,  0x00182c88, -0x000b25de,  0x00633548,
		-0x00683368,  0x000627be, -0x001d2aa8,  0x0051307e, -0x0036d48e,  0x00378698,  0x00143432,  0x00828f58,
		-0x0074664c, -0x00060b26, -0x00295d8c,  0x0044fd9a, -0x00430772,  0x002b53b4,  0x0008014e,  0x00765c74,
		-0x00550c3c,  0x00194eea, -0x000a037c,  0x006457aa, -0x0023ad62,  0x004aadc4,  0x00275b5e,  0x0095b684,
		-0x0095b684, -0x00275b5e, -0x004aadc4,  0x0023ad62, -0x006457aa,  0x000a037c, -0x00194eea,  0x00550c3c,
		-0x00765c74, -0x0008014e, -0x002b53b4,  0x00430772, -0x0044fd9a,  0x00295d8c,  0x00060b26,  0x0074664c,
		-0x00828f58, -0x00143432, -0x00378698,  0x0036d48e, -0x0051307e,  0x001d2aa8, -0x000627be,  0x00683368,
		-0x00633548,  0x000b25de, -0x00182c88,  0x00562e9e, -0x0031d66e,  0x003c84b8,  0x00193252,  0x00878d78,
		-0x008a8066, -0x001c2540, -0x003f77a6,  0x002ee380, -0x0059218c,  0x0015399a, -0x000e18cc,  0x0060425a,
		-0x006b2656,  0x000334d0, -0x00201d96,  0x004e3d90, -0x0039c77c,  0x003493aa,  0x00114144,  0x007f9c6a,
		-0x0077593a, -0x0008fe14, -0x002c507a,  0x00420aac, -0x0045fa60,  0x002860c6,  0x00050e60,  0x00736986,
		-0x0057ff2a,  0x00165bfc, -0x000cf66a,  0x006164bc, -0x0026a050,  0x0047bad6,  0x00246870,  0x0092c396,
		-0x008f74c2, -0x0021199c, -0x00446c02,  0x0029ef24, -0x005e15e8,  0x0010453e, -0x00130d28,  0x005b4dfe,
		-0x00701ab2, -0x0001bf8c, -0x002511f2,  0x00494934, -0x003ebbd8,  0x002f9f4e,  0x000c4ce8,  0x007aa80e,
		-0x007c4d96, -0x000df270, -0x003144d6,  0x003d1650, -0x004aeebc,  0x00236c6a,  0x00001a04,  0x006e752a,
		-0x005cf386,  0x001167a0, -0x0011eac6,  0x005c7060, -0x002b94ac,  0x0042c67a,  0x001f7414,  0x008dcf3a,
		-0x00843ea4, -0x0015e37e, -0x003935e4,  0x00352542, -0x0052dfca,  0x001b7b5c, -0x0007d70a,  0x0066841c,
		-0x0064e494,  0x00097692, -0x0019dbd4,  0x00547f52, -0x003385ba,  0x003ad56c,  0x00178306,  0x0085de2c,
		-0x00711778, -0x0002bc52, -0x00260eb8,  0x00484c6e, -0x003fb89e,  0x002ea288,  0x000b5022,  0x0079ab48,
		-0x0051bd68,  0x001c9dbe, -0x0006b4a8,  0x0067a67e, -0x00205e8e,  0x004dfc98,  0x002aaa32,  0x00990558
	},
	{ // [2]
		-0x07834f9b, -0x03f40d41, -0x048a65db, -0x00fb2381, -0x0515cfef, -0x01868d95, -0x021ce62f,  0x01725c2b,
		-0x059320f9, -0x0203de9f, -0x029a3739,  0x00f50b21, -0x0325a14d,  0x0069a10d, -0x002cb78d,  0x03628acd,
		-0x0600700b, -0x02712db1, -0x0307864b,  0x0087bc0f, -0x0392f05f, -0x0003ae05, -0x009a069f,  0x02f53bbb,
		-0x04104169, -0x0080ff0f, -0x011757a9,  0x0277eab1, -0x01a2c1bd,  0x01ec809d,  0x01562803,  0x04e56a5d,
		-0x065d0247, -0x02cdbfed, -0x03641887,  0x002b29d3, -0x03ef829b, -0x00604041, -0x00f698db,  0x0298a97f,
		-0x046cd3a5, -0x00dd914b, -0x0173e9e5,  0x021b5875, -0x01ff53f9,  0x018fee61,  0x00f995c7,  0x0488d821,
		-0x04da22b7, -0x014ae05d, -0x01e138f7,  0x01ae0963, -0x026ca30b,  0x01229f4f,  0x008c46b5,  0x041b890f,
		-0x02e9f415,  0x00a54e45,  0x000ef5ab,  0x039e3805, -0x007c7469,  0x0312cdf1,  0x027c7557,  0x060bb7b1,
		-0x06a921b5, -0x0319df5b, -0x03b037f5, -0x0020f59b, -0x043ba209, -0x00ac5faf, -0x0142b849,  0x024c8a11,
		-0x04b8f313, -0x0129b0b9, -0x01c00953,  0x01cf3907, -0x024b7367,  0x0143cef3,  0x00ad7659,  0x043cb8b3,
		-0x05264225, -0x0196ffcb, -0x022d5865,  0x0161e9f5, -0x02b8c279,  0x00d67fe1,  0x00402747,  0x03cf69a1,
		-0x03361383,  0x00592ed7, -0x003d29c3,  0x03521897, -0x00c893d7,  0x02c6ae83,  0x023055e9,  0x05bf9843,
		-0x0582d461, -0x01f39207, -0x0289eaa1,  0x010557b9, -0x031554b5,  0x0079eda5, -0x001c6af5,  0x0372d765,
		-0x0392a5bf, -0x00036365, -0x0099bbff,  0x02f5865b, -0x01252613,  0x026a1c47,  0x01d3c3ad,  0x05630607,
		-0x03fff4d1, -0x0070b277, -0x01070b11,  0x02883749, -0x01927525,  0x01fccd35,  0x0166749b,  0x04f5b6f5,
		-0x020fc62f,  0x017f7c2b,  0x00e92391,  0x047865eb,  0x005db97d,  0x03ecfbd7,  0x0356a33d,  0x06e5e597,
		-0x06e5e597, -0x0356a33d, -0x03ecfbd7, -0x005db97d, -0x047865eb, -0x00e92391, -0x017f7c2b,  0x020fc62f,
		-0x04f5b6f5, -0x0166749b, -0x01fccd35,  0x01927525, -0x02883749,  0x01070b11,  0x0070b277,  0x03fff4d1,
		-0x05630607, -0x01d3c3ad, -0x026a1c47,  0x01252613, -0x02f5865b,  0x0099bbff,  0x00036365,  0x0392a5bf,
		-0x0372d765,  0x001c6af5, -0x0079eda5,  0x031554b5, -0x010557b9,  0x0289eaa1,  0x01f39207,  0x0582d461,
		-0x05bf9843, -0x023055e9, -0x02c6ae83,  0x00c893d7, -0x03521897,  0x003d29c3, -0x00592ed7,  0x03361383,
		-0x03cf69a1, -0x00402747, -0x00d67fe1,  0x02b8c279, -0x0161e9f5,  0x022d5865,  0x0196ffcb,  0x05264225,
		-0x043cb8b3, -0x00ad7659, -0x0143cef3,  0x024b7367, -0x01cf3907,  0x01c00953,  0x0129b0b9,  0x04b8f313,
		-0x024c8a11,  0x0142b849,  0x00ac5faf,  0x043ba209,  0x0020f59b,  0x03b037f5,  0x0319df5b,  0x06a921b5,
		-0x060bb7b1, -0x027c7557, -0x0312cdf1,  0x007c7469, -0x039e3805, -0x000ef5ab, -0x00a54e45,  0x02e9f415,
		-0x041b890f, -0x008c46b5, -0x01229f4f,  0x026ca30b, -0x01ae0963,  0x01e138f7,  0x014ae05d,  0x04da22b7,
		-0x0488d821, -0x00f995c7, -0x018fee61,  0x01ff53f9, -0x021b5875,  0x0173e9e5,  0x00dd914b,  0x046cd3a5,
		-0x0298a97f,  0x00f698db,  0x00604041,  0x03ef829b, -0x002b29d3,  0x03641887,  0x02cdbfed,  0x065d0247,
		-0x04e56a5d, -0x01562803, -0x01ec809d,  0x01a2c1bd, -0x0277eab1,  0x011757a9,  0x0080ff0f,  0x04104169,
		-0x02f53bbb,  0x009a069f,  0x0003ae05,  0x0392f05f, -0x0087bc0f,  0x0307864b,  0x02712db1,  0x0600700b,
		-0x03628acd,  0x002cb78d, -0x0069a10d,  0x0325a14d, -0x00f50b21,  0x029a3739,  0x0203de9f,  0x059320f9,
		-0x01725c2b,  0x021ce62f,  0x01868d95,  0x0515cfef,  0x00fb2381,  0x048a65db,  0x03f40d41,  0x07834f9b
	},
	{ // [3]
		-0x17e23908, -0x10ae59b6, -0x10d17c02, -0x099d9cb0, -0x1115daca, -0x09e1fb78, -0x0a051dc4, -0x02d13e72,
		-0x1177d3ca, -0x0a43f478, -0x0a6716c4, -0x03333772, -0x0aab758c, -0x0377963a, -0x039ab886,  0x039926cc,
		-0x11f2565c, -0x0abe770a, -0x0ae19956, -0x03adba04, -0x0b25f81e, -0x03f218cc, -0x04153b18,  0x031ea43a,
		-0x0b87f11e, -0x045411cc, -0x04773418,  0x02bcab3a, -0x04bb92e0,  0x02784c72,  0x02552a26,  0x09890978,
		-0x127f4d90, -0x0b4b6e3e, -0x0b6e908a, -0x043ab138, -0x0bb2ef52, -0x047f1000, -0x04a2324c,  0x0291ad06,
		-0x0c14e852, -0x04e10900, -0x05042b4c,  0x022fb406, -0x05488a14,  0x01eb553e,  0x01c832f2,  0x08fc1244,
		-0x0c8f6ae4, -0x055b8b92, -0x057eadde,  0x01b53174, -0x05c30ca6,  0x0170d2ac,  0x014db060,  0x08818fb2,
		-0x062505a6,  0x010ed9ac,  0x00ebb760,  0x081f96b2,  0x00a75898,  0x07db37ea,  0x07b8159e,  0x0eebf4f0,
		-0x13181aaa, -0x0be43b58, -0x0c075da4, -0x04d37e52, -0x0c4bbc6c, -0x0517dd1a, -0x053aff66,  0x01f8dfec,
		-0x0cadb56c, -0x0579d61a, -0x059cf866,  0x0196e6ec, -0x05e1572e,  0x01528824,  0x012f65d8,  0x0863452a,
		-0x0d2837fe, -0x05f458ac, -0x06177af8,  0x011c645a, -0x065bd9c0,  0x00d80592,  0x00b4e346,  0x07e8c298,
		-0x06bdd2c0,  0x00760c92,  0x0052ea46,  0x0786c998,  0x000e8b7e,  0x07426ad0,  0x071f4884,  0x0e5327d6,
		-0x0db52f32, -0x06814fe0, -0x06a4722c,  0x008f6d26, -0x06e8d0f4,  0x004b0e5e,  0x0027ec12,  0x075bcb64,
		-0x074ac9f4, -0x0016eaa2, -0x003a0cee,  0x06f9d264, -0x007e6bb6,  0x06b5739c,  0x06925150,  0x0dc630a2,
		-0x07c54c86, -0x00916d34, -0x00b48f80,  0x067f4fd2, -0x00f8ee48,  0x063af10a,  0x0617cebe,  0x0d4bae10,
		-0x015ae748,  0x05d8f80a,  0x05b5d5be,  0x0ce9b510,  0x057176f6,  0x0ca55648,  0x0c8233fc,  0x13b6134e,
		-0x13b6134e, -0x0c8233fc, -0x0ca55648, -0x057176f6, -0x0ce9b510, -0x05b5d5be, -0x05d8f80a,  0x015ae748,
		-0x0d4bae10, -0x0617cebe, -0x063af10a,  0x00f8ee48, -0x067f4fd2,  0x00b48f80,  0x00916d34,  0x07c54c86,
		-0x0dc630a2, -0x06925150, -0x06b5739c,  0x007e6bb6, -0x06f9d264,  0x003a0cee,  0x0016eaa2,  0x074ac9f4,
		-0x075bcb64, -0x0027ec12, -0x004b0e5e,  0x06e8d0f4, -0x008f6d26,  0x06a4722c,  0x06814fe0,  0x0db52f32,
		-0x0e5327d6, -0x071f4884, -0x07426ad0, -0x000e8b7e, -0x0786c998, -0x0052ea46, -0x00760c92,  0x06bdd2c0,
		-0x07e8c298, -0x00b4e346, -0x00d80592,  0x065bd9c0, -0x011c645a,  0x06177af8,  0x05f458ac,  0x0d2837fe,
		-0x0863452a, -0x012f65d8, -0x01528824,  0x05e1572e, -0x0196e6ec,  0x059cf866,  0x0579d61a,  0x0cadb56c,
		-0x01f8dfec,  0x053aff66,  0x0517dd1a,  0x0c4bbc6c,  0x04d37e52,  0x0c075da4,  0x0be43b58,  0x13181aaa,
		-0x0eebf4f0, -0x07b8159e, -0x07db37ea, -0x00a75898, -0x081f96b2, -0x00ebb760, -0x010ed9ac,  0x062505a6,
		-0x08818fb2, -0x014db060, -0x0170d2ac,  0x05c30ca6, -0x01b53174,  0x057eadde,  0x055b8b92,  0x0c8f6ae4,
		-0x08fc1244, -0x01c832f2, -0x01eb553e,  0x05488a14, -0x022fb406,  0x05042b4c,  0x04e10900,  0x0c14e852,
		-0x0291ad06,  0x04a2324c,  0x047f1000,  0x0bb2ef52,  0x043ab138,  0x0b6e908a,  0x0b4b6e3e,  0x127f4d90,
		-0x09890978, -0x02552a26, -0x02784c72,  0x04bb92e0, -0x02bcab3a,  0x04773418,  0x045411cc,  0x0b87f11e,
		-0x031ea43a,  0x04153b18,  0x03f218cc,  0x0b25f81e,  0x03adba04,  0x0ae19956,  0x0abe770a,  0x11f2565c,
		-0x039926cc,  0x039ab886,  0x0377963a,  0x0aab758c,  0x03333772,  0x0a6716c4,  0x0a43f478,  0x1177d3ca,
		 0x02d13e72,  0x0a051dc4,  0x09e1fb78,  0x1115daca,  0x099d9cb0,  0x10d17c02,  0x10ae59b6,  0x17e23908
	},
	{ // [4]
		-0x17e23908, -0x13b6134e, -0x13181aaa, -0x0eebf4f0, -0x127f4d90, -0x0e5327d6, -0x0db52f32, -0x09890978,
		-0x11f2565c, -0x0dc630a2, -0x0d2837fe, -0x08fc1244, -0x0c8f6ae4, -0x0863452a, -0x07c54c86, -0x039926cc,
		-0x1177d3ca, -0x0d4bae10, -0x0cadb56c, -0x08818fb2, -0x0c14e852, -0x07e8c298, -0x074ac9f4, -0x031ea43a,
		-0x0b87f11e, -0x075bcb64, -0x06bdd2c0, -0x0291ad06, -0x062505a6, -0x01f8dfec, -0x015ae748,  0x02d13e72,
		-0x1115daca, -0x0ce9b510, -0x0c4bbc6c, -0x081f96b2, -0x0bb2ef52, -0x0786c998, -0x06e8d0f4, -0x02bcab3a,
		-0x0b25f81e, -0x06f9d264, -0x065bd9c0, -0x022fb406, -0x05c30ca6, -0x0196e6ec, -0x00f8ee48,  0x03333772,
		-0x0aab758c, -0x067f4fd2, -0x05e1572e, -0x01b53174, -0x05488a14, -0x011c645a, -0x007e6bb6,  0x03adba04,
		-0x04bb92e0, -0x008f6d26,  0x000e8b7e,  0x043ab138,  0x00a75898,  0x04d37e52,  0x057176f6,  0x099d9cb0,
		-0x10d17c02, -0x0ca55648, -0x0c075da4, -0x07db37ea, -0x0b6e908a, -0x07426ad0, -0x06a4722c, -0x02784c72,
		-0x0ae19956, -0x06b5739c, -0x06177af8, -0x01eb553e, -0x057eadde, -0x01528824, -0x00b48f80,  0x0377963a,
		-0x0a6716c4, -0x063af10a, -0x059cf866, -0x0170d2ac, -0x05042b4c, -0x00d80592, -0x003a0cee,  0x03f218cc,
		-0x04773418, -0x004b0e5e,  0x0052ea46,  0x047f1000,  0x00ebb760,  0x0517dd1a,  0x05b5d5be,  0x09e1fb78,
		-0x0a051dc4, -0x05d8f80a, -0x053aff66, -0x010ed9ac, -0x04a2324c, -0x00760c92,  0x0027ec12,  0x045411cc,
		-0x04153b18,  0x0016eaa2,  0x00b4e346,  0x04e10900,  0x014db060,  0x0579d61a,  0x0617cebe,  0x0a43f478,
		-0x039ab886,  0x00916d34,  0x012f65d8,  0x055b8b92,  0x01c832f2,  0x05f458ac,  0x06925150,  0x0abe770a,
		 0x02552a26,  0x06814fe0,  0x071f4884,  0x0b4b6e3e,  0x07b8159e,  0x0be43b58,  0x0c8233fc,  0x10ae59b6,
		-0x10ae59b6, -0x0c8233fc, -0x0be43b58, -0x07b8159e, -0x0b4b6e3e, -0x071f4884, -0x06814fe0, -0x02552a26,
		-0x0abe770a, -0x06925150, -0x05f458ac, -0x01c832f2, -0x055b8b92, -0x012f65d8, -0x00916d34,  0x039ab886,
		-0x0a43f478, -0x0617cebe, -0x0579d61a, -0x014db060, -0x04e10900, -0x00b4e346, -0x0016eaa2,  0x04153b18,
		-0x045411cc, -0x0027ec12,  0x00760c92,  0x04a2324c,  0x010ed9ac,  0x053aff66,  0x05d8f80a,  0x0a051dc4,
		-0x09e1fb78, -0x05b5d5be, -0x0517dd1a, -0x00ebb760, -0x047f1000, -0x0052ea46,  0x004b0e5e,  0x04773418,
		-0x03f218cc,  0x003a0cee,  0x00d80592,  0x05042b4c,  0x0170d2ac,  0x059cf866,  0x063af10a,  0x0a6716c4,
		-0x0377963a,  0x00b48f80,  0x01528824,  0x057eadde,  0x01eb553e,  0x06177af8,  0x06b5739c,  0x0ae19956,
		 0x02784c72,  0x06a4722c,  0x07426ad0,  0x0b6e908a,  0x07db37ea,  0x0c075da4,  0x0ca55648,  0x10d17c02,
		-0x099d9cb0, -0x057176f6, -0x04d37e52, -0x00a75898, -0x043ab138, -0x000e8b7e,  0x008f6d26,  0x04bb92e0,
		-0x03adba04,  0x007e6bb6,  0x011c645a,  0x05488a14,  0x01b53174,  0x05e1572e,  0x067f4fd2,  0x0aab758c,
		-0x03333772,  0x00f8ee48,  0x0196e6ec,  0x05c30ca6,  0x022fb406,  0x065bd9c0,  0x06f9d264,  0x0b25f81e,
		 0x02bcab3a,  0x06e8d0f4,  0x0786c998,  0x0bb2ef52,  0x081f96b2,  0x0c4bbc6c,  0x0ce9b510,  0x1115daca,
		-0x02d13e72,  0x015ae748,  0x01f8dfec,  0x062505a6,  0x0291ad06,  0x06bdd2c0,  0x075bcb64,  0x0b87f11e,
		 0x031ea43a,  0x074ac9f4,  0x07e8c298,  0x0c14e852,  0x08818fb2,  0x0cadb56c,  0x0d4bae10,  0x1177d3ca,
		 0x039926cc,  0x07c54c86,  0x0863452a,  0x0c8f6ae4,  0x08fc1244,  0x0d2837fe,  0x0dc630a2,  0x11f2565c,
		 0x09890978,  0x0db52f32,  0x0e5327d6,  0x127f4d90,  0x0eebf4f0,  0x13181aaa,  0x13b6134e,  0x17e23908
	},
	{ // [5]
		-0x07834f9b, -0x06e5e597, -0x06a921b5, -0x060bb7b1, -0x065d0247, -0x05bf9843, -0x0582d461, -0x04e56a5d,
		-0x0600700b, -0x05630607, -0x05264225, -0x0488d821, -0x04da22b7, -0x043cb8b3, -0x03fff4d1, -0x03628acd,
		-0x059320f9, -0x04f5b6f5, -0x04b8f313, -0x041b890f, -0x046cd3a5, -0x03cf69a1, -0x0392a5bf, -0x02f53bbb,
		-0x04104169, -0x0372d765, -0x03361383, -0x0298a97f, -0x02e9f415, -0x024c8a11, -0x020fc62f, -0x01725c2b,
		-0x0515cfef, -0x047865eb, -0x043ba209, -0x039e3805, -0x03ef829b, -0x03521897, -0x031554b5, -0x0277eab1,
		-0x0392f05f, -0x02f5865b, -0x02b8c279, -0x021b5875, -0x026ca30b, -0x01cf3907, -0x01927525, -0x00f50b21,
		-0x0325a14d, -0x02883749, -0x024b7367, -0x01ae0963, -0x01ff53f9, -0x0161e9f5, -0x01252613, -0x0087bc0f,
		-0x01a2c1bd, -0x010557b9, -0x00c893d7, -0x002b29d3, -0x007c7469,  0x0020f59b,  0x005db97d,  0x00fb2381,
		-0x048a65db, -0x03ecfbd7, -0x03b037f5, -0x0312cdf1, -0x03641887, -0x02c6ae83, -0x0289eaa1, -0x01ec809d,
		-0x0307864b, -0x026a1c47, -0x022d5865, -0x018fee61, -0x01e138f7, -0x0143cef3, -0x01070b11, -0x0069a10d,
		-0x029a3739, -0x01fccd35, -0x01c00953, -0x01229f4f, -0x0173e9e5, -0x00d67fe1, -0x0099bbff,  0x0003ae05,
		-0x011757a9, -0x0079eda5, -0x003d29c3,  0x00604041,  0x000ef5ab,  0x00ac5faf,  0x00e92391,  0x01868d95,
		-0x021ce62f, -0x017f7c2b, -0x0142b849, -0x00a54e45, -0x00f698db, -0x00592ed7, -0x001c6af5,  0x0080ff0f,
		-0x009a069f,  0x00036365,  0x00402747,  0x00dd914b,  0x008c46b5,  0x0129b0b9,  0x0166749b,  0x0203de9f,
		-0x002cb78d,  0x0070b277,  0x00ad7659,  0x014ae05d,  0x00f995c7,  0x0196ffcb,  0x01d3c3ad,  0x02712db1,
		 0x01562803,  0x01f39207,  0x023055e9,  0x02cdbfed,  0x027c7557,  0x0319df5b,  0x0356a33d,  0x03f40d41,
		-0x03f40d41, -0x0356a33d, -0x0319df5b, -0x027c7557, -0x02cdbfed, -0x023055e9, -0x01f39207, -0x01562803,
		-0x02712db1, -0x01d3c3ad, -0x0196ffcb, -0x00f995c7, -0x014ae05d, -0x00ad7659, -0x0070b277,  0x002cb78d,
		-0x0203de9f, -0x0166749b, -0x0129b0b9, -0x008c46b5, -0x00dd914b, -0x00402747, -0x00036365,  0x009a069f,
		-0x0080ff0f,  0x001c6af5,  0x00592ed7,  0x00f698db,  0x00a54e45,  0x0142b849,  0x017f7c2b,  0x021ce62f,
		-0x01868d95, -0x00e92391, -0x00ac5faf, -0x000ef5ab, -0x00604041,  0x003d29c3,  0x0079eda5,  0x011757a9,
		-0x0003ae05,  0x0099bbff,  0x00d67fe1,  0x0173e9e5,  0x01229f4f,  0x01c00953,  0x01fccd35,  0x029a3739,
		 0x0069a10d,  0x01070b11,  0x0143cef3,  0x01e138f7,  0x018fee61,  0x022d5865,  0x026a1c47,  0x0307864b,
		 0x01ec809d,  0x0289eaa1,  0x02c6ae83,  0x03641887,  0x0312cdf1,  0x03b037f5,  0x03ecfbd7,  0x048a65db,
		-0x00fb2381, -0x005db97d, -0x0020f59b,  0x007c7469,  0x002b29d3,  0x00c893d7,  0x010557b9,  0x01a2c1bd,
		 0x0087bc0f,  0x01252613,  0x0161e9f5,  0x01ff53f9,  0x01ae0963,  0x024b7367,  0x02883749,  0x0325a14d,
		 0x00f50b21,  0x01927525,  0x01cf3907,  0x026ca30b,  0x021b5875,  0x02b8c279,  0x02f5865b,  0x0392f05f,
		 0x0277eab1,  0x031554b5,  0x03521897,  0x03ef829b,  0x039e3805,  0x043ba209,  0x047865eb,  0x0515cfef,
		 0x01725c2b,  0x020fc62f,  0x024c8a11,  0x02e9f415,  0x0298a97f,  0x03361383,  0x0372d765,  0x04104169,
		 0x02f53bbb,  0x0392a5bf,  0x03cf69a1,  0x046cd3a5,  0x041b890f,  0x04b8f313,  0x04f5b6f5,  0x059320f9,
		 0x03628acd,  0x03fff4d1,  0x043cb8b3,  0x04da22b7,  0x0488d821,  0x05264225,  0x05630607,  0x0600700b,
		 0x04e56a5d,  0x0582d461,  0x05bf9843,  0x065d0247,  0x060bb7b1,  0x06a921b5,  0x06e5e597,  0x07834f9b
	},
	{ // [6]
		-0x00990558, -0x0095b684, -0x0092c396, -0x008f74c2, -0x008dcf3a, -0x008a8066, -0x00878d78, -0x00843ea4,
		-0x0085de2c, -0x00828f58, -0x007f9c6a, -0x007c4d96, -0x007aa80e, -0x0077593a, -0x0074664c, -0x00711778,
		-0x0079ab48, -0x00765c74, -0x00736986, -0x00701ab2, -0x006e752a, -0x006b2656, -0x00683368, -0x0064e494,
		-0x0066841c, -0x00633548, -0x0060425a, -0x005cf386, -0x005b4dfe, -0x0057ff2a, -0x00550c3c, -0x0051bd68,
		-0x0067a67e, -0x006457aa, -0x006164bc, -0x005e15e8, -0x005c7060, -0x0059218c, -0x00562e9e, -0x0052dfca,
		-0x00547f52, -0x0051307e, -0x004e3d90, -0x004aeebc, -0x00494934, -0x0045fa60, -0x00430772, -0x003fb89e,
		-0x00484c6e, -0x0044fd9a, -0x00420aac, -0x003ebbd8, -0x003d1650, -0x0039c77c, -0x0036d48e, -0x003385ba,
		-0x00352542, -0x0031d66e, -0x002ee380, -0x002b94ac, -0x0029ef24, -0x0026a050, -0x0023ad62, -0x00205e8e,
		-0x004dfc98, -0x004aadc4, -0x0047bad6, -0x00446c02, -0x0042c67a, -0x003f77a6, -0x003c84b8, -0x003935e4,
		-0x003ad56c, -0x00378698, -0x003493aa, -0x003144d6, -0x002f9f4e, -0x002c507a, -0x00295d8c, -0x00260eb8,
		-0x002ea288, -0x002b53b4, -0x002860c6, -0x002511f2, -0x00236c6a, -0x00201d96, -0x001d2aa8, -0x0019dbd4,
		-0x001b7b5c, -0x00182c88, -0x0015399a, -0x0011eac6, -0x0010453e, -0x000cf66a, -0x000a037c, -0x0006b4a8,
		-0x001c9dbe, -0x00194eea, -0x00165bfc, -0x00130d28, -0x001167a0, -0x000e18cc, -0x000b25de, -0x0007d70a,
		-0x00097692, -0x000627be, -0x000334d0,  0x00001a04,  0x0001bf8c,  0x00050e60,  0x0008014e,  0x000b5022,
		 0x0002bc52,  0x00060b26,  0x0008fe14,  0x000c4ce8,  0x000df270,  0x00114144,  0x00143432,  0x00178306,
		 0x0015e37e,  0x00193252,  0x001c2540,  0x001f7414,  0x0021199c,  0x00246870,  0x00275b5e,  0x002aaa32,
		-0x002aaa32, -0x00275b5e, -0x00246870, -0x0021199c, -0x001f7414, -0x001c2540, -0x00193252, -0x0015e37e,
		-0x00178306, -0x00143432, -0x00114144, -0x000df270, -0x000c4ce8, -0x0008fe14, -0x00060b26, -0x0002bc52,
		-0x000b5022, -0x0008014e, -0x00050e60, -0x0001bf8c, -0x00001a04,  0x000334d0,  0x000627be,  0x00097692,
		 0x0007d70a,  0x000b25de,  0x000e18cc,  0x001167a0,  0x00130d28,  0x00165bfc,  0x00194eea,  0x001c9dbe,
		 0x0006b4a8,  0x000a037c,  0x000cf66a,  0x0010453e,  0x0011eac6,  0x0015399a,  0x00182c88,  0x001b7b5c,
		 0x0019dbd4,  0x001d2aa8,  0x00201d96,  0x00236c6a,  0x002511f2,  0x002860c6,  0x002b53b4,  0x002ea288,
		 0x00260eb8,  0x00295d8c,  0x002c507a,  0x002f9f4e,  0x003144d6,  0x003493aa,  0x00378698,  0x003ad56c,
		 0x003935e4,  0x003c84b8,  0x003f77a6,  0x0042c67a,  0x00446c02,  0x0047bad6,  0x004aadc4,  0x004dfc98,
		 0x00205e8e,  0x0023ad62,  0x0026a050,  0x0029ef24,  0x002b94ac,  0x002ee380,  0x0031d66e,  0x00352542,
		 0x003385ba,  0x0036d48e,  0x0039c77c,  0x003d1650,  0x003ebbd8,  0x00420aac,  0x0044fd9a,  0x00484c6e,
		 0x003fb89e,  0x00430772,  0x0045fa60,  0x00494934,  0x004aeebc,  0x004e3d90,  0x0051307e,  0x00547f52,
		 0x0052dfca,  0x00562e9e,  0x0059218c,  0x005c7060,  0x005e15e8,  0x006164bc,  0x006457aa,  0x0067a67e,
		 0x0051bd68,  0x00550c3c,  0x0057ff2a,  0x005b4dfe,  0x005cf386,  0x0060425a,  0x00633548,  0x0066841c,
		 0x0064e494,  0x00683368,  0x006b2656,  0x006e752a,  0x00701ab2,  0x00736986,  0x00765c74,  0x0079ab48,
		 0x00711778,  0x0074664c,  0x0077593a,  0x007aa80e,  0x007c4d96,  0x007f9c6a,  0x00828f58,  0x0085de2c,
		 0x00843ea4,  0x00878d78,  0x008a8066,  0x008dcf3a,  0x008f74c2,  0x0092c396,  0x0095b684,  0x00990558
	},
	{ // [7]
		-0x000171f4, -0x000171f0, -0x00017188, -0x00017184, -0x00016f6e, -0x00016f6a, -0x00016f02, -0x00016efe,
		-0x00016810, -0x0001680c, -0x000167a4, -0x000167a0, -0x0001658a, -0x00016586, -0x0001651e, -0x0001651a,
		-0x0001533a, -0x00015336, -0x000152ce, -0x000152ca, -0x000150b4, -0x000150b0, -0x00015048, -0x00015044,
		-0x00014956, -0x00014952, -0x000148ea, -0x000148e6, -0x000146d0, -0x000146cc, -0x00014664, -0x00014660,
		-0x00012030, -0x0001202c, -0x00011fc4, -0x00011fc0, -0x00011daa, -0x00011da6, -0x00011d3e, -0x00011d3a,
		-0x0001164c, -0x00011648, -0x000115e0, -0x000115dc, -0x000113c6, -0x000113c2, -0x0001135a, -0x00011356,
		-0x00010176, -0x00010172, -0x0001010a, -0x00010106, -0x0000fef0, -0x0000feec, -0x0000fe84, -0x0000fe80,
		-0x0000f792, -0x0000f78e, -0x0000f726, -0x0000f722, -0x0000f50c, -0x0000f508, -0x0000f4a0, -0x0000f49c,
		-0x0000aff6, -0x0000aff2, -0x0000af8a, -0x0000af86, -0x0000ad70, -0x0000ad6c, -0x0000ad04, -0x0000ad00,
		-0x0000a612, -0x0000a60e, -0x0000a5a6, -0x0000a5a2, -0x0000a38c, -0x0000a388, -0x0000a320, -0x0000a31c,
		-0x0000913c, -0x00009138, -0x000090d0, -0x000090cc, -0x00008eb6, -0x00008eb2, -0x00008e4a, -0x00008e46,
		-0x00008758, -0x00008754, -0x000086ec, -0x000086e8, -0x000084d2, -0x000084ce, -0x00008466, -0x00008462,
		-0x00005e32, -0x00005e2e, -0x00005dc6, -0x00005dc2, -0x00005bac, -0x00005ba8, -0x00005b40, -0x00005b3c,
		-0x0000544e, -0x0000544a, -0x000053e2, -0x000053de, -0x000051c8, -0x000051c4, -0x0000515c, -0x00005158,
		-0x00003f78, -0x00003f74, -0x00003f0c, -0x00003f08, -0x00003cf2, -0x00003cee, -0x00003c86, -0x00003c82,
		-0x00003594, -0x00003590, -0x00003528, -0x00003524, -0x0000330e, -0x0000330a, -0x000032a2, -0x0000329e,
		 0x0000329e,  0x000032a2,  0x0000330a,  0x0000330e,  0x00003524,  0x00003528,  0x00003590,  0x00003594,
		 0x00003c82,  0x00003c86,  0x00003cee,  0x00003cf2,  0x00003f08,  0x00003f0c,  0x00003f74,  0x00003f78,
		 0x00005158,  0x0000515c,  0x000051c4,  0x000051c8,  0x000053de,  0x000053e2,  0x0000544a,  0x0000544e,
		 0x00005b3c,  0x00005b40,  0x00005ba8,  0x00005bac,  0x00005dc2,  0x00005dc6,  0x00005e2e,  0x00005e32,
		 0x00008462,  0x00008466,  0x000084ce,  0x000084d2,  0x000086e8,  0x000086ec,  0x00008754,  0x00008758,
		 0x00008e46,  0x00008e4a,  0x00008eb2,  0x00008eb6,  0x000090cc,  0x000090d0,  0x00009138,  0x0000913c,
		 0x0000a31c,  0x0000a320,  0x0000a388,  0x0000a38c,  0x0000a5a2,  0x0000a5a6,  0x0000a60e,  0x0000a612,
		 0x0000ad00,  0x0000ad04,  0x0000ad6c,  0x0000ad70,  0x0000af86,  0x0000af8a,  0x0000aff2,  0x0000aff6,
		 0x0000f49c,  0x0000f4a0,  0x0000f508,  0x0000f50c,  0x0000f722,  0x0000f726,  0x0000f78e,  0x0000f792,
		 0x0000fe80,  0x0000fe84,  0x0000feec,  0x0000fef0,  0x00010106,  0x0001010a,  0x00010172,  0x00010176,
		 0x00011356,  0x0001135a,  0x000113c2,  0x000113c6,  0x000115dc,  0x000115e0,  0x00011648,  0x0001164c,
		 0x00011d3a,  0x00011d3e,  0x00011da6,  0x00011daa,  0x00011fc0,  0x00011fc4,  0x0001202c,  0x00012030,
		 0x00014660,  0x00014664,  0x000146cc,  0x000146d0,  0x000148e6,  0x000148ea,  0x00014952,  0x00014956,
		 0x00015044,  0x00015048,  0x000150b0,  0x000150b4,  0x000152ca,  0x000152ce,  0x00015336,  0x0001533a,
		 0x0001651a,  0x0001651e,  0x00016586,  0x0001658a,  0x000167a0,  0x000167a4,  0x0001680c,  0x00016810,
		 0x00016efe,  0x00016f02,  0x00016f6a,  0x00016f6e,  0x00017184,  0x00017188,  0x000171f0,  0x000171f4
	}
};
#endif // PDM_FIR__H
