/*
    linux handbook pg 250 13-5.

    The command tail [-n num] file prints the last num lines in a file (defailt
    is 10 lines). Implement this command using I/O system calls (lseek(), read()
    , write() etc. etc.) Keep in mind the buffering issues described in the 
    chapter to make the implementation efficient. 
    
    filename: mainTail.c
    
*/

#include <sys/types.h>/* portability, start programs with it. (why in book) */
#include <stdlib.h>   /* exit() */
#include <unistd.h>   /* getopt(); declares NULL for me */
#include <getopt.h>   /* getopt(); can compile w/o implicit warning in c99 */
/* #define NDEBUG // if defined no assertions are implemented */
#include <assert.h>   /* debugging */

#include "my_tail.h"  /* get_num.h ; err_handle.h ; <inttypes.h> see my_tail.h */


int32_t main(int32_t argc, char *argv[])
{
    uint32_t num = 10;      /* number of lines to print, default 10 */
    int32_t opt = 0;        /* handles the return value of getopt */ 
    char *path = NULL;      /* name of path to display tail */
    
    if(argc > 4){
        noerrExit("Too many arguments, $cmd [-n num] file");}

    /* since there is only 1 flag, while loop is overkill. Wanted to show how
       to properly use getopt.
       
       Also, with while/switch we can easily implement aditional command line
       arguments, and therefore additional functionalities. */
    while(argc > 2 && (opt = getopt(argc, argv, "n:")) != -1)
    {
        switch(opt)
        {
            case 'n': num = getu32_t(argv[optind-1], 0, (char*)NULL); break;
            default : noerrExit("Invalid arguments, $cmd [-n num] file"); 
        } 
    } /* end while */

    /* optind index of first non-option. If no options, argv[1] is filename */
    path = (argc == 2) ? argv[1] : argv[optind]; 
    
    sedihs_tail(path, num);
     
    exit(EXIT_SUCCESS);
} /* end main */


