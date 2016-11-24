#!/bin/bash

#git clone git://git.kernel.org/pub/scm/linux/kernel/git/shemminger/iproute2.git
#yum install -y bison flex
#make
#make install

#ip -all netns delete $startwith

function ovs_add_netns() {
    local br=$1
    local port=$2
    local ipv4=$3

    local name=${port}_ns
    local mask=24

    ip netns add ${name}
    ovs-vsctl --may-exist add-br $br

    ovs-vsctl --may-exist add-port ${br} ${port} -- set Interface ${port} type=internal \
        external-ids:iface-id=${port}\
        external-ids:iface-status=active
        #external-ids:attached-mac=${mac}

    ip link set ${port} netns ${name} promisc on up
    ip netns exec ${name} ip link set lo up
    ip netns exec ${name} ip link set ${port} up
    ip netns exec ${name} ip addr add ${ipv4}/${mask} dev ${port}
}

#function netns_add_iface() {
#    local name=$1
#    local dev=$3
#    local ipv4=$4
#    ip netns exec ${name} ip link set ${dev} up
#    ip netns exec ${name} ip addr add ${ipv4}/${mask} dev ${dev}
#    #ip netns exec ${name} ip route add -net default dev ${dev}
#}

function ovs_del_netns() {
    local br=$1
    local port=$2

    local name=${port}_ns
    ip netns delete $name
    ovs-vsctl del-port ${br} ${port}
}

function netns_kill_process() {
    ip netns pids $name | xargs -r kill
}

function netns_exec() {
    port=$1
    shift 1
    cmd=$@
    echo $cmd
    ip netns exec ${port}_ns $cmd
}

function install() {
    ovs-vsctl --may-exist add-br bench_p5p1
    ovs_add_netns bench_p5p1 client110 10.2.1.110
    ovs_add_netns bench_p5p1 client111 10.2.1.111
    ovs_add_netns bench_p5p1 client112 10.2.1.112
    ovs_add_netns bench_p5p1 client113 10.2.1.113

    ovs-vsctl --may-exist add-br bench_p5p2
    ovs_add_netns bench_p5p2 client120 10.2.1.120
    ovs_add_netns bench_p5p2 client121 10.2.1.121
    ovs_add_netns bench_p5p2 client122 10.2.1.122
    ovs_add_netns bench_p5p2 client123 10.2.1.123

    ovs-vsctl --may-exist add-br bench_p5p3
    ovs_add_netns bench_p5p3 client130 10.2.1.130
    ovs_add_netns bench_p5p3 client131 10.2.1.131
    ovs_add_netns bench_p5p3 client132 10.2.1.132
    ovs_add_netns bench_p5p3 client133 10.2.1.133

    ovs-vsctl --may-exist add-br bench_p5p4
    ovs_add_netns bench_p5p4 client140 10.2.1.140
    ovs_add_netns bench_p5p4 client141 10.2.1.141
    ovs_add_netns bench_p5p4 client142 10.2.1.142
    ovs_add_netns bench_p5p4 client143 10.2.1.143
}

function uninstall() {
    ovs_del_netns bench_p5p1 client110
    ovs_del_netns bench_p5p1 client111
    ovs_del_netns bench_p5p1 client112
    ovs_del_netns bench_p5p1 client113
    ovs-vsctl del-br bench_p5p1

    ovs_del_netns bench_p5p2 client120
    ovs_del_netns bench_p5p2 client121
    ovs_del_netns bench_p5p2 client122
    ovs_del_netns bench_p5p2 client123
    ovs-vsctl del-br bench_p5p2

    ovs_del_netns bench_p5p3 client130
    ovs_del_netns bench_p5p3 client131
    ovs_del_netns bench_p5p3 client132
    ovs_del_netns bench_p5p3 client133
    ovs-vsctl del-br bench_p5p3

    ovs_del_netns bench_p5p4 client140
    ovs_del_netns bench_p5p4 client141
    ovs_del_netns bench_p5p4 client142
    ovs_del_netns bench_p5p4 client143
    ovs-vsctl del-br bench_p5p4
}

function show() {
    ovs-vsctl show

    netns_exec client110 ifconfig
    netns_exec client111 ifconfig
    netns_exec client112 ifconfig
    netns_exec client113 ifconfig

    netns_exec client120 ifconfig
    netns_exec client121 ifconfig
    netns_exec client122 ifconfig
    netns_exec client123 ifconfig

    netns_exec client130 ifconfig
    netns_exec client131 ifconfig
    netns_exec client132 ifconfig
    netns_exec client133 ifconfig

    netns_exec client140 ifconfig
    netns_exec client141 ifconfig
    netns_exec client142 ifconfig
    netns_exec client143 ifconfig
}

function pingall() {
    netns_exec client110 ping -c 1 10.2.1.111
    netns_exec client110 ping -c 1 10.2.1.112
    netns_exec client110 ping -c 1 10.2.1.113

    netns_exec client110 ping -c 1 10.2.1.120
    netns_exec client110 ping -c 1 10.2.1.121
    netns_exec client110 ping -c 1 10.2.1.122
    netns_exec client110 ping -c 1 10.2.1.123

    netns_exec client110 ping -c 1 10.2.1.130
    netns_exec client110 ping -c 1 10.2.1.131
    netns_exec client110 ping -c 1 10.2.1.132
    netns_exec client110 ping -c 1 10.2.1.133

    netns_exec client110 ping -c 1 10.2.1.140
    netns_exec client110 ping -c 1 10.2.1.141
    netns_exec client110 ping -c 1 10.2.1.142
    netns_exec client110 ping -c 1 10.2.1.143
}

function usage() {
    cat <<-END >&2
    USAGE: ns_tool [-e exec_mode]
                -e exec_mode                # install, uninstall, show, exec, pingall
                -n namespace		    # namespace only when exec_mode is exec
                -h                          # this usage message
        Example:
           ns_tool -e install
           ns_tool -e show
           ns_tool -e exec client110 ls
           ns_tool -e uninstall
           ns_tool -e pingall

        Dependency:
            curl
END
    exit
}

function main() {
    while getopts e:n:h opt
    do
    	case $opt in
    	e)	exec_mode=$OPTARG ;;
    	n)	net_ns=$OPTARG ;;
    	h|?)	usage ;;
    	esac
    done
    shift $(( $OPTIND - 1 ))
    if [[ $exec_mode == "install" ]];then
        install
    elif [[ $exec_mode == "add" ]];then
        ovs_add_netns $1 $2 $3
    elif [[ $exec_mode == "uninstall" ]];then
        uninstall
    elif [[ $exec_mode == "show" ]];then
        show
    elif [[ $exec_mode == "pingall" ]];then
        pingall
    elif [[ $exec_mode == "exec" ]];then
        if [[ -z $net_ns ]];then
            usage
        fi
        echo "will exec: "$@
        netns_exec $net_ns "$@"
    else
        usage
    fi
}

main "$@"
