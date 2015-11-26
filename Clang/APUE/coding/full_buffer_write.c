#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  char buf[2048]={0};
  int i;
  FILE *myfile;
  myfile = fopen("aaa.txt", "r+");
  i= 0;
  while (i<2048)
  {
    fwrite(buf+i, 1, 512, myfile);
    i +=512;
    //注释掉这句则可以写入aaa.txt
    myfile->_IO_write_ptr = myfile->_IO_write_base;
    printf("%p write buffer base\n", myfile->_IO_write_base);
    printf("%p buf buffer base \n", myfile->_IO_buf_base);
    printf("%p read buffer base \n", myfile->_IO_read_base);
    printf("%p write buffer ptr \n", myfile->_IO_write_ptr);
    printf("\n");
  }
  return 0;
}
