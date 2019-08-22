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

#ifndef __FAT32_H__

#define __FAT32_H__

struct Disk_Partition_Table_Entry
{
	unsigned char flags;
	unsigned char start_head;
	unsigned short  start_sector	:6,	//0~5
			start_cylinder	:10;	//6~15
	unsigned char type;
	unsigned char end_head;
	unsigned short  end_sector	:6,	//0~5
			end_cylinder	:10;	//6~15
	unsigned int start_LBA;
	unsigned int sectors_limit;
	
}__attribute__((packed));

struct Disk_Partition_Table
{
	unsigned char BS_Reserved[446];
	struct Disk_Partition_Table_Entry DPTE[4];
	unsigned short BS_TrailSig;
}__attribute__((packed));

struct FAT32_BootSector
{
	unsigned char BS_jmpBoot[3];
	unsigned char BS_OEMName[8];
	unsigned short BPB_BytesPerSec;
	unsigned char BPB_SecPerClus;
	unsigned short BPB_RsvdSecCnt;
	unsigned char BPB_NumFATs;
	unsigned short BPB_RootEntCnt;
	unsigned short BPB_TotSec16;
	unsigned char BPB_Media;
	unsigned short BPB_FATSz16;
	unsigned short BPB_SecPerTrk;
	unsigned short BPB_NumHeads;
	unsigned int BPB_HiddSec;
	unsigned int BPB_TotSec32;
	
	unsigned int BPB_FATSz32;
	unsigned short BPB_ExtFlags;
	unsigned short BPB_FSVer;
	unsigned int BPB_RootClus;
	unsigned short BPB_FSInfo;
	unsigned short BPB_BkBootSec;
	unsigned char BPB_Reserved[12];

	unsigned char BS_DrvNum;
	unsigned char BS_Reserved1;
	unsigned char BS_BootSig;
	unsigned int BS_VolID;
	unsigned char BS_VolLab[11];
	unsigned char BS_FilSysType[8];

	unsigned char BootCode[420];

	unsigned short BS_TrailSig;
}__attribute__((packed));


struct FAT32_FSInfo
{
	unsigned int FSI_LeadSig;
	unsigned char FSI_Reserved1[480];
	unsigned int FSI_StrucSig;
	unsigned int FSI_Free_Count;
	unsigned int FSI_Nxt_Free;
	unsigned char FSI_Reserved2[12];
	unsigned int FSI_TrailSig;
}__attribute__((packed));

#define	ATTR_READ_ONLY	(1 << 0)
#define ATTR_HIDDEN	(1 << 1)
#define ATTR_SYSTEM	(1 << 2)
#define ATTR_VOLUME_ID	(1 << 3)
#define ATTR_DIRECTORY	(1 << 4)
#define ATTR_ARCHIVE	(1 << 5)
#define ATTR_LONG_NAME	(ATTR_READ_ONLY | ATTR_HIDDEN | ATTR_SYSTEM | ATTR_VOLUME_ID)

struct FAT32_Directory
{
	unsigned char DIR_Name[11];
	unsigned char DIR_Attr;
	unsigned char DIR_NTRes;
	unsigned char DIR_CrtTimeTenth;
	unsigned short DIR_CrtTime;	
	unsigned short DIR_CrtDate;
	unsigned short DIR_LastAccDate;
	unsigned short DIR_FstClusHI;
	unsigned short DIR_WrtTime;
	unsigned short DIR_WrtDate;
	unsigned short DIR_FstClusLO;
	unsigned int DIR_FileSize;
}__attribute__((packed));

#define LOWERCASE_BASE (8)
#define LOWERCASE_EXT (16)
struct FAT32_LongDirectory
{
	unsigned char LDIR_Ord;
	unsigned short LDIR_Name1[5];
	unsigned char LDIR_Attr;
	unsigned char LDIR_Type;
	unsigned char LDIR_Chksum;
	unsigned short LDIR_Name2[6];
	unsigned short LDIR_FstClusLO;
	unsigned short LDIR_Name3[2];
}__attribute__((packed));

void DISK1_FAT32_FS_init();

#endif
