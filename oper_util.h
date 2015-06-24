#ifndef	OPER_UTIL_H
#define	OPER_UTIL_H

#include "my_head.h"

#define	DIR_FILE	1
#define	CHAR_SIZE	8

//这个函数是通过文件名获得entry和ino，但是和getInodeByName功能重合。所以舍弃
//int	getDentryByName(inode* i, const char* name, entry** ent);

void	initInodeData(inode* i, int mode);
//已经设置了全局变量cur_inode，当前文件块的便宜量就不需要作为参数传递。
//这样还能避免错误
//如果插入的是根节点的.文件，则特殊处理，不进行规范化查找空entry
void	setInodeDentry(int ino, const char* name, int len, int isRoot);
void	initDirInodeDentryFor(int ino, inode* for_i, char* name, int len);

int	getInodeByNo(int ino, inode** i);
int	getInodeByName(const char* name);
int	getNullInode();
int	getNullBlock();

void	addFileSetESB();
void	addFileSetG_desc(inode* i);
void	addFileSetInodeBitMap(int ino);

int	delFileSetInodeData(int ino, inode* i);
void	delFileSetDentry(int del_ino);
void	delDirFileSetDentry(int del_ino);
void	delFileESB();
void	delFileSetG_des(inode* i);
void	delFileSetInodeBitMap(int ino);

#endif
