#ifndef _FS_H_
#define _FS_H_

#include "my_head.h"

#define file_undefine	0
#define file_common	1
#define file_dir	2
#define EXT2_N_BLOCKS	8
#define EXT2_NAME_LEN	26

struct ext2_super_block {
	unsigned long  s_inodes_count;	/* Inodes count */
	unsigned long  s_blocks_count;	/* Blocks count */
	unsigned long  s_r_blocks_count;/* Reserved blocks count */
	unsigned long  s_free_blocks_count;/* Free blocks count */
	unsigned long  s_free_inodes_count;/* Free inodes count */
	unsigned long  s_first_data_block;/* First Data Block */
	unsigned long  s_log_block_size;/* Block size */
	long           s_log_frag_size;	/* Fragment size */
	unsigned long  s_blocks_per_group;/* # Blocks per group */
	unsigned long  s_frags_per_group;/* # Fragments per group */
	unsigned long  s_inodes_per_group;/* # Inodes per group */
	unsigned long  s_mtime;		/* Mount time */
	unsigned long  s_wtime;		/* Write time */
	unsigned short s_mnt_count;	/* Mount count */
	short          s_max_mnt_count;	/* Maximal mount count */
	unsigned short s_magic;		/* Magic signature */
	unsigned short s_state;		/* File system state */
	unsigned short s_errors;	/* Behaviour when detecting errors */
	unsigned short s_pad;
	unsigned long  s_lastcheck;	/* time of last check */
	unsigned long  s_checkinterval;	/* max. time between checks */
	unsigned long  s_reserved[238];	/* Padding to the end of the block */
};

struct ext2_inode {
	unsigned short i_mode;		/* 鏂囦欢绫诲瀷鍜岃闂潈闄?*/
	unsigned short i_uid;		/* 鎷ユ湁鐫€鏍囪瘑绗?*/
	unsigned long  i_size;		/* Size in bytes */
	unsigned long  i_atime;		/* Access time */
	unsigned long  i_ctime;		/* Creation time */
	unsigned long  i_mtime;		/* Modification time */
	unsigned long  i_dtime;		/* Deletion Time */
	unsigned short i_gid;		/* Group Id */
	unsigned short i_links_count;	/* Links count */
	unsigned long  i_blocks;	/* Blocks count */
	unsigned long  i_flags;		/* File flags */
	unsigned long  i_reserved1;
	unsigned long  i_block[EXT2_N_BLOCKS];/* 鏁版嵁搴撴寚閽堜竴鍏?5涓紝12鐩存帴绱㈠紩锛屼笁涓棿鎺ョ储寮?*/
	unsigned long  i_version;	/* File version (for NFS) */
	unsigned long  i_file_acl;	/* File ACL */
	unsigned long  i_dir_acl;	/* Directory ACL */
	unsigned long  i_faddr;		/* Fragment address */
	unsigned char  i_frag;		/* Fragment number */
	unsigned char  i_fsize;		/* Fragment size */
	unsigned short i_pad1;
	unsigned long  i_reserved2[2];
};

struct ext2_group_desc
{
   long   bg_block_bitmap;     /* 组中块位图所在的块号 */
   long   bg_inode_bitmap;        /* 组中索引节点位图所在块的块号 */
   long   bg_inode_table;     /*组中索引节点表的首块号 */
   long   bg_free_blocks_count;  /* 组中空闲块数 */
   long   bg_free_inodes_count;  /* 组中空闲索引节点数 */
   long   bg_used_dirs_count; /*组中分配给目录的节点数 */
   long   bg_pad;                   /*填充，对齐到字*/
   int    bg_reserved[3];            /*用null填充12个字节*/
   char	  noUse[952];
};
// 鐩綍淇℃伅鐩稿叧
struct ext2_dir_entry {
	unsigned long  inode;			/* Inode number */
	unsigned short rec_len;			/* Directory entry length */
	unsigned short name_len;		/* Name length */
	unsigned short file_type;		/* 鏂囦欢绫诲瀷 */
	char           name[EXT2_NAME_LEN];	/* File name */
};

//鏂囦欢淇℃伅鎯冲叧
//typedef unsigned short mode_t;
//typedef long eoff_t;
//typedef unsigned short dev_t;
//struct file {
//	mode_t f_mode;
//	dev_t f_rdev;			/* needed for /dev/tty */
//	eoff_t f_pos;
//	unsigned short f_flags;
//	unsigned short f_count;
//	unsigned short f_reada;
//	struct file *f_next, *f_prev;
//	struct ext2_inode * f_inode;
//};

//鐩綍閬嶅巻
// 
/*

// 鎿嶄綔
int ext2_mkdir (struct  ext2_inode* dir, const char * name, int len, int mode);
int ext2_rmdir (struct ext2_inode * dir, const char * name, int len);

static int ext2_file_read (struct ext2_inode * inode,char * buf, int count);
static int ext2_file_write (struct ext2_inode * inode,char * buf, int count);
static int ext2_file_delete (struct ext2_inode * inode);

// 鍒濆鍖?int initFileSystem();
void cmd();
int logoutfs();
*/
#endif
