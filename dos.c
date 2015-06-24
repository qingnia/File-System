#include "other.h"
#include "my_head.h"
#include "init.h"

#include "my_head.h"

#define ARGS_LEN	5	
#define	MAX_ARGS_COUNT	32
inode*	 cur_inode;

int main(int argc, char* argv[])
{
	init();
	start();
	while(1)
	{
		printf("fs_dos>>");
		char input[MAXLENGTH];
		int  i = 0;
		scanf("%s", input);
		char* token = NULL;
		char** args = (char**)malloc(ARGS_LEN * sizeof(char*));
		for(; i < ARGS_LEN; i++)
		{
			args[i] = (char*)malloc(MAX_ARGS_COUNT * sizeof(char));
			memset(args + i, 0, MAX_ARGS_COUNT);
		}
		token = strtok(input, ",");
		i = 0;
		args[i] = token;
		token = strtok(NULL, ".");
		while(token)
		{
			i++;
			token = strtok(token, ",");
			args[i] = token;
			token = strtok(NULL, ".");
		}

		if(!strncmp(args[0], "quit", 4))
			return 0;
		if(!strncmp(args[0], "mkdir", 5))
		{
			printf("%s\n", args[0]);
			if(3 != i)
			{
				printf("error: Argument is wrong!\n");
				continue;
			}
			ext2_mkdir(args[1], atoi(args[2]), atoi(args[3]));
		}
		if(!strncmp(args[0], "ls", 2))
		{
			printf("%8s   %4s   %4s   %6s\n", "name", "type", "leng", "inode");
			ext2_lsdir();
		}
		if(!strncmp(args[0], "cd", 2))
		{
			char	str[10] = { 0 };
			printf("dir name: ");
			scanf("%s", str);
			ext2_cddir(str);
		}
		if(!strncmp(args[0], "rmdir", 5))
		{
			if(1 != i)
			{
				printf("Argument is wrong!\n");
				continue;
			}
			ext2_rmdir(args[1]);
		}
		if(!strncmp(args[0], "create", 6))
		{
			if(3 != i)
			{
				printf("Argument is wrong!\n");
				continue;
			}
			ext2_file_create(args[1], atoi(args[2]), atoi(args[3]));
		}
		if(!strncmp(args[0], "delete", 6))
		{
			if(1 != i)
			{
				printf("Argument is wrong!\n");
				continue;
			}
			ext2_file_delete(args[1]);
		}
		if(!strncmp(args[0], "write", 5))
		{
			if(2 != i)
			{
				printf("Argument is wrong!\n");
				continue;
			}
			ext2_file_write(args[1], args[2], MAXLENGTH);
		}
		if(!strncmp(args[0], "read", 4))
		{
			if(1 != i)
			{
				printf("Argument is wrong!\n");
				continue;
			}
			char buf[MAXLENGTH] = { 0 };
			ext2_file_read(args[1], buf, MAXLENGTH);
		}
		memset(input, 0, strlen(input));
	}
	return 0;
}
