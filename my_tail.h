/*
    linux handbook pg 250 13-5.

    The command tail [-n num] file prints the last num lines in a file (defailt
    is 10 lines). Implement this command using I/O system calls (lseek(), read()
    , write() etc. etc.) Keep in mind the buffering issues described in the 
    chapter to make the implementation efficient. 
    
    filename: my_tail.h

    functions to implement $tail [-n num] file for main
*/

#ifndef _MY_TAIL_H
#define _MY_TAIL_H

#ifndef Restrict
#define Restrict __restrict
#endif 

#include <sys/types.h> /* portability, start programs with it. (why in book) */
#include <inttypes.h>  /* integer, limits, printf portability */ 

#include "get_num/get_num.h"       /* get numbers from strings */
#include "err_handle/err_handle.h" /* errors, <errno.h> */

/* print the last num lines to stdout using system I/O.
   Return values: 1 on success, -1 on error, exit() on fatal error */
int32_t sedihs_tail(const char *Restrict path, int32_t num); 
#endif
