#ifndef	MY_HEAD_H
#define MY_HEAD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	MAXLENGTH	64

//本来是init的时候才用的常量，因为其他地方涉及到。被迫迁移
#define	INODE_SIZE	64

typedef	struct ext2_super_block	S_block;
typedef struct ext2_group_desc	G_desc;
typedef struct ext2_inode	inode;
typedef struct ext2_dir_entry	entry;

S_block*	ESB;
G_desc*		EGD;
int		uid;
int		gid;
inode*		cur_inode;
int		cur_ino;
int		Root;	//是否是root目录

#endif
