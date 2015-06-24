#include "init.h"

extern	S_block*	ESB;
extern	G_desc*		EGD;
extern	int		uid;
extern	int		gid;
extern	inode*		cur_inode;
extern	int		cur_ino;
extern	int		Root;

/*文件系统初始化*/
//只有一个块组，超级块与块组分离，块组描述符在块组的第一块
void	init()
{
	init_super_block();
	init_block_group();
	init_block_BitMap();
	init_Inode_BitMap();
	init_Inode_Table();
	init_Root();
}

void	init_super_block()
{
	ESB = (S_block*)malloc(sizeof(S_block));
	ESB->s_inodes_count = INODE_PER_GROUP * BLOCK_GROUP_COUNT;
	ESB->s_blocks_count = BLOCK_PER_GROUP;
	ESB->s_r_blocks_count = 1; //?
	ESB->s_free_inodes_count = INODE_PER_GROUP * BLOCK_GROUP_COUNT - 1;
	ESB->s_log_block_size = BLOCK_SIZE;
	ESB->s_inodes_per_group = INODE_PER_GROUP;
}

void	init_block_group()
{
	//块组描述符在第0块
	EGD = (G_desc*)calloc(BLOCK_PER_GROUP, BLOCK_SIZE);
	EGD->bg_block_bitmap = 1;	//块位图在第0块
	EGD->bg_inode_bitmap = 2;	//inode位图
	EGD->bg_inode_table = 3;	//inode节点表
	EGD->bg_free_blocks_count = 1013;//1024-1个块是块位图-1个inode位图-8个节点表
	EGD->bg_free_inodes_count = INODE_PER_GROUP - 1;//自己用了一个
	EGD->bg_used_dirs_count = 1;	
}

void	init_block_BitMap()
{
	int	offset = EGD->bg_block_bitmap;
	char*	dir = (char*)(EGD + offset);
	//目前用的是前11块
	dir[0] = 0xff;
	dir[1] = 0xff;
}

void	init_Inode_BitMap()
{
	int	offset = EGD->bg_inode_bitmap;
	char*	dir = (char*)(EGD + offset);
	//只用了第1个inode
	//由于C的右移是算数由移，不能从左向右依次判1，只能约定从右向左
	dir[0] = 0x01;
}

void	init_Inode_Table()
{
	int	offset = EGD->bg_inode_table;
	inode*	dir = (inode*)(EGD + offset);
	dir->i_mode = 0600;
	dir->i_uid = ROOT_UID;
	dir->i_size = INODE_SIZE;
	dir->i_gid = ROOT_GID;
	dir->i_blocks = 1;	//初始化只给一个块，不够再加
}

/*初始化根节点*/ 
//不用左孩子树
void	init_Root()
{
	inode*	i = (inode*)(EGD + EGD->bg_inode_table);
	cur_inode = i;
	cur_ino = 0;
	Root = 1;
	initInodeData(i);
	initDirInodeDentryFor(0, i, ".", 1024);
	initDirInodeDentryFor(0, i, "..", 1024);
}
