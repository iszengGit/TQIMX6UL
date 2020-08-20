/*
 * Copyright (C) 2015-2016 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6UL 14x14 EVK board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __MX6UL_14X14_EVK_CONFIG_H
#define __MX6UL_14X14_EVK_CONFIG_H


#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include "mx6_common.h"
#include <asm/imx-common/gpio.h>

/* uncomment for PLUGIN mode support */
/* #define CONFIG_USE_PLUGIN */

/* uncomment for SECURE mode support */
/* #define CONFIG_SECURE_BOOT */

/* uncomment for BEE support, needs to enable CONFIG_CMD_FUSE */
/* #define CONFIG_CMD_BEE */

#ifdef CONFIG_SECURE_BOOT
#ifndef CONFIG_CSF_SIZE
#define CONFIG_CSF_SIZE 0x4000
#endif
#endif

#define CUR_VERSION 2016
#define is_mx6ul_9x9_evk()	CONFIG_IS_ENABLED(TARGET_MX6UL_9X9_EVK)

#ifdef CONFIG_TARGET_MX6UL_9X9_EVK
#define PHYS_SDRAM_SIZE		SZ_256M
#define CONFIG_BOOTARGS_CMA_SIZE   "cma=96M "
#else
#define PHYS_SDRAM_SIZE		SZ_256M
#define CONFIG_BOOTARGS_CMA_SIZE   ""
/* DCDC used on 14x14 EVK, no PMIC */
#undef CONFIG_LDO_BYPASS_CHECK
#endif

#define CONFIG_TQIMX6UL 1

#define FREQ_528M           528000000
#define FREQ_396M           396000000
#define CPU_CORE_FREQ       FREQ_528M

/* SPL options */
/* We default not support SPL
 * #define CONFIG_SPL_LIBCOMMON_SUPPORT
 * #define CONFIG_SPL_MMC_SUPPORT
 * #include "imx6_spl.h"
*/

#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

#define CONFIG_DISPLAY_CPUINFO
#define CONFIG_DISPLAY_BOARDINFO

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(16 * SZ_1M)

#define CONFIG_BOARD_EARLY_INIT_F
#define CONFIG_BOARD_LATE_INIT

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

/* MMC Configs */
#ifdef CONFIG_FSL_USDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC2_BASE_ADDR
/* FLASH and environment organization */

#define CONFIG_SYS_BOOT_NAND

/* NAND pin conflicts with usdhc2 */
#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_SYS_FSL_USDHC_NUM	1
#else
#define CONFIG_SYS_FSL_USDHC_NUM	2
#endif
#endif

/* I2C configs */
#define CONFIG_CMD_I2C
#ifdef CONFIG_CMD_I2C
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_SPEED		100000

/* PMIC only for 9X9 EVK */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE3000
#define CONFIG_POWER_PFUZE3000_I2C_ADDR  0x08
#endif

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1

#ifdef CONFIG_SYS_BOOT_NAND
#define CONFIG_MFG_NAND_PARTITION "mtdparts=gpmi-nand:64m(boot),16m(kernel),16m(dtb),1m(misc),-(rootfs) "
#else
#define CONFIG_MFG_NAND_PARTITION ""
#endif

#define UBOOT_NAME	"u-boot.imx"
#define LOGO_NAME 	"logo.bmp"
#define DTB_NAME	"tq-imx6ul.dtb"
#define KERNEL_NAME	"zImage"
#define ROOT_NAME_UBI   "rootfs.img"
#define ROOT_NAME_YAFFS "rootfs.yaffs2"
/*set 1 for sdcard's u-boot.imx*/
#define AUTO_DOWN_FROME_SDCARD 	"0"

#define CONFIG_MFG_ENV_SETTINGS \
    "mfgtool_args=setenv bootargs console=${console},${baudrate} " \
        CONFIG_BOOTARGS_CMA_SIZE \
        CONFIG_MFG_NAND_PARTITION \
        "rdinit=/linuxrc " \
        "g_mass_storage.stall=0 g_mass_storage.removable=1 " \
        "g_mass_storage.file=/fat g_mass_storage.ro=1 " \
        "g_mass_storage.idVendor=0x066F g_mass_storage.idProduct=0x37FF "\
        "g_mass_storage.iSerialNumber=\"\" "\
        "clk_ignore_unused "\
        "\0" \
    "initrd_addr=0x83800000\0" \
    "initrd_high=0xffffffff\0" \
    "bootcmd_mfg=run mfgtool_args;bootz ${loadaddr} ${initrd_addr} ${fdt_addr};\0" \


#define ENV_IMG_NAME \
                "ubootimgname="UBOOT_NAME"\0" \
                "logoimgname="LOGO_NAME"\0" \
                "kernelimgname="KERNEL_NAME"\0" \
                "kloadaddr=0x80800000\0"\
                "dtbname="DTB_NAME"\0"\
                "dtbloadaddr=0x83000000\0"\
                "rootimgname_yaffs="ROOT_NAME_YAFFS"\0" \
                "rootimgname_ubi="ROOT_NAME_UBI"\0" \
                "cpaddr=0x80800000\0"

#if 0
#define LOAD_SPLASH	"load_splash=nand read.e ${splashimage} logo \0"
#else
#endif

#define ENV_LOGO \
	"loadlogo=nand read.e ${splashimage} logo\0" \
	"splashimage=0x85800000\0" \
	"splashpos=m,m\0"

#define ENV_NET \
                "ethaddr=00:03:7f:09:0b:ad\0" \
                "serverip=192.168.1.49\0" \
                "ipaddr=192.168.1.41\0" \
                "netmask=255.255.255.0\0"\
                "gatewayip=192.168.1.2\0"\
                "nfsserverip=192.168.1.49\0" \
                "nfsipaddr=192.168.1.41\0" \
                "nfs_dir=/opt/EmbedSky/nfs/imx6ul/rootfs\0"

#define CONFIG_TQ_ENV_SETTINGS \
                ENV_NET \
                ENV_LOGO \
                ENV_IMG_NAME \
        "mtdparts=" MTDPARTS_DEFAULT "\0"\
        "mtdids=" MTDIDS_DEFAULT "\0"\
        "kimagesize=0xa00000\0"\
        "defroot=root_nand\0"\
	"menucmd=menu\0"\
        "boot_args="\
        "if test ${defroot} = root_mmc;then "\
        	"echo use mmcboot;"\
        	"setenv bootargs console=${console},${baudrate} "\
        			"root="CONFIG_MMCROOT" rootwait rw;"\
        "else "\
        	"if test ${defroot} = root_nfs;then "\
    	    	"echo use nfsboot;"\
				"setenv bootargs console=${console},${baudrate}  "\
					"root=/dev/nfs nfsroot=${nfsserverip}:${nfs_dir},nolock,v3,tcp " \
					"rootwait=1 ip=${ipaddr}:${nfsserverip}:${gatewayip}:${netmask}::eth0:off ;"\
			"else "\
	        	"echo use nandboot;"\
        	"setenv bootargs console=${console},${baudrate} "\
        			"root=ubi0:rootfs rootfstype=ubifs ubi.mtd=5 ${mtdparts} rootwait rw "CONFIG_BOOTARGS_CMA_SIZE";"\
			"fi;"\
		"fi;\0"\
        "loadkernel_nand=nand read.e ${loadaddr} kernel ${kimagesize};\0"\
		"loaddtb_nand=nand read.e ${dtbloadaddr} dtb;\0"\
		"nandboot=run loadkernel_nand && run loaddtb_nand && bootz ${loadaddr} - ${dtbloadaddr}\0"\
		"loadkernel_mmc=fatload mmc ${mmcdev} ${loadaddr} ${kernelimgname}\0"\
		"loaddtb_mmc=fatload mmc ${mmcdev} ${dtbloadaddr} ${dtbname} \0"\
		"mmcboot=mmc rescan && run loadkernel_mmc && run loaddtb_mmc && bootz ${loadaddr} - ${dtbloadaddr}\0"\
		"autoboot= run boot_args;"\
		"if run nandboot; then "\
			"echo use nandboot;"\
		"else "\
			"echo use mmcboot;"\
			"if run mmcboot; then "\
				"echo use mmcboot;"\
			"fi;"\
		"fi;\0"\

#define CONFIG_EXTRA_ENV_SETTINGS \
    CONFIG_MFG_ENV_SETTINGS \
    CONFIG_TQ_ENV_SETTINGS \
    "mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
    "mmcpart=" __stringify(CONFIG_SYS_MMC_IMG_LOAD_PART) "\0" \
    "p_fb0=lcd_7pd\0" \
    "fdt_addr=0x83000000\0" \
        "fdt_high=0xffffffff\0"   \
    "bootdelay=1\0"   \
    "console=ttySAC0\0" \
    "bootcmd=run autoboot;\0"


/* Miscellaneous configurable options */
#define CONFIG_CMD_MEMTEST
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_MEMTEST_START + 0x8000000)

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

#define CONFIG_STACKSIZE		SZ_128K

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* MTD Supports */
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_CMD_MTDPARTS
#define MTDIDS_DEFAULT "nand0=gpmi-nand"
#define MTDPARTS_DEFAULT "mtdparts=gpmi-nand:4m(u-boot),2m(u-boot-env),4m(logo),10m(kernel),2m(dtb),-(rootfs)"
#define	CONFIG_FAT_WRITE
#define CFG_CMD_BMP
#define CFG_CMD_PING


#ifdef CONFIG_SYS_BOOT_QSPI
#define CONFIG_FSL_QSPI
#define CONFIG_ENV_IS_IN_SPI_FLASH
#elif defined CONFIG_SYS_BOOT_NAND
#define CONFIG_SYS_USE_NAND
#define CONFIG_ENV_IS_IN_NAND
#else
#define CONFIG_FSL_QSPI
#define CONFIG_ENV_IS_IN_MMC
#endif

#define VIDEO_PROGRESS

#define CONFIG_SYS_MMC_ENV_DEV		0   /*0 means USDHC1 ,1 means sdhc2*/
#define CONFIG_SYS_MMC_ENV_PART		0	/* user area */
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"  /* USDHC1 */

#define CONFIG_CMD_BMODE

#ifdef CONFIG_FSL_QSPI
#define CONFIG_QSPI_BASE		QSPI0_BASE_ADDR
#define CONFIG_QSPI_MEMMAP_BASE		QSPI0_AMBA_BASE

#define CONFIG_CMD_SF
#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SF_DEFAULT_BUS		0
#define CONFIG_SF_DEFAULT_CS		0
#define CONFIG_SF_DEFAULT_SPEED	40000000
#define CONFIG_SF_DEFAULT_MODE		SPI_MODE_0
#define CONFIG_SPI_FLASH_STMICRO
#endif

/* NAND stuff */
#ifdef CONFIG_SYS_USE_NAND
#define CONFIG_CMD_NAND
#define CONFIG_CMD_NAND_TRIMFFS

#define CONFIG_NAND

#define CONFIG_NAND_MXS
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		0x40000000
#define CONFIG_SYS_NAND_5_ADDR_CYCLE
#define CONFIG_SYS_NAND_ONFI_DETECTION

/* DMA stuff, needed for GPMI/MXS NAND support */
#define CONFIG_APBH_DMA
#define CONFIG_APBH_DMA_BURST
#define CONFIG_APBH_DMA_BURST8
#endif

#define CONFIG_ENV_SIZE			SZ_8K
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(12 * SZ_64K)
#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
#define CONFIG_ENV_OFFSET		(768 * 1024)
#define CONFIG_ENV_SECT_SIZE		(64 * 1024)
#define CONFIG_ENV_SPI_BUS		CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS		CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MODE		CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#elif defined(CONFIG_ENV_IS_IN_NAND)
	#undef CONFIG_ENV_SIZE
	#define CONFIG_ENV_OFFSET		SZ_4M
	#define CONFIG_ENV_SECT_SIZE	SZ_2M
	#define CONFIG_ENV_SIZE			CONFIG_ENV_SECT_SIZE
#endif


/* USB Configs */
#define CONFIG_CMD_USB
#ifdef CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2
#endif

#ifdef CONFIG_CMD_NET
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#define CONFIG_FEC_MXC
#define CONFIG_MII
#define CONFIG_FEC_ENET_DEV		0

#if (CONFIG_FEC_ENET_DEV == 0)
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR          0x2
#define CONFIG_FEC_XCV_TYPE             RMII
#elif (CONFIG_FEC_ENET_DEV == 1)
#define IMX_FEC_BASE			ENET2_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0x1
#define CONFIG_FEC_XCV_TYPE		RMII
#endif
#define CONFIG_ETHPRIME			"FEC"

#define CONFIG_PHYLIB
#define CONFIG_PHY_MICREL
#endif

#define CONFIG_IMX_THERMAL

#ifndef CONFIG_SPL_BUILD
#define CONFIG_VIDEO
#ifdef CONFIG_VIDEO
#define CONFIG_CFB_CONSOLE
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_SW_CURSOR
#define CONFIG_VGA_AS_SINGLE_DEVICE
#define CONFIG_SYS_CONSOLE_IS_IN_ENV
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_VIDEO_SKIP
#endif
#endif

#define CONFIG_MODULE_FUSE
#define CONFIG_OF_SYSTEM_SETUP

#define CPU_TYPE 	"IMX6UL"

#endif
