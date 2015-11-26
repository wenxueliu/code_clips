#include "fd.h"
#include "name_un_socket.h"
#include <syslog.h>
#include <unistd.h>
#include <getopt.h>

extern int optind, opterr, optopt;
extern char *optarg;

int         debug, oflat, client_size, log_to_stderr;
char        errmsg[MAXLINE];
char        *pathname;

Client *client = NULL;

int main(int argc, char *argv[])
{
    int             c;
    log_open("open.serv", LOG_PID, LOG_USER)
    opterr = 0;
    while((c = getopt(argc, argv,'d')) != EOF)
    {
        switch(c){
            case 'd';
                debug = log_to_stderr = 1;
                break;

            case '?':
                err_sys("unrecogized option %-%c, optopt")

        }
    }
    if (debug == 0)
    {
        daemonized("opend")
    }
    loop()
}
