//mysql获取结果集
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
	char sSQL[255] = "select * from mytest";
	int ret = mysql_query(conn, sSQL);
	if(ret!=0)
	{
		printf("mysql_query error, [%s]\n", mysql_error(mysql));
	}
	printf("mysql_query ok\n");

	//获取列数
	//unsigned int mysql_field_count(MYSQL *mysql) 	
	//unsigned int num = mysql_field_count(conn);

	//获取结果集
	//MYSQL_RES *mysql_store_result(MYSQL *mysql) 
	MYSQL_RES *results = mysql_store_result(conn);
	if(results==NULL)
	{
		printf("mysql_store_result error,[%s]\n", mysql_error(mysql));
		return -1;
	}
	printf("mysql_store_result ok\n");


	int i = 0;
	//获取列数
	//	unsigned int mysql_num_fields(MYSQL_RES *result) 
	unsigned int num = mysql_num_fields(results);

	//获取表头信息---列名
	//MYSQL_FIELD *mysql_fetch_fields(MYSQL_RES *result)
	MYSQL_FIELD *fields = mysql_fetch_fields(results);
	if(fields==NULL)
	{
		printf("mysql_fetch_fields error,[%s]\n", mysql_error(mysql));
		return -1;
	}
	//打印表头信息
	printf("+------+-----------+\n");
	for(i=0; i<num; i++)
	{
		printf("%s\t", fields[i].name);
	}
	printf("\n");
	printf("+------+-----------+\n");
	
	//获取结果集中每一行记录
	MYSQL_ROW row;
	while((row=mysql_fetch_row(results)))
	{
		for(i=0; i<num; i++)
		{
			printf("%s\t", row[i]);	
		}
		printf("\n");
	}
	printf("+------+-----------+\n");

	//释放结果集
	//void mysql_free_result(MYSQL_RES *result) 
	mysql_free_result(results);

	//关闭数据库连接
	mysql_close(conn);
	
	return 0;
}
