//ģ��mysql�ͻ��˳���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mysql.h"

int main()
{
	//mysql��ʼ��---mysql_init
	MYSQL *mysql = mysql_init(NULL);
	if(mysql==NULL)
	{
		printf("mysql_init error\n");
		return -1;
	}
	
	//�������ݿ�
	MYSQL *conn = mysql_real_connect(mysql, "localhost", "root", "1qaz@WSX#EDC", "lbh", 0, NULL, 0);
	if(conn==NULL)
	{
		printf("connect mysql db error, [%s]\n", mysql_error(mysql));
		return -1;
	}
	
	//��ȡ��ǰ����ʹ�õ��ַ���
	printf("before:[%s]\n", mysql_character_set_name(conn));
	
	//�����ַ���Ϊutf8��ʽ
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
	
	//����ѭ���ȴ��û�����sql��䲢ִ��sql���
	while(1)
	{
		//��ӡ��ʾ��
		write(STDOUT_FILENO, "mysql> ", strlen("mysql> "));
		
		//��ȡ�û�����
		memset(buf, 0x00, sizeof(buf));
		read(STDIN_FILENO, buf, sizeof(buf));
		
		//1-ȥ��ĩβ��;
		p = strrchr(buf, ';');
		if(p!=NULL)
		{
			*p = '\0';
		}
		
		//2-ȥ���س�
		if(buf[0]=='\n') 
		{
			continue;
		}
		
		//ȥ����ǰ��ļ����ո�
		for(i=0; i<strlen(buf); i++)
		{
			if(buf[i]!=' ')
			{
				break;
			}
		}
		n = strlen(buf);
		memmove(buf, buf+i, n-i+1); //+1��ʾ�࿽��һ��\0
		printf("[%s]\n", buf);
		
		//����������˳�: exit EXIT quit QUIT
		if(strncasecmp(buf, "exit", 4)==0 || strncasecmp(buf, "quit", 4)==0)
		{
			mysql_close(conn);
			exit(0);
		}
		
		//ִ��sql���
		ret = mysql_query(conn, buf);
		if(ret!=0)
		{
			printf("%s\n", mysql_error(conn));
			continue;
		}
		
		//���û�����Ĳ���select��ѯ
		if(strncasecmp(buf, "select", 6)!=0)
		{
			printf("Query OK, %ld row affected\n", mysql_affected_rows(conn));
			continue;
		}
		
		//������select��ѯ�����	
		//��ȡ�����
		results = mysql_store_result(conn);
		if(results==NULL)
		{
			printf("%s\n", mysql_error(conn));
			continue;
		}
		
		//��ȡ����
		num = mysql_num_fields(results);
		
		//��ȡ��ͷ---����
		fields = mysql_fetch_fields(results);
		if(fields==NULL)
		{
			printf("%s\n", mysql_error(conn));
			mysql_free_result(results);
			continue;
		}
		//��ӡ��ͷ
		printf("+----------+-----------+\n");
		for(i=0; i<num; i++)
		{
			printf("%s\t", fields[i].name);
		}
		printf("\n");
		printf("+----------+-----------+\n");
		
		//ѭ����ȡÿһ�м�¼
		while(row=mysql_fetch_row(results))
		{
			for(i=0; i<num; i++)
			{
				printf("%s\t", row[i]);
			}
			printf("\n");
		}
		printf("+----------+-----------+\n");
		
		//�ͷŽ����
		mysql_free_result(results);
	}
		
	//�ر����ݿ�����
	mysql_close(conn);
	
	return 0;
}
