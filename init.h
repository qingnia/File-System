#ifndef	INIT_H
#define INIT_H

#include "my_head.h"
#include "fs.h"

#define BLOCK_GROUP_COUNT	1
#define BLOCK_PER_GROUP		1024
#define	INODE_PER_GROUP		128
#define ROOT_UID		1
#define ROOT_GID		1
#define BLOCK_PER_INODE		8
#define DIR_FILE		2
#define GRE_FILE		1
#define	BLOCK_SIZE		1024

void	init();
void	init_super_block();
void	init_block_group();
void	init_block_BitMap();
void	init_Inode_BitMap();
void	init_Inode_Table();

//这两个函数因为在文件操作上要反复用，所以换到文件操作工据里
//void	initInodeData(inode* i, int mode);
//void	setInodeDentry(int ino, int* offset, const char* name, int len, int mode);

void	init_Root();

#endif
