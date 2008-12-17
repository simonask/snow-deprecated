#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

%%{
	machine snot;
	newline = '\r'? '\n' @{line_no++;};
	main := newline+;
}%%	

	%% write data nofinal;
	
	#define BUFSIZE 128
	
void scanner()
{
	static char buf[BUFSIZE];
	int cs, act, have = 0, line_no = 1;
	char *ts, *te = 0;
	int done = 0;

	%% write init;

	while ( !done ) {
		char *p = buf + have, *pe, *eof = 0;
		int len, space = BUFSIZE - have;
		
		if ( space == 0 ) {
			/* We've used up the entire buffer storing an already-parsed token
			 * prefix that must be preserved. */
			fprintf(stderr, "OUT OF BUFFER SPACE\n" );
			exit(1);
		}

		len = fread( p, 1, space, stdin );
		pe = p + len;

		/* Check if this is the end of file. */
		if ( len < space ) {
			eof = pe;
			done = 1;
		}
			
		%% write exec;

		if ( cs == snot_error ) {
			fprintf(stderr, "PARSE ERROR\n" );
			break;
		}

		if ( ts == 0 )
			have = 0;
		else {
			/* There is a prefix to preserve, shift it over. */
			have = pe - ts;
			memmove( buf, ts, have );
			te = buf + (te-ts);
			ts = buf;
		}
	}
}
	
int main (int argc, char const *argv[])
{
	scanner();
	return 0;
}