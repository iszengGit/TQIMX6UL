#/bin/sh

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

#rm zImage
rm tq-imx6ul*.dtb

#cp config_6ull .config &&
#make zImage -j8 &&
#cp arch/arm/boot/zImage ./zImage &&

#cp config_6ul .config
#make zImage -j8
#cp arch/arm/boot/zImage ./zImage

make dtbs  &&

#imx6ul dtb
#cp arch/arm/boot/dts/tq-imx6ul_net0_net1.dtb ./tq-imx6ul_net0_net1.dtb &&
cp arch/arm/boot/dts/tq-imx6ul_net0_uart.dtb ./tq-imx6ul_net0_uart.dtb 
#imx6ull dtb
#cp arch/arm/boot/dts/tq-imx6ull_net0_net1.dtb ./tq-imx6ull_net0_net1.dtb &&
#cp arch/arm/boot/dts/tq-imx6ull_net0_uart.dtb ./tq-imx6ull_net0_uart.dtb
