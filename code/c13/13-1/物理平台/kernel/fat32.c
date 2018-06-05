/***************************************************
*		版权声明
*
*	本操作系统名为：MINE
*	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
*	只允许个人学习以及公开交流使用
*
*	代码最终所有权及解释权归田宇所有；
*
*	本模块作者：	田宇
*	EMail:		345538255@qq.com
*
*
***************************************************/

#include "fat32.h"
#include "disk.h"
#include "printk.h"
#include "lib.h"


void DISK1_FAT32_FS_init()
{
	int i;
	unsigned char buf[512];
	struct Disk_Partition_Table DPT;
	struct FAT32_BootSector fat32_bootsector;
	struct FAT32_FSInfo fat32_fsinfo;
	
	memset(buf,0,512);
	IDE_device_operation.transfer(ATA_READ_CMD,0x0,1,(unsigned char *)buf);
	DPT = *(struct Disk_Partition_Table *)buf;
//	for(i = 0 ;i < 512 ; i++)
//		color_printk(PURPLE,WHITE,"%02x",buf[i]);
	color_printk(BLUE,BLACK,"DPTE[0] start_LBA:%#018lx\ttype:%#018lx\n",DPT.DPTE[0].start_LBA,DPT.DPTE[0].type);

	memset(buf,0,512);
	IDE_device_operation.transfer(ATA_READ_CMD,DPT.DPTE[0].start_LBA,1,(unsigned char *)buf);
	fat32_bootsector = *(struct FAT32_BootSector *)buf;
//	for(i = 0 ;i < 512 ; i++)
//		color_printk(PURPLE,WHITE,"%02x",buf[i]);	
	color_printk(BLUE,BLACK,"FAT32 Boot Sector\n\tBPB_FSInfo:%#018lx\n\tBPB_BkBootSec:%#018lx\n\tBPB_TotSec32:%#018lx\n",fat32_bootsector.BPB_FSInfo,fat32_bootsector.BPB_BkBootSec,fat32_bootsector.BPB_TotSec32);
	
	memset(buf,0,512);
	IDE_device_operation.transfer(ATA_READ_CMD,DPT.DPTE[0].start_LBA + fat32_bootsector.BPB_FSInfo,1,(unsigned char *)buf);
	fat32_fsinfo = *(struct FAT32_FSInfo *)buf;
//	for(i = 0 ;i < 512 ; i++)
//		color_printk(PURPLE,WHITE,"%02x",buf[i]);	
	color_printk(BLUE,BLACK,"FAT32 FSInfo\n\tFSI_LeadSig:%#018lx\n\tFSI_StrucSig:%#018lx\n\tFSI_Free_Count:%#018lx\n",fat32_fsinfo.FSI_LeadSig,fat32_fsinfo.FSI_StrucSig,fat32_fsinfo.FSI_Free_Count);

}



