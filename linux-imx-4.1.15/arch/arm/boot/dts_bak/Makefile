ifeq ($(CONFIG_OF),y)

dtb-$(CONFIG_SOC_IMX6UL) += \
	tq-imx6ul.dtb \
	tq-imx6ul_net0_net1.dtb \
	tq-imx6ul_net0_uart.dtb
#	tq-imx6ul_pwm_uart.dtb \
	tq-imx6ul_net0_uart.dtb \
	tq-imx6ul_net1_pwm.dtb \

dtb-$(CONFIG_SOC_IMX6ULL) += \
	tq-imx6ull.dtb \
	tq-imx6ull_net0_net1.dtb \
	tq-imx6ull_net0_uart.dtb
endif

always		:= $(dtb-y)
clean-files	:= *.dtb
