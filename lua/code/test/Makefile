GCC = gcc

G++ = g++

CFLAGS = -I/usr/local/include/luajit-2.0/ -c -g -fPIC -O0   #-->本地lua编辑器路径及生成对应.o文件的指令

LINKFLG = -shared -fPIC

#-->生成helloworld.o
.cpp.o:
	$(G++) $(CFLAGS) -o $@ $<                                

all:dump.so object.so

#-->编译指令：make all

.PHONY:clean

#-->清除指令：make clean,清除现有的.o和.so文件
clean:
	rm -rf *.o *.so
#-->生成helloworld.so文件

dump.so:dump.o
	@echo "compile test1..."
	$(G++) $(LINKFLG) -o $@ $?
	
object.so:object.o
	@echo "compile test1..."
	$(G++) $(LINKFLG) -o $@ $?


