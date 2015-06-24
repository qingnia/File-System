#ifndef	OPERATION_H
#define	OPERATION_H

#include "my_head.h"

int	ext2_mkdir(const char* name, int len, int mode);
int	ext2_rmdir(const char* name);
int	ext2_cddir(const char* name);
void	ext2_ls_dir();
int	ext2_file_create(const char* filename, int len, int mode);
int	ext2_file_read(const char* filename, char* buf, int count);
int	ext2_file_write(const char* filename, char* buf, int count);
int	ext2_file_delete(const char* filename);


#endif
