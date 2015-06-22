#include"aio_rwfundef.h"
#include"aio_fundef.h"
#include"aio_fun.h"

struct afdata *io_prep_afddata(int fd,int afd,int n)
{
	struct afdata *pafdata = (struct afdata*)malloc(sizeof(struct afdata));
	pafdata->fd = fd;
	pafdata->afd = afd;
	pafdata->iocb = calloc(n,sizeof(struct iocb));
	pafdata->piocb = calloc(n,sizeof(struct iocb *));
	pafdata->ctx = calloc(1,sizeof(aio_context_t));
	
	if(!pafdata->iocb || !pafdata->piocb || !pafdata->ctx)
	{
		perror("iocb alloc");
		return NULL;
	}
    
	if(io_setup(n + 256,pafdata->ctx))
	{
		perror("io_setup");
		return NULL;
	}
    
	fcntl(afd, F_SETFL, fcntl(afd, F_GETFL, 0) | O_NONBLOCK);
	
	return pafdata;
}


long io_submit_read(struct afdata *pafdata,int fd,long n,int iortx_size,int afd)
{
	//assert(pafdata != NULL);
    int i, r;
	for(i = 0;i < n;i ++)
	{
		pafdata->piocb[i] = &pafdata->iocb[i];
		char *buf = malloc(iortx_size);
		asyio_prep_pread(&pafdata->iocb[i],fd,buf,iortx_size,(n - i -1) * iortx_size,afd);
		pafdata->iocb[i].aio_data = (u_int64_t) i + 1;
	}
	if((r = io_submit(*pafdata->ctx,n,pafdata->piocb)) <= 0)
	{
		perror("io_submit");
		return -1;
	}
	return 0;
}


long io_submit_write(struct afdata *pafdata,int fd,int n,int iortx_size,int afd,char **buf)
{
	int i, r;
	for(i = 0;i < n;i ++)
	{
		pafdata->piocb[i] = &pafdata->iocb[i];
		asyio_prep_pwrite(&pafdata->iocb[i],fd,buf[i],iortx_size,(n - i -1) * iortx_size,afd);
		pafdata->iocb[i].aio_data = (u_int64_t) i + 1;
	}
	if((r = io_submit(*pafdata->ctx,n,pafdata->piocb)) <= 0)
	{
		perror("io_submit");
		return -1;
	}
	return 0;
}

char ** io_obt_filebuf(struct afdata *pafdata,int afd,int num_events,int iortx_size,int *nbuf)
{
	u_int64_t eval = 0;
	int r, min_events = 1, j, k = 0, i = 0;
	struct timespec tmo;
	struct io_event *events = malloc(num_events * sizeof(struct io_event));
	if(read(afd,&eval,sizeof(eval)) != sizeof(eval))
	{
		perror("read");
	}
	char **bufarray = malloc(eval * sizeof(char *));
	*nbuf = eval;
	while(eval > 0)
	{
		tmo.tv_nsec = 0;
		r = io_getevents(*pafdata->ctx, min_events, eval > num_events ? num_events:(long) eval, events, &tmo);
		if(r > 0)
		{
			for(j = r - 1; j >= 0; j --)
			{
				struct iocb *piocb = (struct iocb *)events[j].obj;
				bufarray[k ++] = (char *)piocb->aio_buf;
			}
			i += r;
			eval -= r;
		}
	}
	return bufarray;
}


void io_destroy_afdata(struct afdata *pafdata,int n)
{
	int i;
	for(i = 0;i < n; i ++)
	{
		if(pafdata->iocb[i].aio_buf != 0)
		{
			free((char *)pafdata->iocb[i].aio_buf);
		}
	}
	io_destroy(*pafdata->ctx);
	free(pafdata->ctx);
	free(pafdata->iocb);
	free(pafdata->piocb);
}
