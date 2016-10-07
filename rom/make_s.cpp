#include <stdio.h>
#include <map>
#include <stdlib.h>
#include <string>

using namespace std;

map< string, int > symbolmap;
map< int, string > symbolmaprev;

int main()
{
	FILE * st = fopen( "eagle.rom.addr.v6.ld", "rb" );
	if( !st )
	{
		fprintf( stderr, "Error: can't open eagle rom addresses\n" );
		exit (5);
	}

	while(!feof( st ) )
	{
		char st1[1024], st2[1024], symbol[1024], st4[1024], st5[1024], st6[1024];
		fscanf( st, "%s %s %s %s %s %s\n", st1, st2, symbol, st4, st5, st6 );
		int number = (int)strtol(st5, NULL, 0);
		symbolmap[symbol] = number - 0x40000000;
		symbolmaprev[number-0x40000000] = symbol;
		//printf( "%s = %d\n", symbol, number );
		printf( ".global %s\n", symbol );
	}


	FILE * f = fopen( "rom.bin", "rb" );
	fseek( f, 0, SEEK_END );
	int len = ftell( f );
	fseek( f ,0, SEEK_SET );
	unsigned char dat[len];
	fread( dat, 1, len, f );
	fclose( f );
	printf( ".section .rom\n" );
	printf( ".global rom_start\n" );
	printf( "rom_start:\n" );
	int i;
	for( i = 0; i < len; i++ )
	{
		string s = symbolmaprev[i];
		if( s.length() )
			printf( "%s:", s.c_str() );
		printf( ".byte %d\n", dat[i] );
	}
}

