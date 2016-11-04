#!/bin/bash

INDEX_BEGIN=1
INDEX_END=3

err() {
    echo "[$(date +'%Y-%m-%dT%H:%M:%S%z')]: $@" >&2
    exit 1
}

function check_iface() {
    local iface=$1
    iface_num=$(grep ${iface} /proc/interrupts | wc -l)
    if [[ $iface_num == 0 ]];then
        err "don't exist interface $iface, check and retry"
    fi
}

function is_multi_queue() {
    local iface=$1
    check_iface $iface
    iface_irq=$(grep "${iface}-" /proc/interrupts | wc -l)
    if [[ $iface_irq == 0 ]]; then
       return 1
    else
       return 0
    fi
}

function get_irq_num() {
    local iface=$1
    check_iface $iface
    iface_irq=$(grep "${iface}-" /proc/interrupts | wc -l)
    if [[ $iface_irq == 0 ]]; then
        iface_irq=$(grep ${iface} /proc/interrupts | wc -l)
    fi
    return $iface_irq
}

function set_hardwareirq() {
    local iface=$1
    check_iface $iface

    local cpu_index_begin=${INDEX_BEGIN:-1};
    local cpu_index_end=${INDEX_END:-3};

    if is_multi_queue $iface; then
        for irq in $(grep "${iface}-\|${iface}" /proc/interrupts | cut  -d:  -f1 | sed "s/ //g");do
            local cpu_index=$((1<<${cpu_index_begin}))
            #echo ${cpu_index}
            printf "%x" ${cpu_index} > /proc/irq/${irq}/smp_affinity;
            cpu_index_begin=$((${cpu_index_begin}+1))
            if [[ ${cpu_index_begin} -eq ${cpu_index_end}+1 ]]; then
                cpu_index_begin=1
            fi
        done
    else
        local irq=$(grep "${iface}-\|${iface}" /proc/interrupts | cut  -d:  -f1 | sed "s/ //g")
        local cpu_index=$((1<<${cpu_index_begin}))
        printf "%x" ${cpu_index} > /proc/irq/${irq}/smp_affinity;
    fi
}


function dump_hardwaredirq() {
    local iface=$1
    check_iface $iface

    echo "hardware irq:"
    for i in $(grep "${iface}-\|${iface}" /proc/interrupts | cut  -d:  -f1 | sed "s/ //g");do
        cat /proc/irq/$i/smp_affinity;
    done
}


function set_flow_cnt() {
    local iface=$1
    check_iface $iface

    local sock_entries=262144
    echo $sock_entries > /proc/sys/net/core/rps_sock_flow_entries
    cat /proc/sys/net/core/rps_sock_flow_entries

    iface_irq=$(grep "${iface}-" /proc/interrupts | wc -l)
    if [[ $iface_irq == 0 ]]; then
        iface_irq=$(grep ${iface} /proc/interrupts | wc -l)
    fi
    rps_flow_cnt=$(($sock_entries/$iface_irq))
    if is_multi_queue $iface;then
        for rx_num in $(seq 0 $(($iface_irq-1))); do
            #echo $rx_num
            echo $rps_flow_cnt > /sys/class/net/$iface/queues/rx-${rx_num}/rps_flow_cnt
        done
        for rx_num in $(seq 0 $(($iface_irq-1))); do
            #echo $rx_num
            cat /sys/class/net/$iface/queues/rx-${rx_num}/rps_flow_cnt
        done
    else
        echo $rps_flow_cnt > /sys/class/net/$iface/queues/rx-0/rps_flow_cnt
    fi
}

function set_softirq() {
    local iface=$1
    check_iface $iface

    set_flow_cnt $iface

    local cpu_index_begin=${INDEX_BEGIN:-1};
    local cpu_index_end=${INDEX_END:-3};
    for rx_queue in /sys/class/net/${iface}/queues/rx-*; do
        local cpu_index=$((1<<${cpu_index_begin}))
        #echo ${cpu_index}
        printf "%x" $cpu_index > ${rx_queue}/rps_cpus
        cpu_index_begin=$((${cpu_index_begin}+1))
        if [[ ${cpu_index_begin} -eq ${cpu_index_end}+1 ]]; then
            cpu_index_begin=1
        fi
    done
}

function dump_softirq() {
    local iface=$1
    check_iface $iface

    echo "soft irq:"
    for rx_queue in /sys/class/net/$iface/queues/rx-*; do
        cat ${rx_queue}/rps_cpus
    done
}

function main() {
    set_softirq "p5p1"
    dump_softirq "p5p1"
    set_hardwareirq "p5p1"
    dump_hardwaredirq "p5p1"
}

main "$@"
