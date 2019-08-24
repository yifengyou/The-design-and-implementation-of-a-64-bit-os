#!/bin/bash

# sudo yum install wget -y
# sudo wget -O /etc/yum.repos.d/CentOS-Base.repo https://mirrors.tencent.com/repo/centos7_base.repo
# sudo yum clean all
# sudo yum makecache

sudo yum install gtk2 gtk2-devel libXt libXt-devel libXpm libXpm-devel SDL SDL-devel xorg-x11-server-devel nasm gcc-c++ glibc-headers libX11-devel libXrandr-devel lrzsz -y

if [ $? -eq 0 ];then
	echo "====================================================="
	echo -e "\e[32m\e[1m安装成功!\e[0m"
	echo "====================================================="
else
	echo "====================================================="
	echo -e "\e[31m\e[1m\e[5m安装失败!请根据错误提示自行排查\e[0m"
	echo "====================================================="
fi
