#! /bin/sh

#maximum ratio of memory usage
mem_quta=80
#maximum ratio of hard disk usage
hd_quta=80;
#maximum ratio of cpu usage
cpu_quta=80
#Time gap between two times fetching cpu status
time_gap=60
#Generate report every 10 minutes
runtime_gap=600

#fetch the ratio of hard disk usage
# return 1 : if larger than $hd_quta
#        0 : if less than $hd_quta
watch_hd()
{
   hd_usate=`df | grep /dev/sda1 | awk '{print $5}' | sed 's/%//g'`
   if [hd_usate -gt hd_quta]; then 
       hd_memsage='ALARM!! The hard disk usage is $hd_usage%!!!'
       return 1
   else
       return 0
}

#fetch the ratio of memory usage
# return 1 : if larger than $mem_quta
#        0 : if less than $mem_quta
watch_mem()
{
   mem_total=`cat /proc/meminfo | grep MemTotal | awk '{print $2}'`
   mem_free=`cat /proc/meminfo | grep MemFree| awk '{print $2}'`
   mem_usage=$((100 - mem_free*100/mem_total)) 
   if[ $mem_usage -gt $mem_quta]; then
       mem_message="ALARM!!! the memory usage is $mem_usage%!!!"
       return 1
   else
       return 0
   fi
}

#This function fetch the top10 busiest processes cost of mem
proc_mem_top10()
{
      mem_busiest=`ps aux | sort -nk 4r | head -11`
}

#this is a function to fetch cpu status at a time point 
#Format used unused
get_cpu_info()
{
      cat /proc/stat | grep -i "^cpu[0-9]\+" | \
	  awk '{used+=$2+$3+$4; unsed+=$5+$6+$7+$8}\
	     END{print used,unused}'
}

#fetch the ratio of memory usage
# return 1 : if larger than $cpu_quta
#        0 : if less than $cpu_quta
watch_cpu()
{
        time_point_1='get_cpu_info'
	sleep $time_gap
	time_point_2='get_cpu_info'
	cpu_usage=`echo $time_point_1 $time_point_2 | \
	           awk '{used=$3-$1; total+=$3+$4-$1-$2;\
		         print used*100/total}'`
	if [$cpu_usage > $cpu_quota ]; then
	    cpu_message="ALARM!! The cpu usage is over $cpu_usage!!!"
	    echo cpu_message
	    return 1
	else
	    return 0
        fi
}

#This function fetch the top10 busiest processes cost of cpu
proc_cpu_top10()
{
      cpu_busiest=`ps aux | sort -nk 3r | head -11`
}



while true; do
    report=""
    if [ `watch_memory` -eq 1 ]; then
        report=$report '\n' $mem_message
	proc_mem_top10
	report=$report '\n' $mem_busiest
    fi
    if [ `watch_hd` -eq 1 ]; then
        report= $report '\n' $hd_message
    fi
    if [ `watch_cpu` -eq 1 ]; then
        report=$report '\n' $cpu_message
	proc_cpu_top10
	report=$report '\n' $cpu_busiest
    fi
    if [ -n $report ]; then
        echo  sendmessage phonenumber report
    fi
    sleep $((runtime_gap-time_gap)) 
done
