#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
    char buf[25];
    char *cur;
    FILE *myfile;
    myfile = fopen("bbb.txt", "r");
    printf("before reading, myfile->_IO_read_ptr: %d\n", myfile->_IO_read_ptr - myfile->_IO_read_base);
    printf("before reading, myfile->_IO_read_length: %d\n", myfile->_IO_read_end - myfile->_IO_read_base);
    printf("before reading, myfile->_IO_buf_length: %d\n", myfile->_IO_buf_end - 
            myfile->_IO_buf_base);
    fgets(buf, 20, myfile); 
    printf("\nafterreading, myfile->_IO_buf_length: %d\n", myfile->_IO_buf_end -
            myfile->_IO_buf_base);
    cur = myfile->_IO_read_base;
    while (cur < myfile->_IO_read_end) //实际上读满了这个缓冲区
    {
        printf("%c",*cur);
        cur++;
    }
    printf("\nafter reading, myfile->_IO_read_ptr: %d\n", myfile->_IO_read_ptr - myfile->_IO_read_base);
    printf("\nafter reading, myfile->_IO_read_length: %d\n", myfile->_IO_read_end - myfile->_IO_read_base);
    return 0;
}
