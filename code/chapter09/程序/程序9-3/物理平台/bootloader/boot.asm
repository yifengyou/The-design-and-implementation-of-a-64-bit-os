;/***************************************************
;		版权声明
;
;	本操作系统名为：MINE
;	该操作系统未经授权不得以盈利或非盈利为目的进行开发，
;	只允许个人学习以及公开交流使用
;	
;	代码最终所有权及解释权归田宇所有；
;
;	本模块作者：	田宇
;	EMail:		345538255@qq.com
;
;
;***************************************************/


;|----------------------|
;|	100000 ~ END	|
;|	   KERNEL	|
;|----------------------|
;|	E0000 ~ 100000	|
;| Extended System BIOS |
;|----------------------|
;|	C0000 ~ Dffff	|
;|     Expansion Area   |
;|----------------------|
;|	A0000 ~ bffff	|
;|   Legacy Video Area  |
;|----------------------|
;|	9f000 ~ A0000	|
;|	 BIOS reserve	|
;|----------------------|
;|	90000 ~ 9f000	|
;|	 kernel tmpbuf	|
;|----------------------|
;|	10000 ~ 90000	|
;|	   LOADER	|
;|----------------------|
;|	8000 ~ 10000	|
;|	  VBE info	|
;|----------------------|
;|	7e00 ~ 8000	|
;|	  mem info	|
;|----------------------|
;|	7c00 ~ 7e00	|
;|	 MBR (BOOT)	|
;|----------------------|
;|	0000 ~ 7c00	|
;|	 BIOS Code	|
;|----------------------|

	org	0x7c00	

BaseOfStack	equ	0x7c00

BaseOfLoader	equ	0x1000
OffsetOfLoader	equ	0x00

RootDirSectors		equ	14
SectorNumOfRootDirStart	equ	25
SectorNumOfFAT1Start	equ	1
SectorBalance		equ	23

	jmp	short Label_Start
	nop
	BS_OEMName	db	'MINEboot'
	BPB_BytesPerSec	dw	0x200
	BPB_SecPerClus	db	0x8
	BPB_RsvdSecCnt	dw	0x1
	BPB_NumFATs	db	0x2
	BPB_RootEntCnt	dw	0xe0
	BPB_TotSec16	dw	0x7d82
	BPB_Media	db	0xf0
	BPB_FATSz16	dw	0xc
	BPB_SecPerTrk	dw	0x3f
	BPB_NumHeads	dw	0xff
	BPB_hiddSec	dd	0
	BPB_TotSec32	dd	0
	BS_DrvNum	db	0
	BS_Reserved1	db	0
	BS_BootSig	db	29h
	BS_VolID	dd	0
	BS_VolLab	db	'boot loader'
	BS_FileSysType	db	'FAT12   '

Label_Start:

	mov	ax,	cs
	mov	ds,	ax
	mov	es,	ax
	mov	ss,	ax
	mov	sp,	BaseOfStack

;=======	clear screen

	mov	ax,	0600h
	mov	bx,	0700h
	mov	cx,	0
	mov	dx,	0184fh
	int	10h

;=======	set focus

	mov	ax,	0200h
	mov	bx,	0000h
	mov	dx,	0000h
	int	10h

;=======	display on screen : Start Booting......

	mov	ax,	1301h
	mov	bx,	000fh
	mov	dx,	0000h
	mov	cx,	10
	mov	bp,	StartBootMessage
	int	10h

;=======	search loader.bin
	mov	word	[SectorNo],	SectorNumOfRootDirStart

Lable_Search_In_Root_Dir_Begin:

	cmp	word	[RootDirSizeForLoop],	0
	jz	Label_No_LoaderBin
	dec	word	[RootDirSizeForLoop]	
	mov	ax,	00h
	mov	es,	ax
	mov	bx,	8000h
	mov	ax,	[SectorNo]
	mov	cx,	1
	call	Func_ReadOneSector
	mov	si,	LoaderFileName
	mov	di,	8000h
	cld
	mov	dx,	10h
	
Label_Search_For_LoaderBin:

	cmp	dx,	0
	jz	Label_Goto_Next_Sector_In_Root_Dir
	dec	dx
	mov	cx,	11

Label_Cmp_FileName:

	cmp	cx,	0
	jz	Label_FileName_Found
	dec	cx
	lodsb	
	cmp	al,	byte	[es:di]
	jz	Label_Go_On
	jmp	Label_Different

Label_Go_On:
	
	inc	di
	jmp	Label_Cmp_FileName

Label_Different:

	and	di,	0ffe0h
	add	di,	20h
	mov	si,	LoaderFileName
	jmp	Label_Search_For_LoaderBin

Label_Goto_Next_Sector_In_Root_Dir:
	
	add	word	[SectorNo],	1
	jmp	Lable_Search_In_Root_Dir_Begin
	
;=======	display on screen : ERROR:No LOADER Found

Label_No_LoaderBin:

	mov	ax,	1301h
	mov	bx,	008ch
	mov	dx,	0100h
	mov	cx,	21
	push	ax
	mov	ax,	ds
	mov	es,	ax
	pop	ax
	mov	bp,	NoLoaderMessage
	int	10h
	jmp	$

;=======	found loader.bin name in root director struct

Label_FileName_Found:

	mov	cx,	[BPB_SecPerClus]
	and	di,	0ffe0h
	add	di,	01ah
	mov	ax,	word	[es:di]
	push	ax
	sub	ax,	2
	mul	cl

	mov	cx,	RootDirSectors
	add	cx,	ax
;	add	cx,	SectorBalance
	add	cx,	SectorNumOfRootDirStart
	mov	ax,	BaseOfLoader
	mov	es,	ax
	mov	bx,	OffsetOfLoader
	mov	ax,	cx
	
Label_Go_On_Loading_File:

	push	ax
	push	bx
	mov	ah,	0eh
	mov	al,	'.'
	mov	bx,	0fh
	int	10h
	pop	bx
	pop	ax

	mov	cx,	[BPB_SecPerClus]
	call	Func_ReadOneSector
	pop	ax
	call	Func_GetFATEntry
	cmp	ax,	0fffh
	jz	Label_File_Loaded
	push	ax

	mov	cx,	[BPB_SecPerClus]
	sub	ax,	2
	mul	cl

	mov	dx,	RootDirSectors
	add	ax,	dx
;	add	ax,	SectorBalance
	add	ax,	SectorNumOfRootDirStart

	add	bx,	0x1000	;add	bx,	[BPB_BytesPerSec]

	jmp	Label_Go_On_Loading_File

Label_File_Loaded:

	jmp	BaseOfLoader:OffsetOfLoader

;=======	read one sector from floppy

Func_ReadOneSector:

	push	dword	00h
	push	dword	eax
	push	word	es
	push	word	bx
	push	word	cx
	push	word	10h
	mov	ah,	42h	;read
	mov	dl,	00h
	mov	si,	sp
	int	13h
	add	sp,	10h
	ret

;=======	get FAT Entry

Func_GetFATEntry:

	push	es
	push	bx
	push	ax
	mov	ax,	00
	mov	es,	ax
	pop	ax
	mov	byte	[Odd],	0
	mov	bx,	3
	mul	bx
	mov	bx,	2
	div	bx
	cmp	dx,	0
	jz	Label_Even
	mov	byte	[Odd],	1

Label_Even:

	xor	dx,	dx
	mov	bx,	[BPB_BytesPerSec]
	div	bx
	push	dx
	mov	bx,	8000h
	add	ax,	SectorNumOfFAT1Start
	mov	cx,	2
	call	Func_ReadOneSector
	
	pop	dx
	add	bx,	dx
	mov	ax,	[es:bx]
	cmp	byte	[Odd],	1
	jnz	Label_Even_2
	shr	ax,	4

Label_Even_2:
	and	ax,	0fffh
	pop	bx
	pop	es
	ret

;=======	tmp variable

RootDirSizeForLoop	dw	RootDirSectors
SectorNo		dw	0
Odd			db	0

;=======	display messages

StartBootMessage:	db	"Start Boot"
NoLoaderMessage:	db	"ERROR:No LOADER Found"
LoaderFileName:		db	"LOADER  BIN",0

;=======	fill zero until whole sector

	times	510 - ($ - $$)	db	0
	dw	0xaa55

