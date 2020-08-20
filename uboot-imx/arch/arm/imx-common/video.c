/*
 * Copyright (C) 2016 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/errno.h>
#include <asm/imx-common/video.h>
#include <linux/fb.h>


int env_to_int(char *env)
{
	int ret=0;
	char *env_value;
	env_value = getenv(env);
	if(env_value != NULL){
		ret = simple_strtoul(env_value, NULL, 0);
	}
	return ret;
}

struct display_info_t* env_init_video(struct display_info_t *pdev)
{
	//struct	fb_videomode* pmode = &pdev->mode;
#if 0
	pmode->xres = env_to_int("x_lcd");
	pmode->yres = env_to_int("y_lcd");
	pmode->pixclock = env_to_int("pix_clk_lcd");
	pmode->left_margin = env_to_int("hfp_lcd");
	pmode->right_margin = env_to_int("hbp_lcd");
	pmode->upper_margin = env_to_int("vfp_lcd");
	pmode->lower_margin = env_to_int("vbp_lcd");
	pmode->hsync_len = env_to_int("hsw_lcd");
	pmode->vsync_len = env_to_int("vsw_lcd");
	pmode->sync = 0;
	pmode->vmode = FB_VMODE_NONINTERLACED;
#endif

	pdev->mode.xres = env_to_int("x_lcd");
	pdev->mode.yres = env_to_int("y_lcd");
	pdev->mode.pixclock = KHZ2PICOS(env_to_int("pix_clk_lcd")/1000);//KHz
	pdev->mode.left_margin = env_to_int("hfp_lcd");
	pdev->mode.right_margin = env_to_int("hbp_lcd");
	pdev->mode.upper_margin = env_to_int("vfp_lcd");
	pdev->mode.lower_margin = env_to_int("vbp_lcd");
	pdev->mode.hsync_len = env_to_int("hsw_lcd");
	pdev->mode.vsync_len = env_to_int("vsw_lcd");
	pdev->mode.sync = 0;
	pdev->mode.vmode = FB_VMODE_NONINTERLACED;

	pdev->pixfmt = env_to_int("rgb_lcd");
}


#if 0
#define DEBUG
#include <linux/log_msg.h>
void print_mode_info(struct display_info_t *pdev)
{
	log_d("xres=%d\n",pdev->mode.xres);
	log_d("yres=%d\n",pdev->mode.yres);
	log_d("pixclock=%d\n",pdev->mode.pixclock);
	log_d("left_margin=%d\n",pdev->mode.left_margin);
	log_d("right_margin=%d\n",pdev->mode.right_margin);
	log_d("upper_margin=%d\n",pdev->mode.upper_margin);
	log_d("lower_margin=%d\n",pdev->mode.lower_margin);
	log_d("hsync_len=%d\n",pdev->mode.hsync_len);
	log_d("vsync_len=%d\n",pdev->mode.vsync_len);
	log_d("sync=%d\n",pdev->mode.sync);
	log_d("vmode=%d\n",pdev->mode.vmode);
	log_d("pixfmt=%d\n",pdev->pixfmt);
	log_d("bus=0x%x\n",pdev->bus);
}
#else
void print_mode_info(struct display_info_t *pdev){
    ;
}
#endif

int board_video_skip(void)
{
	int i;
	int ret;
//	char const *panel = getenv("panel");
	char const *panel = getenv("p_fb0"); //ref at modify.c

	if (panel == NULL) {
		for (i = 0; i < display_count; i++) {
			struct display_info_t const *dev = displays+i;
			if (dev->detect && dev->detect(dev)) {
				panel = dev->mode.name;
				printf("auto-detected panel %s\n", panel);
				break;
			}
		}
		if (!panel) {
			panel = displays[0].mode.name;
			printf("No panel detected: default to %s\n", panel);
			i = 0;
		}
	}
	else {
		if(get_product_fb(&displays[0])) {
			displays[0].mode.name = panel;
			i = 0;
		}
	}

	if (i < display_count) {
#if defined(CONFIG_VIDEO_IPUV3)
		ret = ipuv3_fb_init(&displays[i].mode, 0,
				    displays[i].pixfmt);
#elif defined(CONFIG_VIDEO_MXS)
		ret = mxs_lcd_panel_setup(displays[i].mode,
					displays[i].pixfmt,
				    displays[i].bus);
#endif
		if (!ret) {
			if (displays[i].enable)
				displays[i].enable(displays + i);

			printf("Display: %s (%ux%u) picxlock=%d\n",
			       displays[i].mode.name,
			       displays[i].mode.xres,
			       displays[i].mode.yres,displays[i].mode.pixclock);
		} else
			printf("LCD %s cannot be configured: %d\n",
			       displays[i].mode.name, ret);
	} else {
		printf("unsupported panel %s\n", panel);
		return -EINVAL;
	}

	print_mode_info(&displays[i]);
	return 0;
}

#ifdef CONFIG_IMX_HDMI
#include <asm/arch/mxc_hdmi.h>
#include <asm/io.h>
int detect_hdmi(struct display_info_t const *dev)
{
	struct hdmi_regs *hdmi	= (struct hdmi_regs *)HDMI_ARB_BASE_ADDR;
	return readb(&hdmi->phy_stat0) & HDMI_DVI_STAT;
}
#endif
