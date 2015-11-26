#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	0600	/* user read/write */

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define	NLOOPS		1000
#define	SIZE		sizeof(long)	/* size of shared memory area */


char	array[ARRAY_SIZE];	/* uninitialized data = bss */

int test_shm()
{
	int		shmid;
	char	*ptr, *shmptr;

	printf("array[] from %p to %p\n", (void *)&array[0],
	  (void *)&array[ARRAY_SIZE]);
	printf("stack around %p\n", (void *)&shmid);

	if ((ptr = malloc(MALLOC_SIZE)) == NULL){
		printf("malloc error");
        return -1;
    }
	printf("malloced from %p to %p\n", (void *)ptr,
	  (void *)ptr+MALLOC_SIZE);

	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
		printf("shmget error");

	if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1){
		printf("shmat error");
        return -1;
    }
	printf("shared memory attached from %p to %p\n", (void *)shmptr,
	  (void *)shmptr+SHM_SIZE);

	if (shmctl(shmid, IPC_RMID, 0) < 0){
		printf("shmctl error");
    }

}


/* parent/child from {Sec race_conditions} */
static int	pfd1[2], pfd2[2];
int TELL_WAIT(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0){
		printf("pipe error");
        return -1;
    }
}

int TELL_PARENT(pid_t pid)
{
	if (write(pfd2[1], "c", 1) != 1)
		printf("write error");
        return -1;
}

int TELL_CHILD(pid_t pid)
{
	if (write(pfd1[1], "p", 1) != 1){
	    printf("write error");
        return -1;
    }
}

int WAIT_PARENT(void)
{
	char	c;

	if (read(pfd1[0], &c, 1) != 1){
		printf("read error");
        return -1;
    }

	if (c != 'p'){
		printf("WAIT_PARENT: incorrect data");
        return -1;
    }
}

int WAIT_CHILD(void)
{
	char	c;

	if (read(pfd2[0], &c, 1) != 1){
		printf("read error");
        return -1;
    }

	if (c != 'c'){
		printf("WAIT_CHILD: incorrect data");
        return -1;
    }
}

static int update(long *ptr)
{
	return((*ptr)++);	/* return value before increment */
}

int devzero()
{
	int		fd, i, counter;
	pid_t	pid;
	void	*area;

	if ((fd = open("/dev/zero", O_RDWR)) < 0){
		printf("open error");
        return -1;
    }
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
	  fd, 0)) == MAP_FAILED){
		printf("mmap error");
        return -1;
    }
	close(fd);		/* can close /dev/zero now that it's mapped */

	TELL_WAIT();

	if ((pid = fork()) < 0) {
		printf("fork error");
        return -1;
	}
    else if (pid > 0) {			/* parent */
		for (i = 0; i < NLOOPS; i += 2) {
			if ((counter = update((long *)area)) != i){
				printf("parent: expected %d, got %d", i, counter);
            }

			TELL_CHILD(pid);
			WAIT_CHILD();
		}
	}
    else {						/* child */
		for (i = 1; i < NLOOPS + 1; i += 2) {
			WAIT_PARENT();

			if ((counter = update((long *)area)) != i){
				printf("child: expected %d, got %d", i, counter);
            }

			TELL_PARENT(getppid());
		}
	}
}

int main(int argc, char *argv[])
{
    test_shm();
	exit(0);
}

