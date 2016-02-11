#gcc -m32 -g -ggdb -Wall -Wextra main.c asm.S -o step1
#In file included from /usr/include/stdio.h:27:0,
#                 from main.c:1:
#/usr/include/features.h:374:25: fatal error: sys/cdefs.h: No such file or directory
# #  include <sys/cdefs.h>
#                         ^
#compilation terminated.
#make: *** [step1] Error 1
#

step1: main.c asm.S ../macros.h Makefile
	gcc -m64 -g -ggdb -Wall -Wextra main.c asm.S -o $@
