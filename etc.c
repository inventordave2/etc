// ETC_C

#ifndef __LOCAL__ETC__
#define __LOCAL__ETC__	

#include <stdlib.h>
#include <string.h>

char* sanitizeStr( char* );
char* sanitizeStr2( char*, char* invalid_chars_array );
char* getstring( char* );
char* int2str( int );
int str2int( char* );
int cmp_dstr( char*,char* );
int systemEndian( );

// SANITIZESTR: General method for pre-processing of an input c-string (safety).
// [char*] sanitizeStr( [char*] );
// In the switch/case block of this function is the self-evidence of how it works.
// Modify the switch/case template with the ASCII values of input-string characters
// you wish to strip from the string. The example function strips the symbols:
// \ " * / : < > ? |
// Just modify for your custom needs.
char* sanitizeStr( char* A )	{
	
	char * B = (char *)malloc( strlen(A)+1 );
	char * _ = B;
	
	while((*A) != '\0')	{
		
		switch(*A)	{

			case 92: // \ backslash
			case 34: // " double-quote
			case 42: // * star
			case 47: // / forward-slash
			case 58: // : colon
			case 60: // < lt
			case 62: // > gt
			case 63: // ? question-mark
			case 124:// | pipe

				// printf( "Invalid char ('%c') (%d).\n", *_, *_ );
				// Uncomment above line to see reporting to stdout of detected invalid chars in the input string.
				*B = '.'; // Arbitrary printable-char to replace invalid char with.
				break;
			
			default:
				*B = *A; 
				//printf( "char ok: '%c'\n", (*str) );
				break;	
		}
		
		++A;
		++B;
	}
	
	*B = '\0';
	
	return _;
}

// example invalid chars array:
// char* invalid_chars_array = "\\\"*/:<>?|";
// encodes:  \ " * / : < > ? |

char* sanitizeStr2( char* A, char* invalid_chars_array )	{

	char* ics = invalid_chars_array;
	char* B = (char*)calloc( sizeof(char), strlen(A)+1 );
	char* _ = B;
	int i;

	while((*A) != '\0')	{

		*B = *A;

		i=0;
		while( ics[i++]!='\0' )
			if( (*A)==ics[i] )
				*B = '.';

		++A;
		++B;
	}

	*B = '\0';

	return _;
}


// [char*] getstring( [char*] );
// This function prevents a lot of bugs and pointer-mangling etc.
// It simply duplicates ( just like strdup() )
#include <stdlib.h>
#include <string.h>
char* getstring( char* in ){
	
	int str_length = strlen( in );
	char* _ = (char*)malloc( str_length+1 );
	
	int i=0;
	for( ; i<str_length; )
		_[i++] = in[i];
	
	_[i] = '\0';
	
	return _;
}

// [char*] int2str( int );
// Function to convert an integer value to a cstring digit-string of numeric 0-9 numeric ASCII printables.
#include <stdlib.h>
#include <stdio.h>
char* int2str(int v)	{
	
	char* _ = (char*)malloc( (sizeof(int)*8)+1 );
	sprintf( _, "%d", v );
	
	return _;
}

// [int] str2int( [char*] );
// Implementation of non-standard atoi() string-to-int converter.
// Checks that the conversion wouldn't generate an INT value larger than System INT_MAX
// maximum value for int's.
#include <limits.h>
#include <string.h>
int str2int(char* input)	{

	char* t = int2str(INT_MAX);
	if( cmp_dstr( t, input )<0 )	{

		free( t );
		return 0; // The input string represents a number too large!
	}
	free( t );

	int len = strlen(input), i = 0, result = 0;

	if (input[0] == '-')
		i = 1;

	for(; i<len; i++)
		result = result * 10 + ( input[i] - '0' );

	if (input[0] == '-')
		result = 0 - result;
	
	return result;
}

// [int] cpm_dstr( [char*], [char*] );
// Function to test 2 cstrings that encode a decimal (base10) digitstring, eg, "41952" etc, and compares which of the 2 strings represents
// the higher value of the 2 decimal numbers.
// Returns:
// 	-1 if a < b
// 	0  if a == b
// +1  if a > b
#include <stdio.h>
#include <string.h>
int cmp_dstr( char* a_, char* b_ )	{

	char* a = a_;
	char* b = b_;
	
	if( a_ == NULL )	{
		
		printf( "Warning. Arg 'a' in cmp_dstr (\"%s\":%d) is a NULL ptr.\n", __FILE__, __LINE__ );
		a = "0";
	}
	if( b_ == NULL )	{
		
		printf( "Warning. Arg 'b' in cmp_dstr (\"%s\":%d) is a NULL ptr.\n", __FILE__, __LINE__ );
		b = "0";
	}
	
	fflush( stdout );
	
	int i = 0;
	
	while( a[i++]=='0' )
		++a;
	
	i = 0;
	while( b[i++]=='0' )
		++b;
	
	int len_a = strlen( a );
	int len_b = strlen( b );
	
	if( len_a<len_b )
		return -1;

	if( len_a>len_b )
		return +1;

	for( int test=0; test<len_a; test++ )
		if( a[test]>b[test] )
			return +1;
		else if( a[test]<b[test] )
			return -1;
	
	return 0;	
}

// [int] systemEndian( [void] );
// Function to test for big-endianness or little-endianness in the local machine.
// There are even simpler methods, but I wrote this one to help with your thinking.
// requires #include <stdio.h>.
#include <stdio.h>
int systemEndian()	{
	
	int s = sizeof( int );	
	int v = 0;
	
	int i;
	for( i=0; i<s; i++ )
		v += ( '1'+(i) ) << ( i*8 );

	for( i=0; i<s; i++ )
		// Uncomment the "printf" code line below before compiling the function and it will print a clue on invocation
		// as to how the function performs the test.

		//printf( "Byte at offset '%d' contains '%c'\n", i, ((char*)(void*)(&v))[i] );

		;

	int b = ((char*)(&v))[0] == ( '0' + s );
	if( b )
		//printf( "System is big-endian.\n" );
		;
	else
		//printf( "System is little-endian.\n" );
		;
	return ((char*)(&v))[0] == ( '1' ); // returns 1 if little-endian. returns 0 if big-endian.
}


#endif

