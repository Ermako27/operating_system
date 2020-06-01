/*
* �������� ���������, � ������� ������ � ������� ������������ 
* ���������� ����� ����������� �����.
*/

#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <unistd.h> //pipe
#include <string.h> //strlen
#include <signal.h>
#include <unistd.h>


int child_flag = 0;

void child_sigint_catcher(int signum)
{
	child_flag = 1;
}

void parent_sigint_catcher(int signum)
{
	printf( "\x1b[0m* Parent: finished.\n" );
	exit(0);
}

int main()
{
	int child;
	int descr[2]; //���������� _������_ ������������ ������
	//[0] - ����� ��� ������, [1] - ����� ��� ������
	//������� ���������� �������� ����������� ����� ������
	if ( pipe(descr) == -1)
	{
		perror( "couldn't pipe." );
		exit(1);
	}

	child = fork();
	if ( child == -1 )
	{
		perror( "couldn't fork." );
		exit(1);
	}
	if ( child == 0 )
	{
		signal(SIGINT, child_sigint_catcher);
		
		close( descr[1] ); //������� ������ �� ������� � �����
		printf( "\x1b[93m- Child: successfully forked.\n" );
		printf( "\x1b[93m- Child: waiting for SIGINT to read...\n" );
		printf( "\x1b[0m\n" );
		
		sleep(3);

		if ( !child_flag )
		{
			printf( "\x1b[93m- Child: didn't get SIGINT, exiting." );
			printf( "\x1b[0m\n" );
			exit(0);
		}
		
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
		signal(SIGINT, parent_sigint_catcher);
		
		close( descr[0] ); //������ ������ �� ������� �� ������
		
		printf( "\x1b[0m* Parent: message sent.\n" );
		char msg[64] = "message from parent";
		write( descr[1], msg, strlen(msg) ); //������� ��������� � �����
		
		printf( "\x1b[0m* Parent: waiting for CTRL+C signal...\n" );
		while (1)
			;
		return 0;
	}
}