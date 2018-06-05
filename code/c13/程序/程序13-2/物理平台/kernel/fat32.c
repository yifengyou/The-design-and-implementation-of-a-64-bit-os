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

struct Disk_Partition_Table DPT;
struct FAT32_BootSector fat32_bootsector;
struct FAT32_FSInfo fat32_fsinfo;

unsigned long FirstDataSector = 0;
unsigned long BytesPerClus = 0;
unsigned long FirstFAT1Sector = 0;
unsigned long FirstFAT2Sector = 0;

struct FAT32_Directory * lookup(char * name,int namelen,struct FAT32_Directory *dentry,int flags)
{
	unsigned int cluster = 0;
	unsigned long sector = 0;
	unsigned char *buf =NULL; 
	int i = 0,j = 0,x = 0;
	struct FAT32_Directory *tmpdentry = NULL;
	struct FAT32_LongDirectory *tmpldentry = NULL;
	struct FAT32_Directory *p = NULL;

	buf = kmalloc(BytesPerClus,0);

	cluster = (dentry->DIR_FstClusHI << 16 | dentry->DIR_FstClusLO) & 0x0fffffff;
next_cluster:
	sector = FirstDataSector + (cluster - 2) * fat32_bootsector.BPB_SecPerClus;
	color_printk(BLUE,BLACK,"lookup cluster:%#010x,sector:%#018lx\n",cluster,sector);
	if(!IDE_device_operation.transfer(ATA_READ_CMD,sector,fat32_bootsector.BPB_SecPerClus,(unsigned char *)buf))
	{
		color_printk(RED,BLACK,"FAT32 FS(lookup) read disk ERROR!!!!!!!!!!\n");
		kfree(buf);
		return NULL;
	}

	tmpdentry = (struct FAT32_Directory *)buf;

	for(i = 0;i < BytesPerClus;i+= 32,tmpdentry++)
	{
		if(tmpdentry->DIR_Attr == ATTR_LONG_NAME)
			continue;
		if(tmpdentry->DIR_Name[0] == 0xe5 || tmpdentry->DIR_Name[0] == 0x00 || tmpdentry->DIR_Name[0] == 0x05)
			continue;

		tmpldentry = (struct FAT32_LongDirectory *)tmpdentry-1;
		j = 0;

		//long file/dir name compare
		while(tmpldentry->LDIR_Attr == ATTR_LONG_NAME && tmpldentry->LDIR_Ord != 0xe5)
		{
			for(x=0;x<5;x++)
			{
				if(j>namelen && tmpldentry->LDIR_Name1[x] == 0xffff)
					continue;
				else if(j>namelen || tmpldentry->LDIR_Name1[x] != (unsigned short)(name[j++]))
					goto continue_cmp_fail;
			}
			for(x=0;x<6;x++)
			{
				if(j>namelen && tmpldentry->LDIR_Name2[x] == 0xffff)
					continue;
				else if(j>namelen || tmpldentry->LDIR_Name2[x] != (unsigned short)(name[j++]))
					goto continue_cmp_fail;
			}
			for(x=0;x<2;x++)
			{
				if(j>namelen && tmpldentry->LDIR_Name3[x] == 0xffff)
					continue;
				else if(j>namelen || tmpldentry->LDIR_Name3[x] != (unsigned short)(name[j++]))
					goto continue_cmp_fail;
			}

			if(j>=namelen)
			{
				p = (struct FAT32_Directory *)kmalloc(sizeof(struct FAT32_Directory),0);
				*p = *tmpdentry;
				kfree(buf);
				return p;
			}

			tmpldentry --;
		}

		//short file/dir base name compare
		j = 0;
		for(x=0;x<8;x++)
		{
			switch(tmpdentry->DIR_Name[x])
			{
				case ' ':
					if(!(tmpdentry->DIR_Attr & ATTR_DIRECTORY))
					{
						if(name[j]=='.')
							continue;
						else if(tmpdentry->DIR_Name[x] == name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					}
					else
					{
						if(j < namelen && tmpdentry->DIR_Name[x] == name[j])
						{
							j++;
							break;
						}
						else if(j == namelen)
							continue;
						else
							goto continue_cmp_fail;
					}

				case 'A' ... 'Z':
				case 'a' ... 'z':
					if(tmpdentry->DIR_NTRes & LOWERCASE_BASE)
						if(j < namelen && tmpdentry->DIR_Name[x] + 32 == name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					else
					{
						if(j < namelen && tmpdentry->DIR_Name[x] == name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;
					}

				case '0' ... '9':
					if(j < namelen && tmpdentry->DIR_Name[x] == name[j])
					{
						j++;
						break;
					}
					else
						goto continue_cmp_fail;

				default :
					j++;
					break;
			}
		}
		//short file ext name compare
		if(!(tmpdentry->DIR_Attr & ATTR_DIRECTORY))
		{
			j++;
			for(x=8;x<11;x++)
			{
				switch(tmpdentry->DIR_Name[x])
				{
					case 'A' ... 'Z':
					case 'a' ... 'z':
						if(tmpdentry->DIR_NTRes & LOWERCASE_EXT)
							if(tmpdentry->DIR_Name[x] + 32 == name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						else
						{
							if(tmpdentry->DIR_Name[x] == name[j])
							{
								j++;
								break;
							}
							else
								goto continue_cmp_fail;
						}

					case '0' ... '9':
						if(tmpdentry->DIR_Name[x] == name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;

					case ' ':
						if(tmpdentry->DIR_Name[x] == name[j])
						{
							j++;
							break;
						}
						else
							goto continue_cmp_fail;

					default :
						goto continue_cmp_fail;
				}
			}
		}
		p = (struct FAT32_Directory *)kmalloc(sizeof(struct FAT32_Directory),0);
		*p = *tmpdentry;
		kfree(buf);
		return p;

continue_cmp_fail:;
	}
	
	cluster = DISK1_FAT32_read_FAT_Entry(cluster);
	if(cluster < 0x0ffffff7)
		goto next_cluster;

	kfree(buf);
	return NULL;
}

struct FAT32_Directory * path_walk(char * name,unsigned long flags)
{
	char * tmpname = NULL;
	int tmpnamelen = 0;
	struct FAT32_Directory *parent = NULL;
	struct FAT32_Directory *path = NULL;
	char * dentryname = NULL;

	while(*name == '/')
		name++;

	if(!*name)
		return NULL;

	parent = (struct FAT32_Directory *)kmalloc(sizeof(struct FAT32_Directory),0);
	dentryname = kmalloc(PAGE_4K_SIZE,0);
	memset(parent,0,sizeof(struct FAT32_Directory));
	memset(dentryname,0,PAGE_4K_SIZE);
	parent->DIR_FstClusLO = fat32_bootsector.BPB_RootClus & 0xffff;
	parent->DIR_FstClusHI = (fat32_bootsector.BPB_RootClus >> 16) & 0x0fff;

	for(;;)
	{
		tmpname = name;
		while(*name && (*name != '/'))
			name++;
		tmpnamelen = name - tmpname;
		memcpy(tmpname,dentryname,tmpnamelen);
		dentryname[tmpnamelen] = '\0';

		path = lookup(dentryname,tmpnamelen,parent,flags);
		if(path == NULL)
		{
			color_printk(RED,WHITE,"can not find file or dir:%s\n",dentryname);
			kfree(dentryname);
			kfree(parent);
			return NULL;
		}

		if(!*name)
			goto last_component;
		while(*name == '/')
			name++;
		if(!*name)
			goto last_slash;

		*parent = *path;
		kfree(path);
	}

last_slash:
last_component:
	if(flags & 1)
	{
		kfree(dentryname);
		kfree(path);
		return parent;
	}

	kfree(dentryname);
	kfree(parent);
	return path;

}

unsigned int DISK1_FAT32_read_FAT_Entry(unsigned int fat_entry)
{
	unsigned int buf[128];
	memset(buf,0,512);
	IDE_device_operation.transfer(ATA_READ_CMD,FirstFAT1Sector + (fat_entry >> 7),1,(unsigned char *)buf);
	color_printk(BLUE,BLACK,"DISK1_FAT32_read_FAT_Entry fat_entry:%#018lx,%#010x\n",fat_entry,buf[fat_entry & 0x7f]);
	return buf[fat_entry & 0x7f] & 0x0fffffff;
}

unsigned long DISK1_FAT32_write_FAT_Entry(unsigned int fat_entry,unsigned int value)
{
	unsigned int buf[128];
	memset(buf,0,512);
	IDE_device_operation.transfer(ATA_READ_CMD,FirstFAT1Sector + (fat_entry >> 7),1,(unsigned char *)buf);
	buf[fat_entry & 0x7f] = (buf[fat_entry & 0x7f] & 0xf0000000) | (value & 0x0fffffff);
	IDE_device_operation.transfer(ATA_WRITE_CMD,FirstFAT1Sector + (fat_entry >> 7),1,(unsigned char *)buf);
	IDE_device_operation.transfer(ATA_WRITE_CMD,FirstFAT2Sector + (fat_entry >> 7),1,(unsigned char *)buf);
	return 1;	
}


void DISK1_FAT32_FS_init()
{
	int i;
	unsigned char buf[512];
	struct FAT32_Directory * dentry = NULL;
	
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

	FirstDataSector = DPT.DPTE[0].start_LBA + fat32_bootsector.BPB_RsvdSecCnt + fat32_bootsector.BPB_FATSz32 * fat32_bootsector.BPB_NumFATs;
	FirstFAT1Sector = DPT.DPTE[0].start_LBA + fat32_bootsector.BPB_RsvdSecCnt;
	FirstFAT2Sector = FirstFAT1Sector + fat32_bootsector.BPB_FATSz32;
	BytesPerClus = fat32_bootsector.BPB_SecPerClus * fat32_bootsector.BPB_BytesPerSec;


	dentry = path_walk("/JIOL123Llliwos/89AIOlejk.TXT",0);
	if(dentry != NULL)
		color_printk(BLUE,BLACK,"Find 89AIOlejk.TXT\nDIR_FstClusHI:%#018lx\tDIR_FstClusLO:%#018lx\tDIR_FileSize:%#018lx\n",dentry->DIR_FstClusHI,dentry->DIR_FstClusLO,dentry->DIR_FileSize);
	else
		color_printk(BLUE,BLACK,"Can`t find file\n");
}



