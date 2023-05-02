/*
 * YUVbars.h
 *
 *  Simple test for YUV texture loads and render
 *
 *
 * YUV: set size to 16-bit
 *
 * from memspan:
 * 
 * 63....................0
 * U0 Y0 V0 Y1 U2 Y2 V2 Y3
 * U4 Y4 V4 Y5 U6 Y6 V6 Y7
 * 
 * in tmem:
 * 
 * Bank:	L0	L1	L2	L3	H0	H1	H2	H3
 * Even:	UV0	UV2	UV4	UV6	Y0Y1	Y2Y3	Y4Y5	Y6Y7
 * Odd:		UV4	UV6	UV0	UV2	Y4Y5	Y6Y7	Y0Y1	Y2Y3
 *
 *
 *
 *  Texture is a set of horizontal solid colored bars, 
 *
 *      	R  G  B			Y  U  V
 *		--------		--------
 *  Red, 	ff,00,00		51,5a,ef
 *  Green, 	00,ff,00		90,35,22
 *  Blue, 	00,00,ff		28,ef,6d
 *  Cyan, 	00,ff,ff		a9,a5,10
 *  Yellow, 	ff,ff,00		d1,10,92
 *  Magenta, 	ff,00,ff		6a,ca,dd
 *  Black, 	00,00,00		10,80,80
 *  White	ff,ff,ff		ea,80,80
 *
 *  RJM 12/20/94
 */

/* a 1x8 color bar texture */

unsigned char rdptex_YUVbars[] = {
	0x5a, 0x51, 0xef, 0x51,
	0x35, 0x90, 0x22, 0x90,
	0xef, 0x28, 0x6d, 0x28,
	0xa5, 0xa9, 0x10, 0xa9,
	0x10, 0xd1, 0x92, 0xd1,
	0xca, 0x6a, 0xdd, 0x6a,
	0x80, 0x10, 0x80, 0x10,
	0x80, 0xea, 0x80, 0xea,
};
