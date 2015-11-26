/*
 * From openvswitch daemon-unix.c
 *
 *
 * */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "dynamic-string.h"
#include "type-props.h"
#include <stdbool.h>

#define OVS_SOURCE_LOCATOR __FILE__ ":" OVS_STRINGIZE(__LINE__)
#define OVS_STRINGIZE(ARG) OVS_STRINGIZE2(ARG)
#define OVS_STRINGIZE2(ARG) #ARG
typedef unsigned char uint8, uint8_t, u8;
#define ARRAY_SIZE(_x)  ((sizeof(_x))/sizeof (_x)[0])

/* For each of the standard file descriptors, whether to replace it by
 * /dev/null (if false) or keep it for the daemon to use (if true). */
static bool save_fds[3];

/* --detach: Should we run in the background? */
bool detach;                    /* Was --detach specified? */
static bool detached;           /* Have we already detached? */
/* File descriptor used by daemonize_start() and daemonize_complete(). */
static int daemonize_fd = -1;

/* --no-chdir: Should we chdir to "/"? */
static bool chdir_ = true;

/* --monitor: Should a supervisory process monitor the daemon and restart it if
 * it dies due to an error signal? */
static bool monitor;

/* --pidfile: Name of pidfile (null if none). */
char *pidfile;

/* If there is a reason that we cannot fork anymore (unless the fork will be
 * immediately followed by an exec), then this points to a string that
 * explains why. */
static const char *must_not_fork;

void
xpipe(int fds[2])
{
    if (pipe(fds)) {
        //VLOG_FATAL("failed to create pipe (%s)", ovs_strerror(errno));
        printf("failed to create pipe (%d)", errno);
    }
}

char *
xmemdup0(const char *p_, size_t length)
{
    char *p = xmalloc(length + 1);
    memcpy(p, p_, length);
    p[length] = '\0';
    return p;
}

char *
xstrdup(const char *s)
{
    return xmemdup0(s, strlen(s));
}

void xsleep(unsigned int seconds)
{
    //ovsrcu_quiesce_start();
#ifdef _WIN32
    Sleep(seconds * 1000);
#else
    sleep(seconds);
#endif
    //ovsrcu_quiesce_end();
}

/* Marks all the currently locked lockfiles as no longer locked.  It makes
 * sense to call this function after fork(), because a child created by fork()
 * does not hold its parents' locks. */
void lockfile_postfork(void)
{
    struct lockfile *lockfile;

    //ovs_mutex_lock(&lock_table_mutex);
    //HMAP_FOR_EACH (lockfile, hmap_node, lock_table) {
    //    if (lockfile->fd >= 0) {
    //        VLOG_WARN("%s: child does not inherit lock", lockfile->name);
    //        lockfile_unhash(lockfile);
    //    }
    //}
    //ovs_mutex_unlock(&lock_table_mutex);
}

/* Clears all of the fatal signal hooks without executing them.  If any of the
 * hooks passed a 'cancel_cb' function to fatal_signal_add_hook(), then those
 * functions will be called, allowing them to free resources, etc.
 *
 * Following a fork, one of the resulting processes can call this function to
 * allow it to terminate without calling the hooks registered before calling
 * this function.  New hooks registered after calling this function will take
 * effect normally. */
void
fatal_signal_fork(void)
{
    //size_t i;

    //assert_single_threaded();

    //for (i = 0; i < n_hooks; i++) {
    //    struct hook *h = &hooks[i];
    //    if (h->cancel_cb) {
    //        h->cancel_cb(h->aux);
    //    }
    //}
    //n_hooks = 0;

    ///* Raise any signals that we have already received with the default
    // * handler. */
    //if (stored_sig_nr != SIG_ATOMIC_MAX) {
    //    raise(stored_sig_nr);
    //}
}




pid_t xfork_at(const char *where);
#define xfork() xfork_at(OVS_SOURCE_LOCATOR)

/* Forks the current process (checking that this is allowed).  Aborts with
 * VLOG_FATAL if fork() returns an error, and otherwise returns the value
 * returned by fork().
 *
 * ('where' is used in logging.  Commonly one would use xfork() to
 * automatically provide the caller's source file and line number for
 * 'where'.) */
pid_t xfork_at(const char *where)
{
    pid_t pid;

    if (must_not_fork) {
        printf("%s: attempted to fork but forking not allowed (%s)",
                   where, must_not_fork);
    }

    pid = fork();
    if (pid < 0) {
        printf("%s: fork failed (%d)", where, errno);
    }
    return pid;
}





/* Calls fork() and on success returns its return value.  On failure, logs an
 * error and exits unsuccessfully.
 *
 * Post-fork, but before returning, this function calls a few other functions
 * that are generally useful if the child isn't planning to exec a new
 * process. */
static pid_t
fork_and_clean_up(void)
{
    pid_t pid = xfork();
    if (pid > 0) {
        /* Running in parent process. */
        fatal_signal_fork();
    } else if (!pid) {
        /* Running in child process. */
        lockfile_postfork();
    }
    return pid;
}

int read_fully(int fd, void *p_, size_t size, size_t *bytes_read)
{
    uint8_t *p = p_;

    *bytes_read = 0;
    while (size > 0) {
        ssize_t retval = read(fd, p, size);
        if (retval > 0) {
            *bytes_read += retval;
            size -= retval;
            p += retval;
        } else if (retval == 0) {
            return EOF;
        } else if (errno != EINTR) {
            return errno;
        }
    }
    return 0;
}

/* Given 'status', which is a process status in the form reported by waitpid(2)
 * and returned by process_status(), returns a string describing how the
 * process terminated.  The caller is responsible for freeing the string when
 * it is no longer needed. */
char *
process_status_msg(int status)
{
    struct ds ds = DS_EMPTY_INITIALIZER;
#ifndef _WIN32
    if (WIFEXITED(status)) {
        ds_put_format(&ds, "exit status %d", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        char namebuf[SIGNAL_NAME_BUFSIZE];

        ds_put_format(&ds, "killed (%s)",
                      signal_name(WTERMSIG(status), namebuf, sizeof namebuf));
    } else if (WIFSTOPPED(status)) {
        char namebuf[SIGNAL_NAME_BUFSIZE];

        ds_put_format(&ds, "stopped (%s)",
                      signal_name(WSTOPSIG(status), namebuf, sizeof namebuf));
    } else {
        ds_put_format(&ds, "terminated abnormally (%x)", status);
    }
    if (WCOREDUMP(status)) {
        ds_put_cstr(&ds, ", core dumped");
    }
#else
    ds_put_cstr(&ds, "function not supported.");
#endif
    return ds_cstr(&ds);
}

/* Forks, then:
 *
 *   - In the parent, waits for the child to signal that it has completed its
 *     startup sequence.  Then stores -1 in '*fdp' and returns the child's
 *     pid in '*child_pid' argument.
 *
 *   - In the child, stores a fd in '*fdp' and returns 0 through '*child_pid'
 *     argument.  The caller should pass the fd to fork_notify_startup() after
 *     it finishes its startup sequence.
 *
 * Returns 0 on success.  If something goes wrong and child process was not
 * able to signal its readiness by calling fork_notify_startup(), then this
 * function returns -1. However, even in case of failure it still sets child
 * process id in '*child_pid'. */
static int
fork_and_wait_for_startup(int *fdp, pid_t *child_pid)
{
    int fds[2];
    pid_t pid;
    int ret = 0;

    xpipe(fds);

    pid = fork_and_clean_up();
    if (pid > 0) {
        /* Running in parent process. */
        size_t bytes_read;
        char c;

        close(fds[1]);
        if (read_fully(fds[0], &c, 1, &bytes_read) != 0) {
            int retval;
            int status;

            do {
                retval = waitpid(pid, &status, 0);
            } while (retval == -1 && errno == EINTR);

            if (retval == pid) {
                if (WIFEXITED(status) && WEXITSTATUS(status)) {
                    /* Child exited with an error.  Convey the same error
                     * to our parent process as a courtesy. */
                    exit(WEXITSTATUS(status));
                } else {
                    char *status_msg = process_status_msg(status);
                    //VLOG_ERR("fork child died before signaling startup (%s)",
                    //         status_msg);
                    printf("fork child died before signaling startup (%s)",
                             status_msg);
                    ret = -1;
                }
            } else if (retval < 0) {
                //VLOG_FATAL("waitpid failed (%s)", ovs_strerror(errno));
                printf("waitpid failed (%d)", errno);
            } else {
                abort();
            }
        }
        close(fds[0]);
        *fdp = -1;
    } else if (!pid) {
        /* Running in child process. */
        close(fds[0]);
        *fdp = fds[1];
    }
    *child_pid = pid;
    return ret;
}

int write_fully(int fd, const void *p_, size_t size, size_t *bytes_written)
{
    const uint8_t *p = p_;

    *bytes_written = 0;
    while (size > 0) {
        ssize_t retval = write(fd, p, size);
        if (retval > 0) {
            *bytes_written += retval;
            size -= retval;
            p += retval;
        } else if (retval == 0) {
            printf("write returned 0");
            return EPROTO;
        } else if (errno != EINTR) {
            return errno;
        }
    }
    return 0;
}

static void fork_notify_startup(int fd)
{
    if (fd != -1) {
        size_t bytes_written;
        int error;

        error = write_fully(fd, "", 1, &bytes_written);
        if (error) {
            //VLOG_FATAL("pipe write failed (%s)", ovs_strerror(error));
            printf("pipe write failed (%d)", error);
        }

        close(fd);
    }
}

/* Returns a readable and writable fd for /dev/null, if successful, otherwise
 * a negative errno value.  The caller must not close the returned fd (because
 * the same fd will be handed out to subsequent callers). */
static int get_null_fd(void)
{
    static int null_fd;
#ifndef _WIN32
    char *device = "/dev/null";
#else
    char *device = "nul";
#endif

    if (!null_fd) {
        null_fd = open(device, O_RDWR);
        if (null_fd < 0) {
            int error = errno;
            //VLOG_ERR("could not open %s: %s", device, ovs_strerror(error));
            printf("could not open %s: %d", device, error);
            null_fd = -error;
        }
    }

    return null_fd;
}

/* Close standard file descriptors (except any that the client has requested we
 * leave open by calling daemon_save_fd()).  If we're started from e.g. an SSH
 * session, then this keeps us from holding that session open artificially. */
void close_standard_fds(void)
{
    int null_fd = get_null_fd();
    if (null_fd >= 0) {
        int fd;

        for (fd = 0; fd < 3; fd++) {
            if (!save_fds[fd]) {
                dup2(null_fd, fd);
            }
        }
    }

    /* Disable logging to stderr to avoid wasting CPU time. */
    //vlog_set_levels(NULL, VLF_CONSOLE, VLL_OFF);
}


/* Sets 'subprogram_name' as the name of the currently running thread or
 * process.  (This appears in log messages and may also be visible in system
 * process listings and debuggers.) */
void
set_subprogram_name(const char *subprogram_name)
{
    char *pname = xstrdup(subprogram_name ? subprogram_name : program_name);
    free(subprogram_name_set(pname));

#if HAVE_GLIBC_PTHREAD_SETNAME_NP
    pthread_setname_np(pthread_self(), pname);
#elif HAVE_NETBSD_PTHREAD_SETNAME_NP
    pthread_setname_np(pthread_self(), "%s", pname);
#elif HAVE_PTHREAD_SET_NAME_NP
    pthread_set_name_np(pthread_self(), pname);
#endif
}

static bool should_restart(int status)
{
    if (WIFSIGNALED(status)) {
        static const int error_signals[] = {
            /* This list of signals is documented in daemon.man.  If you
             * change the list, update the documentation too. */
            SIGABRT, SIGALRM, SIGBUS, SIGFPE, SIGILL, SIGPIPE, SIGSEGV,
            SIGXCPU, SIGXFSZ
        };

        size_t i;

        for (i = 0; i < ARRAY_SIZE(error_signals); i++) {
            if (error_signals[i] == WTERMSIG(status)) {
                return true;
            }
        }
    }
    return false;
}

static void monitor_daemon(pid_t daemon_pid)
{
    /* XXX Should log daemon's stderr output at startup time. */
    time_t last_restart;
    char *status_msg;
    int crashes;
    bool child_ready = true;

    set_subprogram_name("monitor");
    status_msg = xstrdup("healthy");
    last_restart = TIME_MIN;
    crashes = 0;
    for (;;) {
        int retval;
        int status;

        //ovs_cmdl_proctitle_set("monitoring pid %lu (%s)",
        //                       (unsigned long int) daemon_pid, status_msg);

        if (child_ready) {
            do {
                retval = waitpid(daemon_pid, &status, 0);
            } while (retval == -1 && errno == EINTR);
            if (retval == -1) {
                //VLOG_FATAL("waitpid failed (%s)", ovs_strerror(errno));
                printf("waitpid failed (%d)", errno);
            }
        }

        if (!child_ready || retval == daemon_pid) {
            char *s = process_status_msg(status);
            if (should_restart(status)) {
                free(status_msg);
                status_msg = xasprintf("%d crashes: pid %lu died, %s",
                                       ++crashes,
                                       (unsigned long int) daemon_pid, s);
                free(s);

                if (WCOREDUMP(status)) {
                    /* Disable further core dumps to save disk space. */
                    struct rlimit r;

                    r.rlim_cur = 0;
                    r.rlim_max = 0;
                    if (setrlimit(RLIMIT_CORE, &r) == -1) {
                        //VLOG_WARN("failed to disable core dumps: %s",
                        //          ovs_strerror(errno));
                        printf("failed to disable core dumps: %s",
                                  ovs_strerror(errno));
                    }
                }

                /* Throttle restarts to no more than once every 10 seconds. */
                if (time(NULL) < last_restart + 10) {
                    //VLOG_WARN("%s, waiting until 10 seconds since last "
                    //          "restart", status_msg);
                    printf("%s, waiting until 10 seconds since last "
                              "restart", status_msg);
                    for (;;) {
                        time_t now = time(NULL);
                        time_t wakeup = last_restart + 10;
                        if (now >= wakeup) {
                            break;
                        }
                        xsleep(wakeup - now);
                    }
                }
                last_restart = time(NULL);

                VLOG_ERR("%s, restarting", status_msg);
                child_ready = !fork_and_wait_for_startup(&daemonize_fd,
                                                         &daemon_pid);
                if (child_ready && !daemon_pid) {
                    /* Child process needs to break out of monitoring
                     * loop. */
                    break;
                }
            } else {
                //VLOG_INFO("pid %lu died, %s, exiting",
                //          (unsigned long int) daemon_pid, s);
                printf("pid %lu died, %s, exiting",
                          (unsigned long int) daemon_pid, s);
                free(s);
                exit(0);
            }
        }
    }
    free(status_msg);

    /* Running in new daemon process. */
    //ovs_cmdl_proctitle_restore();
    set_subprogram_name("");
}



/* If daemonization is configured, then starts daemonization, by forking and
 * returning in the child process.  The parent process hangs around until the
 * child lets it know either that it completed startup successfully (by calling
 * daemon_complete()) or that it failed to start up (by exiting with a nonzero
 * exit code). */
void
daemonize_start(void)
{
    //assert_single_threaded();
    daemonize_fd = -1;

    if (detach) {
        pid_t pid;

        if (fork_and_wait_for_startup(&daemonize_fd, &pid)) {
            //VLOG_FATAL("could not detach from foreground session");
        }
        if (pid > 0) {
            /* Running in parent process. */
            exit(0);
        }

        /* Running in daemon or monitor process. */
        setsid();
    }

    if (monitor) {
        int saved_daemonize_fd = daemonize_fd;
        pid_t daemon_pid;

        if (fork_and_wait_for_startup(&daemonize_fd, &daemon_pid)) {
            //VLOG_FATAL("could not initiate process monitoring");
            printf("could not initiate process monitoring");
        }
        if (daemon_pid > 0) {
            /* Running in monitor process. */
            fork_notify_startup(saved_daemonize_fd);
            close_standard_fds();
            monitor_daemon(daemon_pid);
        }
        /* Running in daemon process. */
    }

    forbid_forking("running in daemon process");

    if (pidfile) {
        make_pidfile();
    }

    /* Make sure that the unixctl commands for vlog get registered in a
     * daemon, even before the first log message. */
    //vlog_init();
}
