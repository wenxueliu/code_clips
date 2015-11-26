#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>


struct pstat {
    long unsigned int uptime_sec;
    long unsigned int idletime_sec;
    long unsigned int utime_ticks;
    long int cutime_ticks;
    long unsigned int stime_ticks;
    long int cstime_ticks;
    long unsigned int starttime_ticks;
    long unsigned int vsize; // virtual memory size in bytes
    long unsigned int rss; //Resident Set Size in bytes
    long unsigned int cpu_total_time;
};

struct cpu_usage{
    double ucpu_usage;
    double scpu_usage;
};

long get_hertz()
{
    //printf("Hertz : %ld \n", sysconf(_SC_CLK_TCK));
    long hertz;
    if ((hertz = sysconf(_SC_CLK_TCK)) == -1){
        perror("sysconf(_SC_CLK_TCK) error");
        exit(EXIT_FAILURE);
    }
    return hertz;
}

int get_pagesize()
{
    return sysconf(_SC_PAGE_SIZE);
}

/*
* read /proc data into the passed struct pstat
* returns 0 on success, -1 on error
*/
int get_usage(const char *pid, struct pstat* result)
{

    //convert pid to string
    char pid_s[20];
    snprintf(pid_s, sizeof(pid_s), "%s", pid);
    char stat_filepath[30] = "/proc/"; 
    strncat(stat_filepath, pid_s, sizeof(stat_filepath) - strlen(stat_filepath) -1);
    strncat(stat_filepath, "/stat", sizeof(stat_filepath) - strlen(stat_filepath) -1);
    FILE *fpstat = fopen(stat_filepath, "r");
    if (fpstat == NULL) {
        perror("FOPEN /proc/[PID]/stat ERROR ");
        fclose(fpstat);
        return -1;
    }

    FILE *fstat = fopen("/proc/stat", "r");
    if (fstat == NULL) {
        perror("FOPEN /proc/stat ERROR ");
        fclose(fstat);
        return -1;
    }

    FILE *fuptime = fopen("/proc/uptime", "r");
    if (fuptime == NULL) {
        perror("FOPEN ERROR ");
        fclose(fuptime);
        return -1;
    }

    //read values from /proc/pid/stat
    bzero(result, sizeof(struct pstat));

    long int rss;
    //if (fscanf(fuptime, "%lu %lu", &result->uptime_sec,
    //            &result->idletime_sec) == EOF){
    //    fclose(fuptime);
    //    return -1;
    //}
    //fclose(fuptime);

    if (fscanf(fpstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
                        "%lu %ld %ld %*d %*d %*d %*d %lu %lu %ld",
                    &result->utime_ticks, &result->stime_ticks,
                    &result->cutime_ticks, &result->cstime_ticks,
                    &result->starttime_ticks, &result->vsize,
                    &rss) == EOF) {
        fclose(fpstat);
        return -1;
    }
    fclose(fpstat);
    result->rss = rss * get_pagesize();
    //read+calc cpu total time from /proc/stat
    long unsigned int cpu_time[10];
    bzero(cpu_time, sizeof(cpu_time));
    //if (fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
    //            &cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
    //            &cpu_time[4], &cpu_time[5], &cpu_time[6], &cpu_time[7],
    //            &cpu_time[8], &cpu_time[9]) == EOF) {
    if (fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                &cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
                &cpu_time[4], &cpu_time[5], &cpu_time[6], &cpu_time[7],
                &cpu_time[8], &cpu_time[9]) == EOF) {
        fclose(fstat);
        return -1;
    }
    fclose(fstat);
    for(int i=0; i < 4;i++){
        result->cpu_total_time += cpu_time[i];
    }
    return 0;
}

/*
* calculates the elapsed CPU usage between 2 measuring points. in percent
*/
void calc_cpu_usage_inter_percent(const struct pstat* cur_usage,
                            const struct pstat* last_usage,
                            struct cpu_usage *cpu_usage)
{
    bzero(cpu_usage, sizeof(struct cpu_usage));
    const long unsigned int total_time_diff = cur_usage->cpu_total_time - last_usage->cpu_total_time;
    cpu_usage->ucpu_usage = 100 * (((cur_usage->utime_ticks + cur_usage->cutime_ticks)
        - (last_usage->utime_ticks + last_usage->cutime_ticks))
        / (double) total_time_diff);
    cpu_usage->scpu_usage = 100 * ((((cur_usage->stime_ticks + cur_usage->cstime_ticks)
        - (last_usage->stime_ticks + last_usage->cstime_ticks))) /
        (double) total_time_diff);

}

double calc_cpu_usage_average_percent(const struct pstat* cur_usage)
{
    double total_time;
    double seconds;
    printf("utime %lu stime %lu\n",cur_usage->utime_ticks,cur_usage->stime_ticks);
    total_time = (cur_usage->utime_ticks + cur_usage->stime_ticks) / get_hertz();
    seconds = cur_usage->uptime_sec - cur_usage->starttime_ticks / get_hertz();
    return total_time * 1000 / seconds;
}
/*
* calculates the elapsed CPU usage between 2 measuring points in ticks
*/
void calc_cpu_usage(const struct pstat* cur_usage,
        const struct pstat* last_usage,
        long unsigned int* ucpu_usage,
        long unsigned int* scpu_usage)
{
    *ucpu_usage = (cur_usage->utime_ticks + cur_usage->cutime_ticks) -
    (last_usage->utime_ticks + last_usage->cutime_ticks);
    *scpu_usage = (cur_usage->stime_ticks + cur_usage->cstime_ticks) -
    (last_usage->stime_ticks + last_usage->cstime_ticks);
}

int main(int argc, char *argv[])
{
    int i = 0;
    struct pstat *cur_usage,  *last_usage;
    struct cpu_usage* cpu_usage;

    if (argc != 2){
        printf("usage : pragram <pid>\n");
        exit(EXIT_FAILURE);
    }

    if ((last_usage = malloc(sizeof(struct pstat))) == NULL)
    {
        perror("malloc last_usage error\n");
        exit(EXIT_FAILURE);
    }

    if ((cur_usage = malloc(sizeof(struct pstat))) == NULL)
    {
        perror("malloc cur_usage error\n");
        exit(EXIT_FAILURE);
    }

    if ((cpu_usage = malloc(sizeof(struct cpu_usage))) == NULL)
    {
        perror("malloc cpu_usage error\n");
        exit(EXIT_FAILURE);
    }


    for (i = 0; i < 10; i++)
    {
        get_usage(argv[1], last_usage);
        //printf("uptime_sec %lu",last_usage->uptime_sec);
        //printf("idletime_sec %lu",last_usage->idletime_sec);
        printf("utime_ticks %lu\t",last_usage->utime_ticks);
        printf("cutime_ticks %lu\t",last_usage->cutime_ticks);
        printf("stime_ticks %lu\t",last_usage->stime_ticks);
        printf("cstime_ticks %lu\n",last_usage->cstime_ticks);
        //printf("starttime_ticks %lu",last_usage->starttime_ticks);
        //printf("vsize %lu",last_usage->vsize);
        //printf("rss %lu",last_usage->rss);
        //printf("cpu_total_time %lu",last_usage->cpu_total_time);
        //printf("average percent : %f \n", calc_cpu_usage_average_percent(last_usage));
        sleep(1);
        get_usage(argv[1], cur_usage);
        calc_cpu_usage_inter_percent(cur_usage,last_usage,cpu_usage);
        //printf("utime %f  stime %f, total %f\n",cpu_usage->ucpu_usage,
        //    cpu_usage->scpu_usage, cpu_usage->ucpu_usage + cpu_usage->scpu_usage);
    }
    return 0;
}
