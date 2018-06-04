#!/bin/bash

echo -e "\033[31m=== env check  ===\033[0m"
if [ ! -e bochsrc.disk ];then
    echo "no bochsrc.disk,please checkout!"
    exit 1
fi

if [ -e boot.img ];then
    echo "find boot.img !"
else
    echo "no boot.img! generating..."
    echo -e  "fd\n\nboot.img\n" | bximage
fi

if [ ! -e  /usr/share/bochs/keymaps/x11-pc-us.map ];then
    echo "/usr/share/bochs/keymaps/x11-pc-us.map does not exist..."
    exit 1
else
    file /usr/share/bochs/keymaps/x11-pc-us.map
fi

if [ ! -e  /usr/share/bochs/BIOS-bochs-latest ];then
    echo " /usr/share/bochs/BIOS-bochs-latest does not exist..."
    exit 1
else
    file /usr/share/bochs/BIOS-bochs-latest
fi

if [ ! -e  /usr/share/vgabios/vgabios.bin ];then
    echo "/usr/share/vgabios/vgabios.bin does not exist..."
    exit 1
else
    file /usr/share/vgabios/vgabios.bin
fi
echo -e "\033[31m=== gen boot.bin ===\033[0m"
nasm boot.asm -o boot.bin
echo -e "\033[31m=== write boot.bin  to boot.img ===\033[0m"
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
echo -e "\033[31m=== running..PS:emulator will stop at beinging,press 'c' to running ===\033[0m"
/usr/bin/bochs -f bochsrc.disk
