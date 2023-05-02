/*
 * Do not edit this file.  It was automatically generated
 * by "rgb2c" from the file "../rdpatt/Texture/checker.rgb".
 *
 *   Size: 32 x 32
 *   Number of channels: 3
 *   Number of bits per texel: 4 (G_IM_SIZ_4b)
 *   Format of texel: G_IM_FMT_IA
 *
 * Example usage:
 *
 *   gsSPTexture (128, 128, 0, G_TX_RENDERTILE, 1),
 *   gsDPPipeSync (),
 *   gsDPSetCombineMode (G_CC_MODULATEIA, G_CC_MODULATEIA),
 *   gsDPSetTexturePersp (G_TP_PERSP),
 *   gsDPSetTextureDetail (G_TD_CLAMP),
 *   gsDPSetTextureLOD (G_TL_TILE),
 *   gsDPSetTextureLUT (G_TT_NONE),
 *   gsDPSetTextureFilter (G_TF_BILERP),
 *   gsDPSetTextureConvert(G_TC_FILT),
 *   gsDPLoadTextureBlock (IA4checker, G_IM_FMT_IA, G_IM_SIZ_4b, 32, 32, 0
 *     G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR,
 *     G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD),
 */

#if 0	/* Image preview */
	+--------------------------------+
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	|.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#|
	|#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.#.|
	+--------------------------------+
#endif

static Gfx IA4checker_C_dummy_aligner1[] = { gsSPEndDisplayList() };

unsigned char IA4checker[] = {
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
	0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 0xf1, 
	0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 
};

