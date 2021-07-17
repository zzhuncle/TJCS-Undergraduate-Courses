/*
*********************************************************************************************************
*                                            uC/USB-Device
*                                    The Embedded USB Device Stack
*
*                         (c) Copyright 2004-2019; Silicon Laboratories Inc.,
*                                400 W. Cesar Chavez, Austin, TX 78701
*
*                   All rights reserved. Protected by international copyright laws.
*
*                  Your use of this software is subject to your acceptance of the terms
*                  of a Silicon Labs Micrium software license, which can be obtained by
*                  contacting info@micrium.com. If you do not agree to the terms of this
*                  license, you may not use this software.
*
*                  Please help us continue to provide the Embedded community with the finest
*                  software available. Your honesty is greatly appreciated.
*
*                    You can find our product's documentation at: doc.micrium.com
*
*                          For more information visit us at: www.micrium.com
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                   USB AUDIO DEVICE CONFIGURATION FILE
*
* Filename : usbd_audio_dev_cfg.c
* Version  : V4.05.06
*********************************************************************************************************
* Note(s) : (1) This audio configuration file allows to build the following topologies:
*
*               (a) Speaker
*
*                   IT (USB OUT) ---------------> FU ---------------> OT (speaker)
*
*               (b) Microphone
*
*                   OT (USB IN)  <--------------- FU <--------------- IT (microphone)
*
*               (a) Headset
*
*                   IT (USB OUT) ---------------> FU ---------------> OT (speaker)
*                   OT (USB IN)  <--------------- FU <--------------- IT (microphone)
*********************************************************************************************************
*/

#include  <usbd_audio_dev_cfg.h>


/*
*********************************************************************************************************
*                                    USB AUDIO DEVICE CONFIGURATION
*
* Note(s) : (1) Type I format type descriptor reports the sampling frequency capabilities of the
*               isochronous data endpoint with the field 'bSamFreqType'. The possible values are:
*
*               (a) USBD_AUDIO_FMT_TYPE_I_SAM_FREQ_CONTINUOUS   Continuous sampling frequency
*
*               (b) 1..255                                      number of discrete sampling frequencies
*                                                               supported by the isochronous data endpoint.
*
*           (2) These fields relates to 'bLockDelayUnits' and 'wLockDelay' fields of Class-Specific AS
*               Isochronous Audio Data Endpoint Descriptor.  'bLockDelayUnits' and 'wLockDelay' indicate
*               to the Host how long it takes for the clock recovery circuitry of this endpoint to lock
*               and reliably produce or consume the audio data stream. Only applicable for synchronous
*               and adaptive endpoints.
*               See 'USB Device Class Definition for Audio Devices, Release 1.0, March 18, 1998',
*               section 4.6.1.2 for more details about class-specific endpoint descriptor.
*
*           (3) Feedback endpoint refresh rate represents the exponent of power of 2 ms. The value must
*               be between 1 (2 ms) and 9 (512 ms).
*********************************************************************************************************
*/

#define  COUNT_OF(a)                              (sizeof(a)/sizeof((a)[0u]))

CPU_INT08U  Mic_IT_ID;
CPU_INT08U  Mic_OT_USB_IN_ID;
CPU_INT08U  Mic_FU_ID;
#if (APP_CFG_USBD_AUDIO_SIMULATION_LOOP_EN == DEF_ENABLED)
CPU_INT08U  Speaker_IT_USB_OUT_ID;
CPU_INT08U  Speaker_OT_ID;
CPU_INT08U  Speaker_FU_ID;
#endif

                                                                /* --------------- INPUT TERMINAL CFG ----------------- */
const  USBD_AUDIO_IT_CFG  USBD_IT_MIC_Cfg = {                   /* MICROPHONE.                                          */
    USBD_AUDIO_TERMINAL_TYPE_MIC,
    USBD_AUDIO_MONO,                                            /* Nbr of log output ch in Terminal Output.             */
    USBD_AUDIO_SPATIAL_LOCATION_LEFT_FRONT,                     /* Spatial location of log ch.                          */
    DEF_DISABLED,                                               /* Dis or En Copy Protection.                           */
    USBD_AUDIO_CPL_NONE,                                        /* Copy Protection Level.                               */
    "IT Microphone"                                             /* Str describing IT. Obtained by Str Desc.             */
};

const  USBD_AUDIO_IT_CFG  USBD_IT_USB_OUT_Cfg = {               /* USB STREAMING OUT.                                   */
    USBD_AUDIO_TERMINAL_TYPE_USB_STREAMING,
    USBD_AUDIO_MONO,                                            /* Nbr of log output ch in Terminal Output.             */
    USBD_AUDIO_SPATIAL_LOCATION_LEFT_FRONT,                     /* Spatial location of log ch.                          */
    DEF_DISABLED,                                               /* Dis or En Copy Protection.                           */
    USBD_AUDIO_CPL_NONE,                                        /* Copy Protection Level.                               */
    "IT USB OUT"                                                /* Str describing IT. Obtained by Str Desc.             */
};

                                                                /* -------------- OUTPUT TERMINAL CFG ----------------- */
const  USBD_AUDIO_OT_CFG  USBD_OT_USB_IN_Cfg = {
    USBD_AUDIO_TERMINAL_TYPE_USB_STREAMING,
    DEF_DISABLED,                                               /* Dis or En Copy Protection.                           */
    "OT USB IN"                                                 /* Str describing OT. Obtained by Str Desc.             */
};

const  USBD_AUDIO_OT_CFG  USBD_OT_SPEAKER_Cfg = {
    USBD_AUDIO_TERMINAL_TYPE_SPEAKER,
    DEF_DISABLED,                                               /* Dis or En Copy Protection.                           */
    "OT Speaker"                                                /* Str describing OT. Obtained by Str Desc.             */
};

                                                                /* ---------------- FEATURE UNIT CFG ------------------ */
CPU_INT16U  FU_LogChCtrlTbl[] = {
   (USBD_AUDIO_FU_CTRL_MUTE | USBD_AUDIO_FU_CTRL_VOL),          /* Master ch.                                           */
    USBD_AUDIO_FU_CTRL_NONE,                                    /* Log ch #1.                                           */
    USBD_AUDIO_FU_CTRL_NONE                                     /* Log ch #2.                                           */
};


const  USBD_AUDIO_FU_CFG  USBD_FU_SPEAKER_Cfg = {
    USBD_AUDIO_MONO,                                            /* Nbr of log ch.                                       */
   &FU_LogChCtrlTbl[0u],                                        /* Ptr to Feature Unit Controls tbl.                    */
    "FU Speaker"                                                /* Str describing FU. Obtained by Str Desc.             */
};

const  USBD_AUDIO_FU_CFG  USBD_FU_MIC_Cfg = {
    USBD_AUDIO_MONO,                                            /* Nbr of log ch.                                       */
   &FU_LogChCtrlTbl[0u],                                        /* Ptr to Feature Unit Controls tbl.                    */
    "FU Microphone"                                             /* Str describing FU. Obtained by Str Desc.             */
};

                                                                /* -------------- AUDIO STREAMING IF CFG -------------- */
const  USBD_AUDIO_STREAM_CFG  USBD_MicStreamCfg = {
    USBD_AUDIO_DEV_CFG_RECORD_NBR_BUF,
    USBD_AUDIO_DEV_CFG_RECORD_CORR_PERIOD
};

#if (APP_CFG_USBD_AUDIO_SIMULATION_LOOP_EN == DEF_ENABLED)
const  USBD_AUDIO_STREAM_CFG  USBD_SpeakerStreamCfg = {
    USBD_AUDIO_DEV_CFG_PLAYBACK_NBR_BUF,
    USBD_AUDIO_DEV_CFG_PLAYBACK_CORR_PERIOD
};
#endif


CPU_INT32U  AS_SamFreqTbl[] = {
#if (APP_CFG_USBD_AUDIO_SIMULATION_LOOP_EN == DEF_DISABLED)
    USBD_AUDIO_FMT_TYPE_I_SAMFREQ_44_1KHZ,
#endif
    USBD_AUDIO_FMT_TYPE_I_SAMFREQ_48KHZ
};

const  USBD_AUDIO_AS_ALT_CFG  USBD_AS_IF1_Alt1_SpeakerCfg = {
                                                                /* AS IF RELATED:                                       */
    1u,                                                         /* Delay introduced by the data path.                   */
    USBD_AUDIO_DATA_FMT_TYPE_I_PCM,                             /* Audio data fmt used to comm with this IF.            */
    USBD_AUDIO_MONO,                                            /* Nbr of physical ch in the audio data stream.         */
    USBD_AUDIO_FMT_TYPE_I_SUBFRAME_SIZE_2,                      /* Nbr of bytes occupied by one audio subframe.         */
    USBD_AUDIO_FMT_TYPE_I_BIT_RESOLUTION_16,                    /* Effectively used bits in an audio subframe.          */
    COUNT_OF(AS_SamFreqTbl),                                    /* Nbr of sampling frequencies (see Note #1).           */
    0u,                                                         /* Lower bound in Hz of the sampling freq range.        */
    0u,                                                         /* Upper bound in Hz of the sampling freq range.        */
   &AS_SamFreqTbl[0u],                                          /* Tbl of discrete sampling frequencies.                */
                                                                /* ENDPOINT CONFIG:                                     */
    DEF_NO,                                                     /* Direction OUT                                        */
    USBD_EP_TYPE_SYNC_ADAPTIVE,                                 /* Synchronization type supported by Isoc EP.           */
                                                                /* CLASS SPECIFIC EP RELATED:                           */
    USBD_AUDIO_AS_EP_CTRL_SAMPLING_FREQ,                        /* Class specific controls supported by Isoc EP.        */
    USBD_AUDIO_AS_EP_LOCK_DLY_UND,                              /* See Note #2.                                         */
    0u,                                                         /* See Note #2.                                         */
                                                                /* SYNCH EP RELATED:                                    */
    0u,                                                         /* Refresh Rate (see Note #3).                          */
};

const  USBD_AUDIO_AS_ALT_CFG  USBD_AS_IF2_Alt1_MicCfg = {
                                                                /* AS IF RELATED:                                       */
    1u,                                                         /* Delay introduced by the data path.                   */
    USBD_AUDIO_DATA_FMT_TYPE_I_PCM,                             /* Audio data fmt used to comm with this IF.            */
                                                                /* TYPE I FMT RELATED:                                  */
    USBD_AUDIO_MONO,                                            /* Nbr of physical ch in the audio data stream.         */
    USBD_AUDIO_FMT_TYPE_I_SUBFRAME_SIZE_2,                      /* Nbr of bytes occupied by one audio subframe.         */
    USBD_AUDIO_FMT_TYPE_I_BIT_RESOLUTION_16,                    /* Effectively used bits in an audio subframe.          */
    COUNT_OF(AS_SamFreqTbl),                                    /* Nbr of sampling frequencies (see Note #1).           */
    0u,                                                         /* Lower bound in Hz of the sampling freq range.        */
    0u,                                                         /* Upper bound in Hz of the sampling freq range.        */
   &AS_SamFreqTbl[0u],                                          /* Tbl of discrete sampling frequencies.                */
                                                                /* ENDPOINT CONFIG:                                     */
    DEF_YES,                                                    /* Direction IN                                         */
    USBD_EP_TYPE_SYNC_ASYNC,                                    /* Synchronization type supported by Isoc EP.           */
                                                                /* CLASS SPECIFIC EP RELATED:                           */
    USBD_AUDIO_AS_EP_CTRL_SAMPLING_FREQ,                        /* Class specific controls supported by Isoc EP.        */
    USBD_AUDIO_AS_EP_LOCK_DLY_UND,                              /* See Note #2.                                         */
    0u,                                                         /* See Note #2.                                         */
                                                                /* SYNCH EP RELATED:                                    */
    0u,                                                         /* Refresh Rate (see Note #3).                          */
};

USBD_AUDIO_AS_ALT_CFG  *USBD_AS_IF1_Alt_SpeakerCfgTbl[] = {
   &USBD_AS_IF1_Alt1_SpeakerCfg,
};

USBD_AUDIO_AS_ALT_CFG  *USBD_AS_IF2_Alt_MicCfgTbl[] = {
   &USBD_AS_IF2_Alt1_MicCfg,
};

USBD_AUDIO_AS_IF_CFG  USBD_AS_IF1_SpeakerCfg = {
   &USBD_AS_IF1_Alt_SpeakerCfgTbl[0u],
    COUNT_OF(USBD_AS_IF1_Alt_SpeakerCfgTbl),
};

USBD_AUDIO_AS_IF_CFG  USBD_AS_IF2_MicCfg = {
   &USBD_AS_IF2_Alt_MicCfgTbl[0u],
    COUNT_OF(USBD_AS_IF2_Alt_MicCfgTbl),
};

USBD_AUDIO_AS_IF_CFG  *USBD_AS_IF_Cfg_Tbl[] = {
   &USBD_AS_IF1_SpeakerCfg,
   &USBD_AS_IF2_MicCfg,
};

CPU_INT08U  USBD_AS_IF_Cfg_Nbr = COUNT_OF(USBD_AS_IF_Cfg_Tbl);

