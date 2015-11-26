#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
struct _IO_FILE {
int _flags;
#define _IO_file_flags _flags

char* _IO_read_ptr;
char* _IO_read_end;
char* _IO_read_base;
char* _IO_write_base;
char* _IO_write_ptr;
char* _IO_write_end;
char* _IO_buf_base;
char* _IO_buf_end;

char *_IO_save_base;
char *_IO_backup_base;
char *_IO_save_end;

struct _IO_marker *_markers;

struct _IO_FILE *_chain;

int _fileno;
};
*/

int main(void)
{
	  char buf[5];
	  FILE *myfile =stdin;
	  printf("before reading\n");
	  printf("read buffer base %p\n", myfile->_IO_read_base);
	  printf("read buffer length %d\n", myfile->_IO_read_end - myfile->_IO_read_base);
	  printf("write buffer base %p\n", myfile->_IO_write_base);
	  printf("write buffer length %d\n", myfile->_IO_write_end - myfile->_IO_write_base);
	  printf("buf buffer base %p\n", myfile->_IO_buf_base);
	  printf("buf buffer length %d\n", myfile->_IO_buf_end - myfile->_IO_buf_base);
	  printf("\n");
	  fgets(buf, 5, myfile);
	  fputs(buf, myfile);
	  printf("\n");
	  printf("after reading\n");
	  printf("read buffer base %p\n", myfile->_IO_read_base);
	  printf("read buffer length %d\n", myfile->_IO_read_end - myfile->_IO_read_base);
	  printf("write buffer base %p\n", myfile->_IO_write_base);
	  printf("write buffer length %d\n", myfile->_IO_write_end - myfile->_IO_write_base);
	  printf("buf buffer base %p\n", myfile->_IO_buf_base);
	  printf("buf buffer length %d\n", myfile->_IO_buf_end - myfile->_IO_buf_base);

	  return 0;
}
