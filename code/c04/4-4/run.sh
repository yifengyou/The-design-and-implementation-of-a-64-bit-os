#!/bin/bash

RED_COLOR='\E[1;31m'
RESET='\E[0m'

echo -e "${RED_COLOR}=== env check  ===${RESET}"

if [ ! -e bochsrc.floppy ];then
    echo "no bochsrc.floppy,please checkout!"
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
    echo -e  "1\nfd\n\nboot.img\n" | bximage
fi

echo -e "${RED_COLOR}=== gen boot.bin ===${RESET}"
cd bootloader

make 

cd ..

echo -e "${RED_COLOR}=== gen kernel.bin ===${RESET}"


cd kernel

make 

cd ..


echo -e "${RED_COLOR}=== write boot.bin  to boot.img ===${RESET}"

dd if=./bootloader/boot.bin of=boot.img bs=512 count=1 conv=notrunc

echo -e "${RED_COLOR}=== write kernel.bin  to boot.img  FAT12 ===${RESET}"

if [ ! -e tmp ];then
             mkdir tmp
fi

mount -t vfat -o loop boot.img tmp/

rm -rf tmp/*

cp kernel/kernel.bin tmp/
cp bootloader/loader.bin tmp/

sync

umount tmp/

rmdir tmp

echo -e "${RED_COLOR}=== running..PS:emulator will stop at beinging,press 'c' to running ===${RESET}"

if [ -e /usr/local/bin/bochs ];then
    /usr/local/bin/bochs -qf bochsrc.floppy
else
    echo "Please check your bochs environment!!!"
    exit 1
fi

