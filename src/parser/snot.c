#line 1 "snot.rl"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#line 10 "snot.rl"
	

	
#line 12 "snot.c"
static const char _snot_actions[] = {
	0, 1, 0
};

static const char _snot_key_offsets[] = {
	0, 0, 2, 3
};

static const char _snot_trans_keys[] = {
	10, 13, 10, 10, 13, 0
};

static const char _snot_single_lengths[] = {
	0, 2, 1, 2
};

static const char _snot_range_lengths[] = {
	0, 0, 0, 0
};

static const char _snot_index_offsets[] = {
	0, 0, 3, 5
};

static const char _snot_indicies[] = {
	0, 2, 1, 0, 1, 0, 2, 1, 
	0
};

static const char _snot_trans_targs[] = {
	3, 0, 2
};

static const char _snot_trans_actions[] = {
	1, 0, 0
};

static const int snot_start = 1;
static const int snot_error = 0;

static const int snot_en_main = 1;

#line 13 "snot.rl"
	
	#define BUFSIZE 128
	
void scanner()
{
	static char buf[BUFSIZE];
	int cs, act, have = 0, line_no = 1;
	char *ts, *te = 0;
	int done = 0;

	
#line 67 "snot.c"
	{
	cs = snot_start;
	}
#line 24 "snot.rl"

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
			
		
#line 94 "snot.c"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _snot_trans_keys + _snot_key_offsets[cs];
	_trans = _snot_index_offsets[cs];

	_klen = _snot_single_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _snot_range_lengths[cs];
	if ( _klen > 0 ) {
		const char *_lower = _keys;
		const char *_mid;
		const char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += ((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	_trans = _snot_indicies[_trans];
	cs = _snot_trans_targs[_trans];

	if ( _snot_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _snot_actions + _snot_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 8 "snot.rl"
	{line_no++;}
	break;
#line 172 "snot.c"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	_out: {}
	}
#line 46 "snot.rl"

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