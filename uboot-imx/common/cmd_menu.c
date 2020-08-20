/*
 * (C) Copyright 2016
 * Richard, GuangDong Linux Center, <edaplayer@163.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <cli.h>
#include <tq_util.h>


#ifdef CONFIG_TQLICENSE
#define  randlen           	16
#define  uuidlen           	32
#define  serialnumlen    	64
#define  licenselen        	64
#endif


enum menu_index {
	MAIN_MENU,
	TFTP_MENU
};

typedef struct menu_info {
	const char shortcut;
	const char *help;
	const char *cmd;
	void (*func)(void);
	const struct menu_item *next_menu_item;
	int flag;
} menu_info_t;

typedef struct menu_item {
	const char *title;
	struct menu_info *menu;
	const int menu_arraysize;
} menu_item_t;

typedef struct params {
	const char *envname;
	const char *promt;
} params_t;

static const params_t const tftp_params[];
static void set_params(const params_t *params, int array_size);
static void set_tftp_params(void);
static menu_info_t tftp_menu_info[];
static menu_info_t main_menu_info[];
static menu_info_t params_menu_info[];
static menu_info_t lcdtype_menu_info[];
static const menu_item_t main_menu_item;
static const menu_item_t tftp_menu_item;
static const menu_item_t params_menu_item;
static const menu_item_t lcdtype_menu_item;
static const menu_item_t uart_mux_menu_item;
#ifdef CONFIG_TQLICENSE
static const menu_item_t encrypt_menu_item;
static menu_info_t encrypt_menu_info[];
#endif


extern char console_buffer[];
extern int menu_readline(const char *const prompt);

static int run_command_args(const char *msg, ...);

static const params_t tftp_params[] = {
	{"serverip",		"Enter the TFTP Server(PC) IP address:"},
	{"ipaddr", 			"Enter the IP address:"},
	{"netmask", 		"Enter the netmask address:"},
};

static const params_t nfs_params[] = {
	{"nfsserverip",	"Enter the NFS Server(PC) IP address:"},
	{"ipaddr", 		"Enter the IP address:"},
	{"gatewayip", 	"Enter the NFS Gateway IP address:"},
	{"netmask", 	"Enter the Net Mask:"},
	{"nfs_dir", 	"Enter NFS directory:"},
};

static const params_t imageName_params[] = {
	{"ubootimgname", 	"Enter the u-boot image name"},
	{"logoimgname",		"Enter the logo image name"},
	{"kernelimgname", 	"Enter the kernel image name"},
	{"dtbname", 		"Enter the dtb image name"},
	{"rootimgname_ubi", "Enter the ubi rootfs image name"},
	{"rootimgname_yaffs", "Enter the yaffs rootfs image name"},

};

static const params_t func_params[] = {
    {"ethuart",     "Select eth1(enter eth1) or uart7/8(enter uart)"},
    {"ethpwm",     "Select eth0(enter eth0) or pwm(enter pwm)"},
    {"i2cts",     "Select i2c(enter i2c) or touchscreen(enter ts)"},
};

static int set_args(const char *pmenu, const char *pargs)
{
	char param_buf[256], cmd_buf[256];
	char* pdesc = NULL;
	char gets[256] = "\0";

	pdesc = getenv(pargs);
	if (pdesc == NULL) {
		pdesc = "NULL";
	}
	sprintf(cmd_buf, "%s (%s)", pmenu, pdesc);
	printf("%s\n", cmd_buf);
	cli_readline(gets);
	if (strlen(console_buffer))
		strcpy(param_buf, console_buffer);
	else
		strcpy(param_buf, pdesc);
	run_command_args("setenv %s %s", pargs, param_buf);
	return 0;
}


static int run_command_args(const char *msg, ...)
{
	char buf[256] = "\0";
	int ret;
	va_list argptr;

	va_start(argptr, msg);
	vsprintf(buf + strlen(buf), msg, argptr);

	//printf("%s\n", buf);
	ret = run_command(buf, 0);
	va_end(argptr);
	return ret;
}

static void set_params(const params_t *params, int array_size)
{
	int i;
	for (i = 0; i < array_size; i++) {
		set_args(params[i].promt, params[i].envname);
	}
	printf("Save parameters?(y/n)\n");
	if (getc() == 'y' ) {
		printf("y");
		if (getc() == '\r') {
			printf("\n");
			run_command("saveenv", 0);
		}
		else
			puts("save aborted\n");
	}
	else {
		printf("Not Save it!!!\n");
	}
}


static void set_tftp_params(void)
{
	set_params(tftp_params, ARRAY_SIZE(tftp_params));
}

static void set_nfs_params(void)
{
	run_command_args("setenv defroot root_nfs", 0);
	set_params(nfs_params, ARRAY_SIZE(nfs_params));
}


static void set_downloadimage_params(void)
{
	set_params(imageName_params, ARRAY_SIZE(imageName_params));
}
static void set_func_params(void)
{
	set_params(func_params, ARRAY_SIZE(func_params));
}

void downlaod_uboot_sd(void)
{
	int ret;
 	run_command_args("mmc rescan;");
	ret = run_command_args("fatload mmc 0 ${kloadaddr} ${ubootimgname}");
	if(ret != 0)
		return ;
	downlaod_uboot();
	run_command_args("nand erase.part u-boot-env");
}

void downlaod_uboot_tftp(void)
{
	int ret;
	ret = run_command_args("tftp ${kloadaddr} ${ubootimgname};");
	if(ret != 0)
		return ;
	downlaod_uboot();
	run_command_args("nand erase.part u-boot-env");
}

char g_help_buf[256];
char* custom_help(const char*help,char* rtn_buf)
{
	char buf[256]={'\0'},env[32]={'\0'};

	char *start=NULL,*end=NULL;

	end = help;
	memset(g_help_buf, 0, 256);
	while(1)
	{
		start = strstr(end, "${");
		if (start!=NULL)
		{
			strncpy(buf, end, start-end); //cpy before '${'
			end = strstr(start,"}");
			start += 2; //del prefix ${
			strncpy(env, start, end-start);
			start = getenv(env);
			if(start == NULL)
				start="NULL";
			strcat(buf, start);
			end += 1;
			//strcat(buf, end+1); //del '}'
			strcat(g_help_buf, buf);
			memset(buf, 0, 256);
			memset(env, 0, 32);
		}
		if(start == NULL)
		{
			strcat(g_help_buf, end);
			break;
		}
	}
	if(rtn_buf!=NULL){
		strcpy(rtn_buf, g_help_buf);
	}
	return g_help_buf;


	start = strstr(help,"${");
	if (start != NULL){
		strncpy(buf, help, start-help);
		end = strstr(start,"}");
		start += 2; //del prefix ${
		strncpy(env, start, end-start);
		start = getenv(env);
		if(start == NULL)
			start="NULL";
		strcat(buf, start);
		strcat(buf, end+1); //del '}'
		if(rtn_buf == NULL){
			strcpy(g_help_buf, buf);
			start = g_help_buf;
		}
		else{
			start = strcpy(rtn_buf, buf);
			start = rtn_buf;
		}
	}
	else
		start = help;
	return start;
}
#include <asm/imx-common/video.h>
//define in mx6ullevk.c
extern size_t display_count;

void print_disp_info()
{	int i;
	char const *panel = getenv("p_fb0");

	for (i=0;i<display_count;++i){
		if(strcmp(panel, displays[i].mode.name)==0){
			break;
		}
	}
	if(i == display_count){
		printf("panel %s not found !\n", panel);
		return;
	}
	printf("DISP_IFO : %s [ %dx%d bpp=%d ]\n", panel, \
		displays[i].mode.xres, displays[i].mode.yres, displays[i].pixfmt);
}
void print_core_info(void){
	print_cpuinfo();
	show_board_info();
	print_dram_info();
	print_nand_info();
	print_disp_info();
}

int embedsky_update()
{
	int ret;
	if (run_command("mmc rescan", 0) != 0){
		printf("no mmc found! skip download\n");
		return -1;
	}
	ret = run_command("tq_ini EmbedSky.ini", 0);
	/*ret = run_command("tq_ini /images/EmbedSky.ini", 0);
	if(ret != 0){
		printf("/images/EmbedSky.ini not found ! try to use /EmbedSky.ini\n");
	}*/
	return ret;
}
/* common menu title and cmd */
#define DOWN_UBOOT_TITLE	"Download u-boot to Nand Flash       [${ubootimgname}]"
#define DOWN_KERNEL_TITLE	"Download Linux Kernel to Nand Flash [${kernelimgname}]"
#define DOWN_LOGO_TITLE		"Download Logo image to Nand Flash   [${logoimgname}] "
#define DOWN_UBIFS_TITLE	"Download UBIFS image to Nand Flash  [${rootimgname_ubi}]"
#define DOWN_YAFFS_TITLE	"Download YAFFS image to Nand Flash  [${rootimgname_yaffs}]"
#define DOWN_ALL_TITLE		"Download all (uboot,kernel,dtb,rootfs)"
#define BOOT_SYSTEM_TITLE	"Boot the system"
#define NAND_SCRUB_TITLE	"Format the Nand Flash"

/* tftp menu title and cmd */
#define DOWN_DTB_TITLE		"Download Device tree blob(*.dtb)    [${dtbname}]"
#define TEST_NET_TITLE		"Test network (Ping PC's IP)  [pc:${serverip},arm:${ipaddr}]"
#define SET_TFTP_TITLE		"Set TFTP parameters(serverip,ipaddr...)"
#define TEST_KERNEL_TITLE	"Test kernel Image (zImage)"
#define TEST_DTB_TITLE	    "Test dtb Image (*.dtb)"

/* tftp menu cmd */
#define CMD_DOWN_KERNEL_TFTP	"tftp ${cpaddr} ${kernelimgname} && nand erase.part kernel && nand write.e ${cpaddr} kernel ${filesize} && setenv kimagesize ${filesize}&&saveenv"
#define CMD_DOWN_LOGO_TFTP		"tftp ${splashimage} ${logoimgname} && nand erase.part logo && nand write.e ${splashimage} logo ${filesize} "
//root.ubifs
#define CMD_DOWN_UBIFS_TFTP		"tftp ${cpaddr} ${rootimgname_ubi} && nand erase.part rootfs &&"\
                               		" nand write.i ${cpaddr} rootfs ${filesize}; "
//root.img
//#define CMD_DOWN_UBIFS_TFTP	"tftp ${cpaddr} rootfs.img && nand erase.part rootfs && nand write.e ${cpaddr} rootfs ${filesize}; "

#define CMD_DOWN_YAFFS_TFTP		"tfyaffs ${cpaddr} ${rootimgname_yaffs} rootfs;"
#define CMD_DOWN_PROGRAM_TFTP	"echo DOWN_PROGRAM_TFTP: not implemented"
#define CMD_DOWN_DTB_TFTP		"nand erase.part dtb && tftp ${dtbloadaddr} ${dtbname} && nand write ${dtbloadaddr} dtb"
//#define CMD_TEST_KERNEL_TFTP	"run boot_args;nand read.i ${dtbloadaddr} dtb;tftp ${loadaddr} ${kernelimgname}; bootz ${loadaddr} - ${dtbloadaddr}"
#define CMD_TEST_KERNEL_TFTP	"run boot_args; mmc rescan; fatload mmc ${mmcdev} ${dtbloadaddr} ${dtbname}; tftp ${loadaddr} ${kernelimgname}; bootz ${loadaddr} - ${dtbloadaddr}"
#define CMD_TEST_DTB_TFTP	    "run boot_args; mmc rescan; fatload mmc ${mmcdev} ${loadaddr} ${kernelimgname}; tftp ${dtbloadaddr} ${dtbname}; bootz ${loadaddr} - ${dtbloadaddr}"
#define CMD_TEST_KERNEL_DTB_TFTP "run boot_args; tftp ${loadaddr} ${kernelimgname}; tftp ${dtbloadaddr} ${dtbname}; bootz ${loadaddr} - ${dtbloadaddr}"
#define CMD_NAND_SCRUB			"nand scrub.chip"
#define CMD_TEST_NET			"ping ${serverip}"

/* main menu title */
#define SET_PARAMS_TITLE		"Set the boot parameters"
#define ENTER_TFTP_MENU_TITLE	"Enter TFTP download mode menu"
#define ENTER_ENCRYPT_MENU_TITLE  "Enter encrypt mode menu"    //zhz2016


/* main menu cmd */
#define CMD_DOWN_KERNEL_SD	"mmc rescan && fatload mmc 0 ${cpaddr} ${kernelimgname} && nand erase.part kernel &&  nand write.e ${cpaddr} kernel ${filesize} && setenv kimagesize ${filesize}&&saveenv"
#define CMD_DOWN_DTB_SD		"mmc rescan && fatload mmc 0 ${dtbloadaddr} ${dtbname} && nand erase.part dtb && nand write.e ${dtbloadaddr} dtb ${filesize}"
#define CMD_DOWN_LOGO_SD	"mmc rescan && fatload mmc 0 ${cpaddr} ${logoimgname} && nand erase.part logo &&  nand write.e ${cpaddr} logo ${filesize}"
#define CMD_DOWN_UBIFS_SD 	"mmc rescan && fatload mmc 0 ${loadaddr} ${rootimgname_ubi} && nand erase.part rootfs &&"\
                                " nand write.i ${loadaddr} rootfs ${filesize}; "

//#define CMD_DOWN_UBIFS_SD	"mmc rescan;fatload mmc 0 ${cpaddr} ${rootimgname}; nand erase.part rootfs;" \
								"nand write.e ${cpaddr} rootfs ${filesize};"
#define CMD_DOWN_YAFFS_SD	"mmc rescan && fatload mmc 0 ${cpaddr} ${rootimgname} && " \
								"nand erase.part rootfs && nand write.yaffs2 ${cpaddr} rootfs ${filesize}"
#define CMD_DOWN_PROGRAM_SD	"echo DOWN_PROGRAM_SD: not implemented"

#define CMD_TEST_KERNEL_SD	"mmc rescan && fatload mmc 0 ${loadaddr} ${kernelimgname} &&  bootz ${loadaddr}"
#define CMD_TEST_KERNEL_DTB_SD	"run boot_args; mmc rescan && fatload mmc 0 ${loadaddr} ${kernelimgname} && fatload mmc 0 ${dtbloadaddr} ${dtbname}  &&  bootz ${loadaddr} - ${dtbloadaddr}"

/* params menu title and cmd */
#define CMD_YAFFS_PARAMS "setenv defroot root_yaffs"
#define CMD_UBIFS_PARAMS "setenv defroot root_ubi"
#define CMD_MMC_PARAMS "setenv defroot root_mmc"

#define CONFIG_START_SYS "boot"

void auto_download_sd(){
//	run_command_args("nand scrub.chip");
	downlaod_uboot_sd();
	run_command_args(CMD_DOWN_LOGO_SD);
	run_command_args(CMD_DOWN_KERNEL_SD);
	run_command_args(CMD_DOWN_DTB_SD);
	run_command_args(CMD_DOWN_UBIFS_SD);
}
void auto_download_tftp(){
	downlaod_uboot_tftp();
	run_command_args(CMD_DOWN_LOGO_TFTP);
	run_command_args(CMD_DOWN_KERNEL_TFTP);
	run_command_args(CMD_DOWN_DTB_TFTP);
	run_command_args(CMD_DOWN_UBIFS_TFTP);
}

/****** tftp menu info *******/
static menu_info_t tftp_menu_info[] = {
    {'1', DOWN_UBOOT_TITLE, 	NULL, downlaod_uboot_tftp, NULL, 0},
    {'2', DOWN_LOGO_TITLE, 		CMD_DOWN_LOGO_TFTP, NULL, NULL, 0},
    {'3', DOWN_KERNEL_TITLE,	CMD_DOWN_KERNEL_TFTP, NULL, NULL, 0},
    {'4', DOWN_DTB_TITLE,		CMD_DOWN_DTB_TFTP, NULL, NULL},
    {'5', DOWN_UBIFS_TITLE,		CMD_DOWN_UBIFS_TFTP, NULL, NULL, 0},
	//{'6', DOWN_YAFFS_TITLE, 	CMD_DOWN_YAFFS_TFTP, NULL, NULL, 0},
    {'7', DOWN_ALL_TITLE,		NULL, auto_download_tftp, NULL,0},
    {'8', BOOT_SYSTEM_TITLE,	CONFIG_START_SYS, NULL, NULL},
    {'9', NAND_SCRUB_TITLE,		CMD_NAND_SCRUB, NULL, NULL},
	{'0', SET_PARAMS_TITLE,		NULL, NULL, &params_menu_item},
    {'p', TEST_NET_TITLE,		CMD_TEST_NET, NULL, NULL},
    {'r', "Restart u - boot", 		"reset", NULL, NULL},
    {'k', TEST_KERNEL_TITLE, 	CMD_TEST_KERNEL_TFTP, NULL, NULL},
    {'d', TEST_DTB_TITLE, 	CMD_TEST_DTB_TFTP, NULL, NULL},
    {'a', "test kernel and dtb", 	CMD_TEST_KERNEL_DTB_TFTP, NULL, NULL},
    {'q', "Return Main menu", 	NULL, NULL, NULL}
};


/****** set params menu *******/
static menu_info_t params_menu_info[] = {
    {'1', "Set NFS boot parameter", NULL, set_nfs_params, NULL},
    //{'2', "Set YAffS boot parameter Standard Linux", CMD_YAFFS_PARAMS, NULL, NULL},
    {'3', "Set UBIfs boot parameter Standard Linux",CMD_UBIFS_PARAMS, NULL, NULL},
    {'4', "Set MMC boot parameter Standard Linux",CMD_MMC_PARAMS, NULL, NULL},
    {'5', "Set Download Image name", 	NULL, set_downloadimage_params, NULL},
    {'6', SET_TFTP_TITLE,		NULL, set_tftp_params, NULL},
    {'s', "Save the parameters to Nand Flash" ,	"saveenv", NULL, NULL},
    {'c', "Choice lcd type" ,		NULL, NULL, &lcdtype_menu_item},
    {'i', "printf board infomation", NULL, print_core_info, NULL},
    {'e', "reset default environment variables" ,"nand erase.part u-boot-env ", NULL, NULL},
    {'r', "Restart u - boot", 			"reset", NULL, NULL},
    {'q', "Return Main menu", 		NULL, NULL, NULL}
};

/****** set lcdtype menu *******/
static menu_info_t lcdtype_menu_info[] = {
    {'1', "H50 5 inch(800x480) cap_tp screen.", 	"setenv p_fb0 lcd_5hd;", NULL, NULL },
    {'2', "TN92 7 inch(800x480) cap_tp/res_tp screen.", "setenv p_fb0 lcd_7pd;", NULL, NULL},
    {'3', "7HD 7 inch(1024x600) cap_tp screen.", "setenv p_fb0 lcd_7hd;", NULL, NULL},
    {'4', "T43 4.3 inch(480x272) res_tp screen.", "setenv p_fb0 lcd_4_3pd", NULL, NULL},
    {'5', "A104 10.4 inch(800x600) res_tp screen.", "setenv p_fb0 lcd_10_4pd", NULL, NULL},
    {'s', "Save the parameters to Nand Flash" ,	"saveenv", NULL, NULL},
    {'q', "Return to the previous menu", NULL, NULL, NULL}
};

/****** set uart mux menu *******/
static menu_info_t uart_mux_menu_info[] = {
    {'1', "switch func uart or eth0.", 	NULL, set_func_params, NULL},
    {'2', "switch func pwm or eth1.",   NULL, set_func_params, NULL},
    {'3', "switch func i2c or ts.",     NULL, set_func_params, NULL},
    {'r', "Restart u-boot",				"reset", NULL, NULL},
    {'q', "Return to the previous menu", NULL, NULL, NULL}
};


/****** main menu *******/
static menu_info_t main_menu_info[] = {
	{'1', DOWN_UBOOT_TITLE, 	NULL, downlaod_uboot_sd, NULL, 0},
	{'2', DOWN_LOGO_TITLE, 		CMD_DOWN_LOGO_SD, NULL, NULL, 0},
	{'3', DOWN_KERNEL_TITLE,	CMD_DOWN_KERNEL_SD, NULL, NULL, 0},
	{'4', DOWN_DTB_TITLE,		CMD_DOWN_DTB_SD, NULL, NULL},
	{'5', DOWN_UBIFS_TITLE,		CMD_DOWN_UBIFS_SD, NULL, NULL, 0},
	//{'6', DOWN_YAFFS_TITLE, 	CMD_DOWN_YAFFS_SD, NULL, NULL, 0},
	{'7', DOWN_ALL_TITLE,		NULL,auto_download_sd, NULL, 0},
	{'8', BOOT_SYSTEM_TITLE,	CONFIG_START_SYS, NULL, NULL},
	{'9', NAND_SCRUB_TITLE,		CMD_NAND_SCRUB, NULL, NULL},
	{'0', SET_PARAMS_TITLE,		NULL, NULL, &params_menu_item},
	{'n', ENTER_TFTP_MENU_TITLE, NULL, NULL, &tftp_menu_item},
	{'r', "Restart u-boot",		"reset", NULL, NULL},
    {'a', "using embedsky.ini to update system", NULL, embedsky_update, NULL},
	{'q', "Return console", NULL, NULL, NULL},
	{'x', "read uboot to sdcard", NULL, NULL, NULL}
};

static const menu_item_t main_menu_item = {
	"SDCARD MODE"
	, main_menu_info, ARRAY_SIZE(main_menu_info)
};

#ifdef CONFIG_TQLICENSE
static const menu_item_t encrypt_menu_item = {
	"Encrypt MODE"
	, encrypt_menu_info, ARRAY_SIZE(encrypt_menu_info)
};
#endif

static const menu_item_t tftp_menu_item = {
	"TFTP MODE"
	, tftp_menu_info, ARRAY_SIZE(tftp_menu_info)
};

static const menu_item_t params_menu_item = {
	"SET PARAMS"
	, params_menu_info, ARRAY_SIZE(params_menu_info)
};

static const menu_item_t lcdtype_menu_item = {
	"SET LCDTYPE"
	, lcdtype_menu_info, ARRAY_SIZE(lcdtype_menu_info)
};

static const menu_item_t uart_mux_menu_item = {
	"SET LCDTYPE"
	, uart_mux_menu_info, ARRAY_SIZE(uart_mux_menu_info)
};

extern int builtin_run_command(const char *cmd, int flag);


static int parse_menu(const menu_item_t *x, int array_size)
{
	int i;
	char key;
	menu_info_t *menu = x->menu;
	while (1) {
		if (x->title)
		{
			printf("\n#####   TQ-%s U-boot MENU    #####\n", CPU_TYPE);
			printf(  "            [%s]\n", x->title);
			printf(  "#######################################\n\n");
		}
        for(i = 0; i < array_size; i++)//显示当前菜单的所有子项信息
        {
        	printf("[%c] %s\n", menu[i].shortcut, custom_help(menu[i].help,NULL));
        }
		printf("Please press a key to continue :");
		key = getc();
		key = tolower(key);
		if (key == 'q')
		{
			printf("\n");
			return  0;
		}
		printf("%c\n",key);
		for (i = 0; i < array_size; i++)
		{
			if (key == menu[i].shortcut)
			{
				if (menu[i].next_menu_item != NULL)
					parse_menu(menu[i].next_menu_item, menu[i].next_menu_item->menu_arraysize);
				else if (menu[i].cmd != NULL) {
					//printf("%s\n", menu[i].cmd);
					//printf("menu[i].flag = 0x%x\n", menu[i].flag);
					//run_command_list(menu[i].cmd, -1, menu[i].flag);
					//if(menu[i].flag)
					//	lcd_printf_tar_start(menu[i].flag);//print file info on lcd
					run_command_args(menu[i].cmd);
					//builtin_run_command(menu[i].cmd, menu[i].flag);
				}
				else if (menu[i].func != NULL)
					menu[i].func();
				else
					printf("Nothing to do.\n");
				break;
			}
		}
	}
}

int do_menu(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	parse_menu(&main_menu_item, main_menu_item.menu_arraysize);
	return 0;
}

U_BOOT_CMD(
    menu, 1, 1, do_menu,
    "Terminal command menu",
    "show terminal menu"
);
