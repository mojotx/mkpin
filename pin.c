#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <assert.h>

/*
 * This program generates a pseudorandom 10-digit number, from the
 * range of 0000000001 - 9999999999. Therefore, the maximum value must
 * be less than 10e11.
 *
 * The value of UINT32_MAX (4294967295) is less than our 10-digit max
 * of 9999999999, and 9999999999 % UINT32_MAX is 1410065409UL.
 */

typedef unsigned long ulong_t;

#define MAXD (9999999999UL)

static ulong_t
get_val( void )
{
    ulong_t val=0;

    for ( ulong_t i=0; i<(MAXD / UINT32_MAX); i++)
        val += arc4random_uniform(UINT32_MAX)+1;
    val += arc4random_uniform(MAXD % UINT32_MAX)+1;
    assert( val <= MAXD );
    return val;
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
                return EXIT_FAILURE;
        }
    }

    if (optind < argc) {
        char *ep=0;
        ulong_t qty = strtoul( argv[optind], &ep, 10 );

        if ( !qty || errno || *ep ) {
            if ( errno )
                fprintf( stderr, "%s [%s]\n", argv[optind], strerror(errno));
            else if ( ep && *ep )
                fprintf( stderr, "•  %s (I don't understand the part starting with \"%s...\")\n", argv[optind], ep );
            else if ( !qty )
                fprintf( stderr, "•  %s (Must be non-zero)\n", argv[optind] );
            else
                fputs("Unknown error: Please open an issue at\n\nhttps://github.com/mojotx/mkpin/issues", stderr);

            return EXIT_FAILURE;
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

    return EXIT_SUCCESS;
}
