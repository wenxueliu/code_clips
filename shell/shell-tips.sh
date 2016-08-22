#! /bin/sh

#使用$*很少是正确的选择。
#$@能够处理空格参数，而且参数间的空格也能正确的处理。
#使用$@时应该用双引号括起来，像”$@”这样。

#对脚本进行语法检查：
#bash -n myscript.sh
#跟踪脚本里每个命令的执行：
#bash -v myscripts.sh
#跟踪脚本里每个命令的执行并附加扩充信息：
#bash -x myscript.sh

#引用未定义的变量(缺省值为"")
#执行失败的命令被忽略
set -o nounset
set -o errexit
set -o verbose
set -o xtrace

#==========================================================

#如果变量可以是 local 或 readonly 的尽量声明为 local 和 readonly
readonly x=3
x=4

#==========================================================

#用$()代替反单引号(`)`)
# both commands below print out: A-B-C-D
echo "A-`echo B-\`echo C-\\\`echo D\\\`\``"
echo "A-$(echo B-$(echo C-$(echo D)))"

#==========================================================

#用[[]](双层中括号)替代[]
#|| 	逻辑or(仅双中括号里使用)
#&& 	逻辑and(仅双中括号里使用)
#< 	    字符串比较(双中括号里不需要转移)
#-lt 	数字比较
#= 	    字符串相等
#== 	以Globbing方式进行字符串比较(仅双中括号里使用，参考下文)
#=~ 	用正则表达式进行字符串比较(仅双中括号里使用，参考下文)
#-n 	非空字符串
#-z 	空字符串
#-eq 	数字相等
#-ne 	数字不等

[ "${name}" \> "a" -o ${name} \< "m" ]
[[ "${name}" > "a" && "${name}" < "m"  ]]

t="abc123"
[[ "$t" == abc* ]]         # true (globbing比较)
[[ "$t" == "abc*" ]]       # false (字面比较)
[[ "$t" =~ [abc]+[123]+ ]] # true (正则表达式比较)
[[ "$t" =~ "abc*" ]]       # false (字面比较)

#==========================================================

# 下载并比较两个网页
diff <(wget -O - url1) <(wget -O - url2)

#==========================================================

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

kernal_version=`uname -r`
machine=`uname -m`

if [[ $UID != 0 ]]
then
    INFO " This script neeeds to be run with sudo, like this:"
    echo -e "\n  sudo $0 $*\n"
    exit 1
fi

if [[ -f /etc/debian_version ]]
then
    OS="Debian"
elif [[ -f /etc/redhat-release ]]
then
    OS="RedHat"
else
    OS="UnKnow"
fi

if grep "release 6" /etc/redhat-release >> /dev/null;
then
    OS_VERSION=7
elif grep "release 6" /etc/redhat-release >> /dev/null;
    OS_VERSION=6
else
    OS_VERSION="Unknow"
fi

RELEASE=`rpm -q --whatprovides "redhat-release"`
OS_VERSION=`rpm -q --qf "%{version}" $pkg`

############################################################

时间截转时间

date -d@1234567890

实时查看本机网络服务的活动状态。

lsof –i

后台运行一段不终止的程序，并可以随时查看它的状态

screen -d -m -S some_name ping my_router

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

内容去重

awk '!_[$0]++{print}' file

测试网络上合适 MTU

Linux: ping -M do -s <size> <host>

MAC: ping -D -s <size> <host>

Windows: ing -f -l <sizela <host>

====================================================================

但是最近发现一个问题，当要查找的字符串是Unicode编码的时候，这种方式只能查找ansi的字符串，遇到Unicode的字符串查找不到 替换为如下方式查找

   find . |xargs strings -e l -f |grep "文字"
