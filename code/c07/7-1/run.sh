#!/bin/bash

RED_COLOR='\E[1;31m'
RESET='\E[0m'

echo -e "${RED_COLOR}=== env check  ===${RESET}"

if [ ! -e bochsrc.udisk ];then
    echo "no bochsrc.udisk,please checkout!"
    exit 1
fi

if [ -e /usr/local/bin/bochs ];then
    echo "find /usr/local/bin/bochs!"
else
    echo "no bochs find!,Please check your bochs environment" 
fi

if [ -e /usr/local/bin/bximage ];then
    echo "find /usr/local/bin/bximage!"
else
    echo "no bximge find!,Please check your bochs environment" 
fi

if [ ! -e  /usr/local/share/bochs/BIOS-bochs-latest ];then
    echo " /usr/local/share/bochs/BIOS-bochs-latest does not exist..."
    exit 1
else
    file /usr/local/share/bochs/BIOS-bochs-latest
fi

if [ ! -e  /usr/local/share/bochs/VGABIOS-lgpl-latest ];then
    echo "/usr/local/share/bochs/VGABIOS-lgpl-latest does not exist..."
    exit 1
else
    file /usr/local/share/bochs/VGABIOS-lgpl-latest
fi


if [ -e boot.img ];then
    echo "find boot.img !"
else
    echo "no boot.img! generating..."
    echo -e  "1\nhd\nflat\n10\nboot.img\n" | bximage
fi

echo -e "${RED_COLOR}=== gen boot.bin ===${RESET}"

cd bootloader
make 
if [ $? -ne 0 ];then
    echo -e "${RED_COLOR}==bootloader make failed!Please checkout first!==${RESET}"
    exit 1
fi
cd ..

echo -e "${RED_COLOR}=== write boot.bin  to boot.img ===${RESET}"

dd if=./bootloader/boot.bin of=boot.img bs=512 count=1 conv=notrunc

echo -e "${RED_COLOR}=== write loader.bin to boot.img  FAT12 ===${RESET}"

if [ ! -e tmp ];then
             mkdir tmp
fi

mount -t vfat -o loop boot.img tmp/

rm -rf tmp/*

cp bootloader/loader.bin tmp/
if [ $? -ne 0 ];then
    echo -e "${RED_COLOR}copy loader.bin error!${RESET}"
    exit 1
fi

sync

umount tmp/

rmdir tmp

echo -e "${RED_COLOR}=== running..PS:emulator will stop at beinging,press 'c' to running ===${RESET}"

if [ -e /usr/local/bin/bochs ];then
    /usr/local/bin/bochs -qf bochsrc.udisk
else
    echo -e "${RED_COLOR}Please check your bochs environment!!!{RESET}"
    exit 1
fi

