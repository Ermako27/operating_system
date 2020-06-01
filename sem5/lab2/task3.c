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
		printf( "\x1b[93m- Child (%d): successfully forked.", getpid() );
		printf( "\x1b[0m\n" );
		char msg[64];
		sprintf( msg, "> exec() called from process %d", getpid() );
		
		if ( execlp( "echo", "echo", msg, NULL ) == -1 )
		{
			perror( "couldn't exec." );
			exit(1);
		}
	}
	else
	{
		printf( "\x1b[0m* Parent: waiting for child to finish...\n" );
		wait();
		return 0;
	}
}