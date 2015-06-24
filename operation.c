#include "operation.h"

#include "oper_util.h"
#include "fs.h"

extern	S_block*	ESB;
extern	G_desc*		EGD;
extern	inode*		cur_inode;
extern	int		cur_ino;
extern	int		Root;

int	ext2_mkdir(const char* name, int len, int mode)
{
	//此处假设当前块组一定有足够空间。
	//否则应该继续查看其他块组情况
	int	ino = getNullInode(EGD);
	//假设inode只有一级指针
	inode* i = NULL;
	getInodeByNo(ino, &i);
	initInodeData(i, mode);
	setInodeDentry(ino, name, len, 0);
	initDirInodeDentryFor(ino, i, ".", len);
	initDirInodeDentryFor(cur_ino, i, "..", len);
	inode* test_i;
	getInodeByNo(cur_ino, &test_i);
	addFileSetESB();
	addFileSetG_desc(i);
	addFileSetInodeBitMap(ino);
	printf("mkdir %s success\n", name);
	return 0;
}

//这种方法不好。应该搜索entry
int	ext2_cddir(const char* name)
{
	int	dir_ino = getInodeByName(name);
	if(-1 == dir_ino)
	{
		printf("no such direction\n");
		return 0;
	}
	if(!dir_ino)
		Root = 1;
	else
		Root = 0;
	cur_ino = dir_ino;
	getInodeByNo(dir_ino, &cur_inode);
	return 0;
}

void	ext2_lsdir()
{
	entry*	e = (entry*)(EGD + cur_inode->i_block[0]);
	int i = 0;
	for(; i < ESB->s_log_block_size / sizeof(entry); i++)
	{
		if(e->inode || (Root && !i) || i < 2)
		{
			printf("%8s   %4d   %4d   %6ld\n", e->name, e->file_type, e->rec_len, e->inode);
		}
		e++;
	}
}

int ext2_rmdir(const char* name)
{
	//这里只把目录当成普通文件删除。会造成严重的存储泄漏
	int	del_ino = getInodeByName(name);
	if(-1 == del_ino)
	{
		printf("No suchFile\n");
		return -1;
	}
	inode* i = NULL;
	getInodeByNo(del_ino, &i);
	if(delFileSetInodeData(del_ino, i))
		return 1;
	delFileSetESB();
	delFileSetG_desc(i);
	delFileSetInodeBitMap(del_ino);
	delDirFileSetDentry(del_ino);
	return 0;
}

int	ext2_file_create(const char* filename, int len, int mode)
{
	inode*	i = NULL;
	int	ino = getNullInode();
	getInodeByNo(ino, &i);
	initInodeData(i, mode);
	setInodeDentry(ino, filename, len, 0);
	addFileSetESB();
	addFileSetG_desc(i);
	addFileSetInodeBitMap(ino);
	return ino;
}

int	ext2_file_read(const char* filename, char* buf, int count)
{
	inode*	i = NULL;
	int	ino = 0;
	if(-1 == (ino = getInodeByName(filename)))
	{
		printf("No such file!\n");
		return -1;
	}
	getInodeByNo(ino, &i);
	char*	data = (char*)(EGD + i->i_block[0]);
	int	addr = i->i_faddr;
	int	index = 0;
	while('\n' != data[addr] && data[addr + index])
	{
		buf[index] = data[addr + index];
		index++;
		if(index > count)
			break;
	}
	buf[index] = 0;
	i->i_faddr = addr + index;
	printf("%s\n", buf);	//输出看一下
	return 0;
}

int ext2_file_write(const char* filename, char* buf, int count)
{
	inode*	i = NULL;
	int	ino = 0;
	//默认w+，如果文件不存在则建一个新文件
	if(-1 == (ino = getInodeByName(filename)))
		ino = ext2_file_create(filename, 1024, 0777);
	getInodeByNo(ino, &i);
	char*	data = (char*)(EGD + i->i_block[0]);
	//默认不覆盖，向后写
	int	addr = i->i_fsize;
	int	index = 0;
	while(buf[index])
	{
		data[addr + index] = buf[index];
		index++;
		if(index >= count)
			break;
	}
	data[addr + index] = 0;
	i->i_fsize +=index;
	printf("block:%ld, str:%s\n", i->i_block[0], data);
}

int	ext2_file_delete(const char* filename)
{
	inode*	i = NULL;
	int	ino = 0;
	if(-1 == (ino = getInodeByName(filename)))
	{
		printf("No such File\n");
		return 1;
	}
	getInodeByNo(ino, &i);
	int ret = delFileSetInodeData(ino, i);
	if(ret)
		return 1;
	printf("ino:%d\n", ino);
	delFileSetESB();
	delFileSetG_desc(i);
	delFileSetInodeBitMap(ino);
	delFileSetDentry(ino);
	return 0;
}
