#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

pthread_key_t key_int;
pthread_key_t thread_log_key;
pthread_key_t key_struct;

struct test_struct {
    int i;
    float k;
};

void *func_key_struct1 (void *arg)
{
    struct test_struct struct_data;

    struct_data.i = 10;
    struct_data.k = 3.1415;

    pthread_setspecific (key_struct, &struct_data);
    struct test_struct *value = (struct test_struct *)pthread_getspecific(key_struct);
    printf ("thread %u func %s value are address=0x%p\t struct_data.i=%d\t struct_data.k: %f\n",
            (unsigned int)pthread_self(), __func__, value, value->i, value->k);
}

void *func_key_struct2 (void *arg)
{
    struct test_struct struct_data;
    struct_data.i = 20;
    struct_data.k = 6.2830;

    pthread_setspecific (key_struct, &struct_data);
    struct test_struct *value = (struct test_struct *)pthread_getspecific(key_struct);
    printf ("thread %u func %s value are address=0x%p\t struct_data.i=%d\t struct_data.k: %f\n",
            (unsigned int)pthread_self(), __func__, value, value->i, value->k);
}

void func_key_int_internal() {
    int *tmp = (int*)pthread_getspecific(key_int);//同一线程内的各个函数间共享数据。
    printf("thread %u value %d is runing in %s\n", (unsigned int)pthread_self(), *tmp, __func__);
}

void *func_key_int(void *args) {
    pthread_setspecific(key_int, args);
    int *tmp = (int*)pthread_getspecific(key_int);//获得线程的私有空间
    printf("thread %u value %d is runing in %s\n", (unsigned int)pthread_self(), *tmp, __func__);
    *tmp = (*tmp)*100;//修改私有变量的值
    func_key_int_internal();
    return (void*)0;
}

/*通用函数里可以利用 pthread_getspecific() 处理线程各自的私有数据*/
void write_to_thread_log (const char *message) {
    FILE *thread_log = (FILE *)pthread_getspecific (thread_log_key);
    fprintf (thread_log, "%s\\n", message);
}

void close_thread_log (void *thread_log) {
    fclose ((FILE *)thread_log);
}

void *func_key_file(void *args) {
    char thread_log_filename[128];
    char thread_start_message[128];

    FILE *thread_log;

    sprintf (thread_log_filename, "thread %u.log", (unsigned int)pthread_self());
    thread_log = fopen (thread_log_filename, "w");
    pthread_setspecific (thread_log_key, thread_log); //每个线程都设置自己的私有数据
    sprintf (thread_start_message, "thread %u starting", (unsigned int)pthread_self());
    write_to_thread_log (thread_start_message);

    pthread_exit(NULL);
}

int main(int argc, char **agrv)
{
    pthread_t t1, t2;
    //--------------------- key_int ---------------------
    int a=1;
    int b=2;
    pthread_key_create(&key_int, NULL);
    pthread_create(&t1, NULL, func_key_int, &a);
    pthread_create(&t2, NULL, func_key_int, &b);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    //struct test
    pthread_t tid1, tid2;
    pthread_key_create (&key_struct, NULL);
    pthread_create (&tid1, NULL, (void *)func_key_struct1, NULL);
    pthread_create (&tid2, NULL, (void *)func_key_struct2, NULL);
    pthread_join (tid1, NULL);
    pthread_join (tid2, NULL);
    pthread_key_delete(key_struct);

    //--------------------- key_file---------------------
    int i;
    pthread_t threads[5];
    /*创建私有数据键,close_thread_log 在线程退出时对 key 关联数据进行清理*/
    pthread_key_create (&thread_log_key, close_thread_log);
    for (i = 0; i < 5; i++) {
        pthread_create (&threads[i], NULL, func_key_file, NULL); //创建多线程
    }
    for (i = 0; i < 5; i++) {
        pthread_join (threads[i], NULL);  //等待各个线程结束
    }

    return (0);
}
