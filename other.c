#include "other.h"
#include "xmlUtils.h"
#include "my_head.h"

extern	int	uid;
extern	int	gid;

int	login()
{
	char	name[MAXLENGTH], password[MAXLENGTH];
	char*	conpwd = (char*)malloc(MAXLENGTH * sizeof(char));
	memset(name, 0, MAXLENGTH * sizeof(char));
	memset(password, 0, MAXLENGTH * sizeof(char));
	memset(conpwd, 0, MAXLENGTH * sizeof(char));
	FILE* fd = openXML("user.xml", "r");
	printf("login: ");
	scanf("%s", name);
	//貌似这里没用循环。应该用循环检测
	if(XMLfind(fd, name, &conpwd, &uid, &gid))
	{
		printf("User is not found!\n");
		free(conpwd);
		return 1;
	}
	printf("password: ");
	scanf("%s", password);
	if(strcmp(password, conpwd))
	{
		printf("Password is wrong!\n");
		free(conpwd);
		return 2;
	}
	free(conpwd);
	return 0;
}

int	start()
{
	while(1)
	{
		printf("login>>");
		int	choice = 0, logStat = 0;
		char	name[MAXLENGTH], password[MAXLENGTH];
		printf("choose login or create user with 1 or 2: ");
		scanf("%d", &choice);
		memset(name, 0, MAXLENGTH * sizeof(char));
		memset(password, 0, MAXLENGTH * sizeof(char));
		switch(choice)
		{
			case 1:
				logStat = login();
				break;
			case 2:	//注册
				printf("username: ");
				scanf("%s", name);
				//2还没写，要检测name是否可用
				printf("%s", password);
				scanf("%s", password);
				break;
		}
		if(!logStat)
		{
			printf("login success!\n");
			break;
		}
	}
	return 0;
}
