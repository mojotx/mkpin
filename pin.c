#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    if (argc == 2 ) {
        char *ep=0;
        ulong_t qty = strtoul( argv[1], &ep, 10 );

        if ( !qty || errno || *ep ) {
            if ( ep && *ep )
                fprintf( stderr, "Invalid argument:  %s (I don't understand the part starting with \"%s...\")\n", argv[1], ep );
            if ( !qty )
                fprintf( stderr, "Invalid argument:  %s (Must be non-zero)\n", argv[1] );
            if ( errno )
                fprintf( stderr, "Invalid argument: %s [%s]\n", argv[1], strerror(errno));

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
