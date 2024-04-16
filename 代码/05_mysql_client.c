//模拟mysql客户端程序
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mysql.h"

int main()
{
	//mysql初始化---mysql_init
	MYSQL *mysql = mysql_init(NULL);
	if(mysql==NULL)
	{
		printf("mysql_init error\n");
		return -1;
	}
	
	//连接数据库
	MYSQL *conn = mysql_real_connect(mysql, "localhost", "root", "1qaz@WSX#EDC", "lbh", 0, NULL, 0);
	if(conn==NULL)
	{
		printf("connect mysql db error, [%s]\n", mysql_error(mysql));
		return -1;
	}
	
	//获取当前进程使用的字符集
	printf("before:[%s]\n", mysql_character_set_name(conn));
	
	//设置字符集为utf8格式
	mysql_set_character_set(conn, "utf8");
	printf("after:[%s]\n", mysql_character_set_name(conn));

	int i;
	int n;
	int ret;
	int num;
	char *p;
	char buf[1024];
	MYSQL_RES *results;
	MYSQL_FIELD *fields;
	MYSQL_ROW row;
	
	//进入循环等待用户输入sql语句并执行sql语句
	while(1)
	{
		//打印提示符
		write(STDOUT_FILENO, "mysql> ", strlen("mysql> "));
		
		//读取用户输入
		memset(buf, 0x00, sizeof(buf));
		read(STDIN_FILENO, buf, sizeof(buf));
		
		//1-去掉末尾的;
		p = strrchr(buf, ';');
		if(p!=NULL)
		{
			*p = '\0';
		}
		
		//2-去掉回车
		if(buf[0]=='\n') 
		{
			continue;
		}
		
		//去掉最前面的几个空格
		for(i=0; i<strlen(buf); i++)
		{
			if(buf[i]!=' ')
			{
				break;
			}
		}
		n = strlen(buf);
		memmove(buf, buf+i, n-i+1); //+1表示多拷贝一个\0
		printf("[%s]\n", buf);
		
		//若输入的是退出: exit EXIT quit QUIT
		if(strncasecmp(buf, "exit", 4)==0 || strncasecmp(buf, "quit", 4)==0)
		{
			mysql_close(conn);
			exit(0);
		}
		
		//执行sql语句
		ret = mysql_query(conn, buf);
		if(ret!=0)
		{
			printf("%s\n", mysql_error(conn));
			continue;
		}
		
		//若用户输入的不是select查询
		if(strncasecmp(buf, "select", 6)!=0)
		{
			printf("Query OK, %ld row affected\n", mysql_affected_rows(conn));
			continue;
		}
		
		//下面是select查询的情况	
		//获取结果集
		results = mysql_store_result(conn);
		if(results==NULL)
		{
			printf("%s\n", mysql_error(conn));
			continue;
		}
		
		//获取列数
		num = mysql_num_fields(results);
		
		//获取表头---列名
		fields = mysql_fetch_fields(results);
		if(fields==NULL)
		{
			printf("%s\n", mysql_error(conn));
			mysql_free_result(results);
			continue;
		}
		//打印表头
		printf("+----------+-----------+\n");
		for(i=0; i<num; i++)
		{
			printf("%s\t", fields[i].name);
		}
		printf("\n");
		printf("+----------+-----------+\n");
		
		//循环获取每一行记录
		while(row=mysql_fetch_row(results))
		{
			for(i=0; i<num; i++)
			{
				printf("%s\t", row[i]);
			}
			printf("\n");
		}
		printf("+----------+-----------+\n");
		
		//释放结果集
		mysql_free_result(results);
	}
		
	//关闭数据库连接
	mysql_close(conn);
	
	return 0;
}
