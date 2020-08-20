#/bin/sh
# to create vfat.img file
#PC run 
dd if=/dev/zero of=vfat.img bs=1M count=64
mkdir tmp_mount
losetup /dev/loop0 vfat.img
mkfs.vfat /dev/loop0
#add file to vfat.img 
#PC run
if [ 0 = 1 ];then
    mount /dev/loop0 tmp_mount
    touch tmp_mount/test.txt
    echo "hello world" > tmp_mount/test.txt
    umount /dev/loop0
fi

#test vfat.img 
#arm-board run
if [ 0 = 1 ];then
    insmod fs/configfs/configfs.ko
    insmod drivers/usb/gadget/libcomposite.ko
    insmod drivers/usb/gadget/function/usb_f_mass_storage.ko
    insmod drivers/usb/gadget/legacy/g_mass_storage.ko file=vfat.img
fi
