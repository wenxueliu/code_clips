#ifndef AIO_RWFUN_H
#define AIO_RWFUN_H

/*
 * initialize a afdata struct
 * @param fd file description
 * @param afd eventfd
 * @param n num of event or num of buf
 * */
struct afdata *io_prep_afddata(int fd,int afd,int n);

/*
 *submit read request
 *@param pafdata fd n afd above
 *@iortx_size file buf size :iortx_size * n == filesize
 * */
long io_submit_read(struct afdata *pafdata,int fd,int n,int iortx_size,int afd);

/*
 *submit write request
 *@prarm above
 *@param buf the data ready to write to file,buf is a char * array which length is n,each element is a length iortx_size of string 
 * */
long io_submit_write(struct afdata *pafdata,int fd,int n,int iortx_size,int afd,char **buf);

/*
 *get the buf which obtain from file
 *
 * */
char ** io_obt_filebuf(struct afdata *pafdata,int afd,int num_events,int iortx_size,int *nbuf);

/*
 *destory pafdata
 * */
void io_destroy_afdata(struct afdata *pafdata,int n);

#endif /*AIO_RWFUN_H*/
