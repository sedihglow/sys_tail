main:	mainTail.c my_tail.c err_handle/err_handle.c get_num/get_num.c
	gcc -Wall -Wextra -g -O0 -m64 -pedantic -Wno-long-long mainTail.c my_tail.c err_handle/err_handle.c get_num/get_num.c
