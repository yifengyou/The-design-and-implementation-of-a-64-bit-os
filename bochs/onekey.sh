#!/bin/bash 
if [ -e bochs-2.6.8 ];then
	/usr/bin/rm -rf bochs-2.6.8
	echo "============================================================"
	echo -e "\e[32m\e[1m\e[1m clean env \e[0m"
	echo "============================================================"
fi

if [ -e bochs-2.6.8.tar.gz ];then
	echo "============================================================"
	echo -e "\e[32m\e[1m\e[1m find bochs-2.6.8.tar.bz2,unpacking it... \e[0m"
	echo "============================================================"
	tar -xvf bochs-2.6.8.tar.gz 
else
	echo "============================================================"
	echo -e "\e[31m\e[1m\e[1m error!no bochs-2.6.8.tar.bz2 found!\e[0m"
	echo "============================================================"
	exit 1
fi


cd bochs-2.6.8 && ./configure --with-x11 --with-wx --enable-debugger --enable-disasm \
                --enable-all-optimizations --enable-readline --enable-long-phy-address \
                --enable-ltdl-install --enable-idle-hack --enable-plugins --enable-a20-pin \
                --enable-x86-64 --enable-smp --enable-cpu-level=6 --enable-large-ramfile \
                --enable-repeat-speedups --enable-fast-function-calls   \
                --enable-handlers-chaining  --enable-trace-linking \
                --enable-configurable-msrs --enable-show-ips --enable-cpp \
                --enable-debugger-gui --enable-iodebug --enable-logging \
                --enable-assert-checks --enable-fpu --enable-vmx=2 --enable-svm \
                --enable-3dnow --enable-alignment-check  --enable-monitor-mwait \
                --enable-avx  --enable-evex --enable-x86-debugger --enable-pci \
                --enable-usb --enable-usb-ohci --enable-usb-ehci   --enable-usb-xhci  \
                --enable-voodoo 

if [ $? -ne 0 ];then 
	echo "============================================================"
	echo -e "\e[31m\e[1m\e[1m configure error!please checkout it.\e[0m"
	echo "============================================================"
	exit 1
else
	echo "============================================================"
	echo -e "\e[32m\e[1m\e[1m configure bochs-2.6.8 success!!compiling it \e[0m"
	echo "============================================================"

fi

cp misc/bximage.cpp misc/bximage.cc
cp iodev/hdimage/hdimage.cpp iodev/hdimage/hdimage.cc           
cp iodev/hdimage/vmware3.cpp iodev/hdimage/vmware3.cc
cp iodev/hdimage/vmware4.cpp iodev/hdimage/vmware4.cc
cp iodev/hdimage/vpc-img.cpp iodev/hdimage/vpc-img.cc
cp iodev/hdimage/vbox.cpp iodev/hdimage/vbox.cc

make  && make install 

if [ $? -eq 0 ];then
	echo "============================================================"
	echo -e "\e[32m\e[1m\e[1m successful installed bochs-2.6.8! \e[0m"
	echo "============================================================"
	cd .. && /usr/bin/rm -rf bochs-2.6.8
	echo -e "\e[32m\e[1m\e[1m clean dir bochs-2.6.8"
	echo "============================================================"
	echo -e "BIN dir:/usr/local/bin/    ELF name:bochs"
	echo -e "lib/plugin dir:/usr/local/lib/bochs"
	echo -e "bios/keymaps dir:/usr/local/share/bochs"
	echo -e "doc dir:/usr/local/share/doc/bochs \e[0m"
	echo "============================================================"
else
	echo "============================================================"
	echo -e "\e[31m\e[1m\e[1m something error!please checkout it.\e[0m"
	echo "============================================================"
fi



