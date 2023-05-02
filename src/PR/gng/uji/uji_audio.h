#define NBUFFERS       20

#define AUDIO_HEAP_SIZE         300000

#define MAX_VOICES              22

#define NUM_SMPL_PER_FRAME      1280
#define DMA_BUFFER_LENGTH       0x600  /* Larger buffers result in fewer DMA' but more  */
                                       /* memory being used.  */


#define NUM_DMA_MESSAGES        32     /* The maximum number of DMAs any one frame can  */
                                       /* have.                                         */

#define FRAME_LAG               1      /* The number of frames to keep a dma buffer.    */
                                       /* Increasing this number causes buffers to not  */
                                       /* be deleted as quickly. This results in fewer  */
                                       /* DMA's but you need more buffers.              */

typedef struct 
{
    ALLink      node;
    int         startAddr;
    u32         lastFrame;
    char        *ptr;
} DMABuffer;

typedef struct 
{
    u8          initialized;
    DMABuffer   *firstUsed;
    DMABuffer   *firstFree;

} DMAState;

extern u8 _sfxbankSegmentRomStart[],  _sfxbankSegmentRomEnd[];
extern u8 _sfxtableSegmentRomStart[], _sfxtableSegmentRomEnd[];


/* $B8z2L2;MQ(B */
#define EVT_COUNT       32

/* $B%a%$%s%k!<%A%s$+$i8F$S=P$9%5%&%s%I%k!<%A%s(B */
extern void chkAppendSoundPlayer(void);
extern void chkPlaySound( int sound_number );

#define SOUND_PASS   0
#define SOUND_FAIL   1
#define SOUND_NONE   2


