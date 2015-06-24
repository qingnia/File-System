#include"my_head.h"

struct user
{
	char*	username;
	char*	password;
};

FILE* openXML(const char* filename, const char* type)
{
	return fopen(filename, type);
}

int XMLfind(FILE* xml, const char* key, char** password, int* uid, int* gid)
{
	char*	token = NULL;
	char	buf[MAXLENGTH];
	char	pes[MAXLENGTH];
	int 	tag = 0;
	while(fgets(buf, MAXLENGTH, xml))
	{
		token = strtok(buf, ">");
		if(strncmp(token, "\t\t<key", 4) != 0)
			continue;
		while((token = strtok(NULL, "<")) != NULL)
		{
			if(strcmp(token, key) == 0)
			{
				tag = 1;
				memset(buf, 0, strlen(buf));
				memset(token, 0, strlen(buf));
				fgets(buf, MAXLENGTH, xml);
				token = strtok(buf, ">");
				token = strtok(NULL, "<");
				strncpy(*password, token, strlen(token));
				memset(buf, 0, strlen(buf));
				fgets(buf, MAXLENGTH, xml);
				token = strtok(buf, ">");
				token = strtok(NULL, "<");
				*uid = atoi(token);
				memset(buf, 0, strlen(buf));
				fgets(buf, MAXLENGTH, xml);
				token = strtok(buf, ">");
				token = strtok(NULL, "<");
//				token = strtok(buf, MAXLENGTH, xml);//这一行什么意思已经看不懂了。怀疑是被不小心改了
				*gid = atoi(token);
				break;
			}
		}
		if(tag == 1)
		{
			return 0;
		}
		memset(buf, 0, strlen(buf));
	}
	printf("over\n");
	return 1;
}

int XMLclose(FILE* xml)
{
	return fclose(xml);
}
/*
int main(int argc, char* argv[])
{
	FILE* f = openXML("user.xml", "r");
	char* password = (char*)malloc(32 * sizeof(char));
	if(XMLfind(f, "qingniao", &password) != 0)
	{
		printf("error\n");
		return 1;
	}
	XMLclose(f);
	return 0;
}
*/
