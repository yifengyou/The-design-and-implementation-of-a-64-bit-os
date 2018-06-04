# 第3章-BootLoader引导启动程序

1. Bootloader是由Boot和loader两部分构成。

> Boot负责开机启动和loader程序的加载。
> loader负责完成硬件配置、引导内核等任务

2. 检测启动设备的第一个扇区（0磁道0磁头1扇区），检验结尾是否为0xaa55(小端序)，如果是的话就断定该扇区位可引导扇区，加载到实模式1MB内存地址的0x7COO处（CS:0x000 IP:0x7COO）
