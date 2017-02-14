#! /bin/bash

###########################################
# author   : wenxueliu
# date     : 2017-02-14
# comment : TODO merge 4k and 100 byte per line then write
###########################################


function usage() {
    echo "$0 LENGH(byte) NEW_FILE"
    exit 1
}

if [[ $# != 2 ]]; then
    usage
fi

len=$1
file=$2

if [[ $len -lt 0 ]]; then
    usage
fi

if [[ -f $file ]];then
    echo "$file has exist"
    exit 1
fi

ONCE_WRITE=4096

function gen_random() {
    if [[ $# != 1 ]];then
       echo "generate tool must be given"
       exit 1
    fi
    tool=$1
    # less than 30 bytes
    if [[ ${len} -lt 30 ]]; then
        head -n 2 /dev/urandom | ${tool} | sed 's/\s//' | head -c ${len} >> ${file}
    else
        quotient=$((${len}/30))
        remainder=$((${len}%30)) 
        lines=$((${quotient}/3)) # per line 90 bytes
        lastline=$((${quotient}%3))
        if [[ ${lines} -lt 1 ]]; then
            # less than 90 bytes
            local lastwrite=""
            for j in `seq 1 ${quotient}`; do
                local tmp_str=`head -n 2 /dev/urandom | $tool | head -c 30`
                lastwrite="${lastwrite}${tmp_str}"
            done
            if [[ ${remainder} -gt 0 ]]; then
                local tmp_str=`head -n 2 /dev/urandom | $tool | head -c ${remainder}`
                lastwrite="${lastwrite}${tmp_str}"
            fi
            echo -n -e ${lastwrite} >> ${file}
        else
            # more than 90 bytes
            local once_write=""
            for i in `seq 1 ${lines}`; do
                # merge 3*30 bytes as one line
                local line=""
                for j in `seq 1 3`; do
                    local tmp_str=`head -n 2 /dev/urandom | $tool | head -c 30`
                    line="${line}${tmp_str}"
                done
                line="${line}\n"
                line=${line:1}

                once_write="${once_write}${line}"
                # write when str more than ONCE_WRITE bytes
                if [[ ${#once_write} -ge ${ONCE_WRITE} ]];then
                    echo -n -e ${once_write} >> ${file}
                    once_write=""
                fi
            done
            # write the remainder bytes less than ONCE_WRITE
            echo -n -e ${once_write} >> ${file}
            once_write=""

            # last line less than 90 bytes
            local lastwrite=""
            if [[ ${lastline} -gt 0 ]];then
                for k in `seq 1 $lastline`; do
                    local tmp_str=`head -n 2 /dev/urandom | $tool | head -c 30`
                    lastwrite="${lastwrite}${tmp_str}"
                done
            fi
            if [[ ${remainder} -gt 0 ]]; then
                local tmp_str=`head -n 2 /dev/urandom | $tool | head -c $remainder`
                lastwrite="${lastwrite}${tmp_str}"
            fi
            echo -n -e "${lastwrite}" >> ${file}
        fi
    fi
}

function gen_strings() {
    gen_random md5sum
}

function gen_nums() {
    gen_random cksum
}

gen_strings
