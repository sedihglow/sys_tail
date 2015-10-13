/*


    linux handbook pg 250 13-5.

    The command tail [-n num] file prints the last num lines in a file (defailt
    is 10 lines). Implement this command using I/O system calls (lseek(), read()
    , write() etc. etc.) Keep in mind the buffering issues described in the 
    chapter to make the implementation efficient. 
    
    filename: my_tail.c
    
*/

/* HEADER NOTE: There is no stdio.h! No standard C io involved. 
                (C++ its == to no <iostream>) */
                

#include <sys/types.h>/* portability, start programs with it. (why in book) */
#include <sys/stat.h> /* open() */
#include <fcntl.h>    /* open() */
#include <unistd.h>   /* read(), write(), close(), lseek(), declares NULL */
#include <malloc.h>   /* malloc/free family */
#include <string.h>   /* memset() */
/* #define NDEBUG // if defined no assertions are implemented */
#include <assert.h>  /* debugging */

#include "my_tail.h" /* "get_num.h" ; "err_handle.h",<errno.h>,"ename.c.inc" ;
                         <inttypes.h>, see my_tail.h */

#define BUFF    1024

/* used in sedihs_tail. Checks and handles when offset reaches negetive */
#define bouch_check(offs, fbufSize, toRead, nlC, num)/*#{{{*/\
{                                                            \
    if(offs < 0){                                            \
        if(fbufSize == 1){                                   \
            toRead = start + toRead;}                        \
        else /* set toRead, break loop with nlC and num */   \
        {                                                    \
            toRead = start + fbufSize;                       \
            nlC = num;                                       \
        } /* end else */                                     \
        offs = 0; /* set offset to start. final read inc. */ \
    } /* end if */                                           \
} /* end bounce_check #}}} */                                     \

/* checks for the implicit newline that linux files have at EOF (set eol) */
#define first_buff()/*#{{{*/                                 \
{                                                            \
    assert(fbuffSize == 1);                                  \
    if(buff[readRet-1] == '\n'){                             \
        --nlCount;}                                          \
                                                             \
    reqBytes = count_nl(buff, readRet, &nlCount, nlMax);     \
    start -= reqBytes;                                       \
                                                             \
    if(reqBytes < BUFF){                                     \
        nlCount = nlMax;}                                    \
} /* end first_buff #}}} */

/* Opens file is the mask O_RDONLY to protect file data.
   Sets fd offset to SEEK_END
   Errors: If file is empty, return -1.
           If file fails to open, errExit() is called.

   Success: If file is not empty, return fd */
static inline int32_t
init_file(const char *Restrict path, off_t *st) /*#{{{*/
{
    int32_t fd = -1;

    if((fd = open(path, O_RDONLY)) == -1){
        errExit("init_file(), open() failure");}

    /* when fseek returns 0, file is empty. */
    if((*st = lseek(fd, 0, SEEK_END)) == -1 || *st == 0)
    {
        close(fd);
        return -1;
    } 
    return fd;
} /* end init_file #}}} */

/* Returns the required bytes needed for the final buffer, assuming the final
   buffer's requires size was less than the buff size. This is a value buff
   needs for its final malloc.

   Else returns the original start value
   Incriments *nlCount */
static ssize_t
count_nl(const char *Restrict buff, const uint64_t start, int32_t *nlCount,/*#{{{*/
         const int32_t nlMax)
{
    int64_t i = 0;
    int64_t nlFound = *nlCount;

    for(i = start-1; i > -1; --i)
    {
        if(buff[i] == '\n')
        {
            ++nlFound;
            if(nlFound == nlMax)
            {
                *nlCount = nlFound;
                return start - (i+1);
            } /* end if */
        } /* end if */
    } /* end for */

    *nlCount = nlFound;
    return start;
} /* end count_nl #}}} */


int32_t sedihs_tail(const char *Restrict path, int32_t nlMax)/*#{{{*/
{
    char *buff = NULL;      /* allocated for initial and final buff */
    uint32_t fbuffSize = 0; /* num of buff size required to allocate in loop */
    int32_t fd = -1;        /* initialized to non-existant fd */
    int32_t nlCount = 0;    /* current count of newlines found */
    int32_t readRet = 0;    /* bytes read into buf from read */
    ssize_t reqBytes = 0;   /* bytes required for last fbuffSize */
    size_t toRead = 0;      /* number of bytes to read into buff */
    off_t start = 0;        /* offset to read from throughout code */

    if((fd = init_file(path, &start)) == -1){
        return 1;} /* success, file was empty. So we "printed" it */
    
    buff = (char*) malloc(sizeof(char) * BUFF);
    toRead = BUFF;
    start -= BUFF;

    /* final buffer will require buffSize*nBytes to read/write only the lines */
    do
    {
        ++fbuffSize; 
        memset(buff, '\0', BUFF);
    
        bouch_check(start, fbuffSize, toRead, nlCount, nlMax);

        if((readRet = pread(fd, buff, toRead, start)) == -1){
            errExit("pread() failure.");}
        
        if(fbuffSize > 1)
        {
            reqBytes = count_nl(buff, start, &nlCount, nlMax);
            start -= reqBytes;
        }
        else{
            first_buff();}
    }while(nlCount != nlMax);
    
    free(buff); /* no longer need the current allocation of buff */ 

    reqBytes = sizeof(char) * ((--fbuffSize * BUFF) + reqBytes);
    buff = (char*) malloc(reqBytes);
    memset(buff,'\0',reqBytes);

    start = lseek(fd, -(reqBytes), SEEK_END);

    if(pread(fd, buff, reqBytes, start) == -1){
        errExit("final read");}

    if(write(STDOUT_FILENO, buff, reqBytes) == -1){
        errExit("final write");}

    free(buff);
    return 1;
} /* end sedihs_tail #}}} */
