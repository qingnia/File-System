#include "oper_util.h"

#include "fs.h"

extern	S_block*	ESB;
extern	G_desc*		EGD;
extern	int		uid;
extern	int		gid;
extern	inode*		cur_inode;
extern	int		Root;
extern	int		cur_ino;

void	initInodeData(inode* i, int mode)
{
	i->i_mode = mode;
	i->i_uid = uid;
	i->i_gid = gid;
	i->i_size = INODE_SIZE;
	i->i_links_count = 1;
	i->i_blocks = 1;
	i->i_fsize = 0;
	i->i_faddr = 0;
	i->i_block[0] = getNullBlock();	
}

//设置目录
//这里的ino是指向文件的ino,不是当前目录的
void	setInodeDentry(int ino, const char* name, int len, int isRoot)
{
	entry*	e = (entry*)(EGD + cur_inode->i_block[0]);
	int i = 2;
	int j = 0;
//	if(Root && !isRoot)
//	{
//		i++;
		while(e[i].inode)
		{
			//每个块能放的目录项是有限的
			if(i < ESB->s_log_block_size / sizeof(entry))
				i++;
			else	
			{
				//全放满了就找下一个块
				//有点复杂，以后再添加。
				entry* e = (entry*)(EGD + cur_inode->i_block[1]);
				i = 0;
			}
		}
//	}
	e[i].inode = ino;
	e[i].rec_len = len;
	e[i].name_len = strlen(name);
	e[i].file_type = DIR_FILE;
	strncpy(e[i].name, name, EXT2_NAME_LEN);
}

void	initDirInodeDentryFor(int ino, inode* for_i, char* name, int len)
{
	int index = 0;
	if(strncmp(name, ".", 2))
		index = 1;
	entry*	e = (entry*)(EGD + for_i->i_block[0]);
	e[index].inode = ino;
	e[index].rec_len = len;
	e[index].name_len = strlen(name);
	e[index].file_type = DIR_FILE;
	strncpy(e[index].name, name, EXT2_NAME_LEN);
}

void	addFileSetESB()
{
	ESB->s_free_inodes_count--;
}

void	addFileSetG_desc(inode* i)
{
	EGD->bg_free_blocks_count -= i->i_blocks;
	EGD->bg_free_inodes_count--;
	EGD->bg_used_dirs_count += i->i_blocks;
}

void	addFileSetInodeBitMap(int ino)
{
	char*	table = (char*)(EGD + EGD->bg_inode_bitmap);
	char	c = table[ino / CHAR_SIZE];
	int	offset = ino % CHAR_SIZE;
	table[ino / CHAR_SIZE] = c | (0x01 << offset);
}
//查找文件用，先找到ino再找到inode
int	getInodeByNo(int ino, inode** i)
{
	if(!i)
	{
		fprintf(stderr, "getInodePoint wrong!\n");
		return 1;
	}
	int	dir_off = ino / ESB->s_inodes_per_group;
	int	offset = ino % ESB->s_inodes_per_group;
	inode*	node = (inode*)(EGD + dir_off + EGD->bg_inode_table);
	*i = node + offset;
	return 0;
}
//查找文件用
int	getInodeByName(const char* name)
{
	entry* e = (entry*)(EGD + cur_inode->i_block[0]);
//	while(NULL != e->name)
	int	i = 0;
	for(; i < ESB->s_log_block_size / sizeof(entry); i++)
	{
		if(!e->inode && i > 2)
		{
			e++;
			continue;
		}
		if(!strncmp(e->name, name, EXT2_NAME_LEN))
			return e->inode;
		e++;
	}
	printf("not find\n");
	return -1;
}
//添加文件用
int getNullInode()
{
	char* table = (char*)(EGD + EGD->bg_inode_bitmap);
	int i, j;
	int tag = 0;
	//以字符为单元
	for(i = 0; i < ESB->s_inodes_count / CHAR_SIZE; i++)
	{
		char test = 0x01;
		for(j = CHAR_SIZE - 1; j >= 0; j--)
		{
			//此处原来是while，感觉逻辑有错，改成if
			//每次判断一个位
			if(table[i] & test)
				test << 1;
			else
			{
				//忘了其他地方有没设置InodeBitMap的。只能在这里先搞一下
				table[i] = table[i] | test;
				tag = 1;
				break;
			}
		}
		if(tag)
		{
			int ret = i * CHAR_SIZE + j;
			return ret;
		}
	}
	return -1;
}

int	getNullBlock()
{
	char*	table = (char*)(EGD + EGD->bg_block_bitmap);
	int	i, j;
	int	tag = 0;
	for(i = 0; i < ESB->s_blocks_count / CHAR_SIZE; i++)
	{
		char	test = 0x01;
		for(j = CHAR_SIZE - 1; j >= 0; j--)
		{
			if(table[i] & test)
				test = test << 1;
			else
			{
				table[i] = table[i] | test;
//				printf("table[%d] : %x\n", i, table[i]);
				tag = 1;
				break;
			}
		}
		if(tag)
			return CHAR_SIZE * i + j;
	}
	return -1;
}

int	delFileSetInodeData(int ino, inode* i)
{
	i->i_links_count--;
	if(i->i_links_count)
		return 1;
	else
		return 0;
}

void	delFileSetDentry(int del_ino)
{
	entry* e = (entry*)(EGD + cur_inode->i_block[0]);
	int i = 0;
	for(; i < ESB->s_log_block_size / sizeof(entry); i++)
	{
		if(e->inode == del_ino)
		{
			e->inode = 0;
			break;
		}
		else
			e++;
	}
}

void	delDirFileSetDentry(int del_ino)
{
	//此处应该写个递归遍历。以后再说。。。
	entry*	e = (entry*)(EGD + cur_inode->i_block[0]);
	int	i = 0;
	for(; i < ESB->s_log_block_size / sizeof(entry); i++)
	{
		if(e->inode == del_ino)
		{
			e->inode = 0;
			break;
		}
		else
			e++;
	}
}

void	delFileSetESB()
{
	//如果是目录的话就不一样了
	ESB->s_free_inodes_count++;
}

void	delFileSetG_desc(inode* i)
{
	EGD->bg_free_blocks_count += i->i_blocks;
	EGD->bg_free_inodes_count++;
	EGD->bg_used_dirs_count--;
}

void	delFileSetInodeBitMap(int ino)
{
	int	offset = ino % CHAR_SIZE;
	char*	table = (char*)(EGD + EGD->bg_inode_bitmap);
	char	c = table[ino / CHAR_SIZE];
	table[ino / CHAR_SIZE] = c & (0xff - (0x80 >> offset));
}
