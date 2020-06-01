
#include <stdio.h> //printf
#include <stdlib.h> //exit
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
		printf( "\x1b[93m- Child: waiting...\n" );
		getchar();
		printf( "\x1b[93m\n- Child: parent=%d now.\n", getppid() );
		printf( "\x1b[93m- Child: exiting." );
		printf( "\x1b[0m\n" );
		return 0;
	}
	else
	{

		printf( "\x1b[0m* Parent: pid=%d;	group=%d;	child=%d\n", getpid(), getpgrp(), child );
		printf( "\x1b[0m* Parent: press enter to kill process.\n" );
		getchar();
		printf( "\x1b[0m* Parent: exiting. Press enter again to check child.\n" );
		return 0;
	}
}