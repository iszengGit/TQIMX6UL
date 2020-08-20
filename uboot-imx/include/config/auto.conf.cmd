deps_config := \
	test/env/Kconfig \
	test/dm/Kconfig \
	test/Kconfig \
	lib/efi/Kconfig \
	lib/rsa/Kconfig \
	lib/dhry/Kconfig \
	lib/Kconfig \
	fs/cramfs/Kconfig \
	fs/ubifs/Kconfig \
	fs/jffs2/Kconfig \
	fs/fat/Kconfig \
	fs/reiserfs/Kconfig \
	fs/ext4/Kconfig \
	fs/Kconfig \
	drivers/watchdog/Kconfig \
	drivers/video/bridge/Kconfig \
	drivers/video/fonts/Kconfig \
	drivers/video/Kconfig \
	drivers/usb/emul/Kconfig \
	drivers/usb/musb-new/Kconfig \
	drivers/usb/host/Kconfig \
	drivers/usb/Kconfig \
	drivers/tpm/Kconfig \
	drivers/timer/Kconfig \
	drivers/thermal/Kconfig \
	drivers/spi/Kconfig \
	drivers/sound/Kconfig \
	drivers/serial/Kconfig \
	drivers/rtc/Kconfig \
	drivers/remoteproc/Kconfig \
	drivers/ram/Kconfig \
	drivers/pwm/Kconfig \
	drivers/power/regulator/Kconfig \
	drivers/power/pmic/Kconfig \
	drivers/power/Kconfig \
	drivers/pinctrl/uniphier/Kconfig \
	drivers/pinctrl/nxp/Kconfig \
	drivers/pinctrl/Kconfig \
	drivers/pcmcia/Kconfig \
	drivers/pci/Kconfig \
	drivers/net/Kconfig \
	drivers/mtd/spi/Kconfig \
	drivers/mtd/nand/Kconfig \
	drivers/mtd/Kconfig \
	drivers/mmc/Kconfig \
	drivers/misc/Kconfig \
	drivers/led/Kconfig \
	drivers/input/Kconfig \
	drivers/i2c/muxes/Kconfig \
	drivers/i2c/Kconfig \
	drivers/hwmon/Kconfig \
	drivers/gpio/Kconfig \
	drivers/dma/Kconfig \
	drivers/dfu/Kconfig \
	drivers/demo/Kconfig \
	drivers/crypto/fsl/Kconfig \
	drivers/crypto/Kconfig \
	drivers/cpu/Kconfig \
	drivers/clk/uniphier/Kconfig \
	drivers/clk/Kconfig \
	drivers/block/Kconfig \
	drivers/adc/Kconfig \
	drivers/core/Kconfig \
	drivers/Kconfig \
	net/Kconfig \
	dts/Kconfig \
	cmd/Kconfig \
	common/Kconfig \
	arch/arm/Kconfig.debug \
	board/EmbedSky/tq-imx6ul/Kconfig \
	board/EmbedSky/tq-imx6ull/Kconfig \
	arch/arm/cpu/armv7/mx6/Kconfig \
	arch/arm/Kconfig \
	arch/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(UBOOTVERSION)" "2016.03"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
