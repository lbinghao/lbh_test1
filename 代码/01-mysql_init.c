//mysql初始化
#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"

int main()
{
	//MYSQL *mysql_init(MYSQL *mysql) 
	MYSQL *mysql = mysql_init(NULL);
	if(mysql==NULL)
	{
		printf("mysql init error\n");
		return -1;
	}
	printf("mysql init ok\n");

	return 0;
}
