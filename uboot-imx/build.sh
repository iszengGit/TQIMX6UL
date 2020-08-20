#!/bin/bash
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

cfgfile=include/configs/tq-imx6ull.h

#默认编译256M内存版本
if [ -z $1 ] ; then
    echo "build for default configuration. ddr=256M "
    sleep 3
    sed -i 's#SZ_512M#SZ_256M#g' ${cfgfile}
else
        if [ $1 = "DDR=512M" ] ; then
            echo "build for DDR=512M configuration. ddr=512M "
            sleep 3
            sed -i 's#SZ_256M#SZ_512M#g' ${cfgfile}

        elif [ $1 = "DDR=256M" ] ; then
            echo "build for DDR=256M configuration. ddr=256M "
            sleep 3
            sed -i 's#SZ_512M#SZ_256M#g' ${cfgfile}
        else
            echo "usage: ./build.sh DDR=512M/DDR=256M"
	    exit 1

        fi

fi
#rm u-boot-imx6ull.imx u-boot-imx6ul.imx
#make tq-imx6ull_defconfig &&
#make -j8 &&
#cp u-boot.imx u-boot-imx6ull.imx &&

make tq-imx6ul_defconfig &&
make -j8 &&
cp u-boot.imx u-boot-imx6ul.imx
