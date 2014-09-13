#! /bin/sh

while true; do
       echo -n "Do you want to attach to a tmux session? [y/n]"
       read yn
       case $yn in
           [Yy]* ) MY_SSH_CONNECTION="yes" tmux attach; break;;
           [Nn]* ) break;;
            * ) echo "Please answer y/n";;
       esac
   done

exit 0

DEBUG=${DEBUG:=0}

DEBUG=1

function dbg()
{
	[ "x$DEBUG" = "x0" ] || echo "$*" >&2;
}


function test_header()
{
	dbg "=========================";
	dbg "TEST $t (line $TESTLINE): $*";
}

test_header

#==========================================================
#if you want a daemon in linux, this may be a good example.
# just replace the SERVER_NAME and SERVER_DIR

#! /bin/sh
### BEGIN INIT INFO
# Provides:          $SERVER_NAME
                     $SERVER_DIR
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: neutron-server
# Description:       From the Openstack Neutron server

### END INIT INFO

function server()
{
set -e

PIDFILE=/var/run/$SERVER_DIR/$SERVER_NAME.pid
LOGFILE=/var/log/$SERVER_DIR/$SERVER_NAME.log

DAEMON=/usr/bin/$SERVER_NAME
DAEMON_ARGS="--log-file=$LOGFILE"
DAEMON_DIR=/var/run

ENABLED=true

if test -f /etc/default/$SERVER_NAME; then
  . /etc/default/$SERVER_NAME
fi

mkdir -p /var/run/$SERVER_DIR
mkdir -p /var/log/$SERVER_DIR

. /lib/lsb/init-functions

export PATH="${PATH:+$PATH:}/usr/sbin:/sbin"
export TMPDIR=/var/lib/$SERVER_DIR/tmp

if [ ! -x ${DAEMON} ] ; then
    exit 0
fi

case "$1" in
  start)
    test "$ENABLED" = "true" || exit 0
    log_daemon_msg "Starting $SERVER_DIR server" "$SERVER_NAME"
    start-stop-daemon -Sbmv --pidfile $PIDFILE --chdir $DAEMON_DIR --exec $DAEMON -- $DAEMON_ARGS
    log_end_msg $?
    ;;
  stop)
    test "$ENABLED" = "true" || exit 0
    log_daemon_msg "Stopping $SERVER_DIR server" "$SERVER_NAME"
    start-stop-daemon --stop --oknodo --pidfile ${PIDFILE}
    log_end_msg $?
    ;;
  restart|force-reload)
    test "$ENABLED" = "true" || exit 1
    $0 stop
    sleep 1
    $0 start
    ;;
  status)
    test "$ENABLED" = "true" || exit 0
    status_of_proc -p $PIDFILE $DAEMON neutron-server && exit 0 || exit $?
    ;;
  *)
    log_action_msg "Usage: /etc/init.d/$SERVER_NAME {start|stop|restart|force-reload|status}"
    exit 1
    ;;
esac

exit 0
}
#==================================================================

#==================================================================
function yes_or_no()
{
while true; do
       echo -n "Do you want to attach to a tmux session? [y/n]"
       read yn
       case $yn in
           [Yy]* ) echo "you press yes"; break;;
           [Nn]* ) break;;
            * ) echo "Please answer y/n";;
       esac
   done
}

#==================================================================

在远程机器上运行一段脚本

    ssh user@server bash < /path/to/local/script.sh

比较一个远程文件和一个本地文件

    ssh user@host cat /path/to/remotefile | diff /path/to/localfile -

vim一个远程文件

    vim scp://username@host//path/to/somefile

一句话实现一个HTTP服务，把当前目录设为HTTP服务目录，可以通过http://localhost:8000访问。

    python -m SimpleHTTPServer 


查看最常用的十条命令 

    history | awk '{CMD[$2]++;count++;} END { for (a in CMD )print CMD[a] " " CMD[a]/count*100 "% " a }' | grep -v "./" | column -c3 -s " " -t | sort -nr | nl | head -n10
