// Based on Adam Taylor code for DP on Ultra96
// July 10 - dhq 

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xvtc.h"
#include "vga_modes.h"
#include "xv_tpg.h"
#include "xvidc.h"
#include "xavbuf.h"
#include "xavbuf_clk.h"
#include "xvidc.h"
#include "xdpdma_video_example.h"

XVtc	VtcInst;
XVtc_Config *vtc_config ;
XV_tpg tpg;
XV_tpg_Config *tpg_config;

#define DEBUG 1

void setup_tpg(void);


int main()
{

	XVtc_Timing vtcTiming;
	XVtc_SourceSelect SourceSelect;
	VideoMode video;

    init_platform();


    vtc_config = XVtc_LookupConfig(XPAR_VTC_0_DEVICE_ID);
    XVtc_CfgInitialize(&VtcInst, vtc_config, vtc_config->BaseAddress);
    run_dppsu();
    /* Initialize Video Pipeline driver */


    video = VMODE_1920x1080;
	vtcTiming.HActiveVideo = video.width;	/**< Horizontal Active Video Size */
	vtcTiming.HFrontPorch = video.hps - video.width;	/**< Horizontal Front Porch Size */
	vtcTiming.HSyncWidth = video.hpe - video.hps;		/**< Horizontal Sync Width */
	vtcTiming.HBackPorch = video.hmax - video.hpe + 1;		/**< Horizontal Back Porch Size */
	vtcTiming.HSyncPolarity = video.hpol;	/**< Horizontal Sync Polarity */
	vtcTiming.VActiveVideo = video.height;	/**< Vertical Active Video Size */
	vtcTiming.V0FrontPorch = video.vps - video.height;	/**< Vertical Front Porch Size */
	vtcTiming.V0SyncWidth = video.vpe - video.vps;	/**< Vertical Sync Width */
	vtcTiming.V0BackPorch = video.vmax - video.vpe + 1;;	/**< Horizontal Back Porch Size */
	vtcTiming.V1FrontPorch = video.vps - video.height;	/**< Vertical Front Porch Size */
	vtcTiming.V1SyncWidth = video.vpe - video.vps;	/**< Vertical Sync Width */
	vtcTiming.V1BackPorch = video.vmax - video.vpe + 1;;	/**< Horizontal Back Porch Size */
	vtcTiming.VSyncPolarity = video.vpol;	/**< Vertical Sync Polarity */
	vtcTiming.Interlaced = 0;


    memset((void *)&SourceSelect, 0, sizeof(SourceSelect));
	SourceSelect.VBlankPolSrc = 1;
	SourceSelect.VSyncPolSrc = 1;
	SourceSelect.HBlankPolSrc = 1;
	SourceSelect.HSyncPolSrc = 1;
	SourceSelect.ActiveVideoPolSrc = 1;
	SourceSelect.ActiveChromaPolSrc= 1;
	SourceSelect.VChromaSrc = 1;
	SourceSelect.VActiveSrc = 1;
	SourceSelect.VBackPorchSrc = 1;
	SourceSelect.VSyncSrc = 1;
	SourceSelect.VFrontPorchSrc = 1;
	SourceSelect.VTotalSrc = 1;
	SourceSelect.HActiveSrc = 1;
	SourceSelect.HBackPorchSrc = 1;
	SourceSelect.HSyncSrc = 1;
	SourceSelect.HFrontPorchSrc = 1;
	SourceSelect.HTotalSrc = 1;
	XVtc_RegUpdateEnable(&VtcInst);
	XVtc_SetGeneratorTiming(&VtcInst, &vtcTiming);
	XVtc_SetSource(&VtcInst, &SourceSelect);
	XVtc_EnableGenerator(&VtcInst);
	XVtc_Enable(&VtcInst);

	 setup_tpg();

	 while(1){

	 }


    cleanup_platform();
    return 0;
}

void setup_tpg(void)
{
    u32 height,width,status;

    tpg_config = XV_tpg_LookupConfig(XPAR_XV_TPG_0_DEVICE_ID);
    XV_tpg_CfgInitialize(&tpg, tpg_config, tpg_config->BaseAddress);

#ifdef DEBUG
    status = XV_tpg_IsReady(&tpg);
	printf("TPG Status %u \n\r", (unsigned int) status);
#endif
	XV_tpg_Set_height(&tpg, (u32)1080);
	XV_tpg_Set_width(&tpg, (u32)1920);
	height = XV_tpg_Get_height(&tpg);
	width = XV_tpg_Get_width(&tpg);
	XV_tpg_Set_colorFormat(&tpg,XVIDC_CSF_RGB);
	XV_tpg_Set_maskId(&tpg, 0x0);
	XV_tpg_Set_motionSpeed(&tpg, 0x4);
#ifdef DEBUG
	printf("info from tpg %u %u \n\r", (unsigned int)height, (unsigned int)width);
#endif
	XV_tpg_Set_bckgndId(&tpg,XTPG_BKGND_TARTAN_COLOR_BARS);

#ifdef DEBUG
	status = XV_tpg_Get_bckgndId(&tpg);
	printf("Status %x \n\r", (unsigned int) status);
#endif
	XV_tpg_EnableAutoRestart(&tpg);
	XV_tpg_Start(&tpg);

#ifdef DEBUG
	status = XV_tpg_IsIdle(&tpg);
	printf("Status %u \n\r", (unsigned int) status);
#endif

}
