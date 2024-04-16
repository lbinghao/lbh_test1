//mysql执行sql语句
#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"

int main()
{
	//初始化
	//MYSQL *mysql_init(MYSQL *mysql) 
	MYSQL *mysql = mysql_init(NULL);
	if(mysql==NULL)
	{
		printf("mysql init error\n");
		return -1;
	}
	printf("mysql init ok\n");

	//连接mysql数据库
	//MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag) 
	MYSQL *conn = mysql_real_connect(mysql, "192.168.10.145", "root", "123456", "scott", 0, NULL, 0);
	if(conn==NULL)
	{
		printf("mysql_real_connect error,[%s]\n", mysql_error(mysql));
		return -1;
	}
	printf("connect mysql OK, [%p], [%p]\n", mysql, conn);

	//执行sql语句
	//int mysql_query(MYSQL *mysql, const char *query) 
	char sSQL[255] = "insert into mytest values(1, 'xiaohuang')";
	int ret = mysql_query(conn, sSQL);
	if(ret!=0)
	{
		printf("mysql_query error, [%s]\n", mysql_error(mysql));
	}
	printf("mysql_query ok\n");

	//关闭数据库连接
	mysql_close(conn);
	
	return 0;
}
