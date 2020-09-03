#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>

#define MAGIC 1410065410U

typedef unsigned long ulong_t;

static ulong_t
get_val( void )
{
    // Generate a random 10-digit number.
    ulong_t    a, b, c;

    a = arc4random_uniform(UINT32_MAX)+1;
    b = arc4random_uniform(UINT32_MAX)+1;
    c = arc4random_uniform(MAGIC)+1;

    return a + b + c;
}

static void
spinner( void )
{
    static char cs[] = { '-', '\\', '|', '/', '\0' };

    static int i;

    putchar( '\r' );
    putchar( cs[i] );
    fflush(stdout);
    i = ( i==3? 0 : i+1 );
}

static void
usage( const char *prg )
{
    fprintf(stderr, "Usage: %s [ -h ] [ n ]\n\n", prg );
    fputs( "If no parameters are passed, generates a 10-digit random PIN.\n"
           "If a number is provided, it generates that many 10-digit random\n"
           "pins, and reports the max and min values for testing purposes.\n\n"
           "If the -h parameter is passed, or any other parameter, this help\n"
           "is displayed.\n\n", stderr);

    fprintf( stderr,
            "Example 1:\n\n$ %s 5\nAfter 5 iterations, min=3034540120, max=7743265784\n\n"
            "Example 2:\n\n$ %s\n4673447243\n\n", prg, prg );
}



int main(int argc, char *argv[])
{
    opterr=0;
    int c;
    while ((c=getopt(argc, argv, "h")) != -1) {
        switch (c){
            case 'h':
                usage(argv[0]);
                return 1;

            case '?':
            default:
                if (isprint(optopt))
                    fprintf( stderr, "Error: Unknown option '-%c'\n", optopt );
                else
                    fprintf( stderr, "Error: Unknown option '\\x%x'\n", optopt );
                return 1;
        }
    }

    if (optind < argc) {
        char *ep=0;
        ulong_t qty = strtoul( argv[optind], &ep, 10 );

        if ( !qty || errno || *ep ) {
            if ( ep && *ep )
                fprintf( stderr, "Invalid argument:  %s (I don't understand the part starting with \"%s...\")\n", argv[optind], ep );
            if ( !qty )
                fprintf( stderr, "Invalid argument:  %s (Must be non-zero)\n", argv[optind] );
            if ( errno )
                fprintf( stderr, "Invalid argument: %s [%s]\n", argv[optind], strerror(errno));

            return 1;
        }

        ulong_t min=ULONG_MAX;
        ulong_t max=0;

        for ( ulong_t i=0; i<qty; i++) {

            if ( i % 1000000 == 0 )
                spinner();

            ulong_t z = get_val();
            if ( z > max )
                max = z;
            if ( z < min )
                min = z;
        }

        fputs( "\r \r", stdout );

        printf( "After %lu iterations, min=%010lu, max=%010lu\n", qty, min, max );
    }
    else {
        printf( "%lu\n", get_val() );
    }

    return 0;
}
