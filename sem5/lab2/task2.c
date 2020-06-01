

#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <sys/types.h> //W***
#include <sys/wait.h> //wait
#include <unistd.h>



int main()
{
	int child = fork();
	if ( child == -1 )
	{
		perror("couldn't fork.");
		exit(1);
	}
	if ( child == 0 )
	{
		printf( "\x1b[93m- Child: successfully forked.\n");
		printf( "\x1b[93m- Child: pid=%d;	group=%d;	parent=%d\n", getpid(), getpgrp(), getppid() );
		printf( "\x1b[93m- Child: Press enter to kill process.\n" );
		getchar();
		int res = getpid() % 2;
		printf( "\x1b[93m- Child: returning %d.", res );
		printf( "\x1b[0m\n" );
		return res;
	}
	else
	{
		printf( "\x1b[0m* Parent: pid=%d;	group=%d;	child=%d\n", getpid(), getpgrp(), child );
		printf( "\x1b[0m* Parent: waiting for child to finish...\n" );
		int stat;
		pid_t res;
		res = wait( &stat );
		if ( WIFEXITED(stat) )
		{
			printf("\x1b[0m* Parent: child %d finished with %d code.\n", res, WEXITSTATUS(stat) );
		}
		else
			printf("\x1b[0m* Parent: child finished abnormally.\n" );
		return 0;
	}
}