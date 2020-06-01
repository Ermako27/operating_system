/*
* �������� ���������, � ������� ������ � ������� ������������ 
* ���������� ����� ����������� �����.
*/

#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <unistd.h> //pipe
#include <string.h> //strlen


int main()
{
	int descr[2]; //���������� _������_ ������������ ������
	//[0] - ����� ��� ������, [1] - ����� ��� ������
	//������� ���������� �������� ����������� ����� ������
	if ( pipe(descr) == -1)
	{
		perror( "couldn't pipe." );
		exit(1);
	}

	int child = fork();
	if ( child == -1 )
	{
		perror( "couldn't fork." );
		exit(1);
	}
	if ( child == 0 )
	{
		//������������� ���
		close( descr[1] ); //������� ������ �� ������� � �����
		printf( "\x1b[93m- Child: successfully forked.\n" );
		printf( "\x1b[93m- Child: reading data...\n" );
		printf( "\x1b[0m\n" );
		
		char msg[64];
		memset( msg, 0, 64 );
		int i = 0;
		
		//��������������� ��������� �� ������������ ������ �� 1 �������
		while( read(descr[0], &(msg[i++]), 1) != '\0' ) 
			;
			
		printf( "\x1b[93m- Child: read <%s>", msg );
		printf( "\x1b[0m\n" );
	}
	else
	{
		//������������ ���
		close( descr[0] ); //������ ������ �� ������� �� ������
		printf( "\x1b[0m* Parent: input string to transmit:\n" );
		char msg[64];
		gets(msg);
		msg[63]='\0';
		write( descr[1], msg, strlen(msg) ); //������� ��������� � �����
		return 0;
	}
}