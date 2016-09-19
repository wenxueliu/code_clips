#! /bin/bash

## redhat 7 最新
#
#echo "------------reinstall yum"
#    echo "------reinstall python-iniparse"
#    wget http://mirrors.163.com/centos/7/os/x86_64/Packages/python-iniparse-0.4-9.el7.noarch.rpm
#    rpm -ivh python-iniparse-0.4-9.el7.noarch.rpm
#
#    echo "------reinstall yum-metadata"
#    wget http://mirrors.163.com/centos/7/os/x86_64/Packages/yum-metadata-parser-1.1.4-10.el7.x86_64.rpm
#    rpm -ivh yum-metadata-parser-1.1.4-10.el7.x86_64.rpm
#
#    echo "------reinstall yum-3.4.3 yum-plugin-fastestmirror"
#    wget http://mirrors.163.com/centos/7/os/x86_64/Packages/yum-3.4.3-125.el7.centos.noarch.rpm
#    wget http://mirrors.163.com/centos/7/os/x86_64/Packages/yum-plugin-fastestmirror-1.1.31-29.el7.noarch.rpm
#
#    rpm -ivh yum-3.4.3-125.el7.centos.noarch.rpm yum-plugin-fastestmirror-1.1.31-29.el7.noarch.rpm
#
#    rm python-iniparse-0.4-9.el7.noarch.rpm  yum-metadata-parser-1.1.4-10.el7.x86_64.rpm  yum-3.4.3-125.el7.centos.noarch.rpm yum-plugin-fastestmirror-1.1.31-29.el7.noarch.rpm


# redhat 7 1406内核版本 uname -r  3.10.0-123.el7.x86_64 (停止更新)

function reinstall_yum() {

    #卸载yum
    rpm -aq | grep yum | xargs rpm -e --nodeps

    base_dir=/tmp
    if cat /etc/redhat-release  | grep -e "Red Hat.*7.0" >> /dev/null
    then
        echo "------reinstall python-iniparse"
        curl http://vault.centos.org/7.0.1406/os/x86_64/Packages/python-iniparse-0.4-9.el7.noarch.rpm \
            -o ${base_dir}/python-iniparse-0.4-9.el7.noarch.rpm

        rpm -ivh ${base_dir}/python-iniparse-0.4-9.el7.noarch.rpm

        echo "------reinstall yum-metadata"
        curl http://vault.centos.org/7.0.1406/os/x86_64/Packages/yum-metadata-parser-1.1.4-10.el7.x86_64.rpm \
            -o ${base_dir}/yum-metadata-parser-1.1.4-10.el7.x86_64.rpm

        rpm -ivh ${base_dir}/yum-metadata-parser-1.1.4-10.el7.x86_64.rpm


        echo "------reinstall yum-3.4.3 yum-plugin-fastestmirror"
        curl http://vault.centos.org/7.0.1406/os/x86_64/Packages/yum-3.4.3-118.el7.centos.noarch.rpm \
            -o ${base_dir}/yum-3.4.3-118.el7.centos.noarch.rpm

        curl http://vault.centos.org/7.0.1406/os/x86_64/Packages/yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm \
            -o ${base_dir}/yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm

        rpm -ivh ${base_dir}/yum-3.4.3-118.el7.centos.noarch.rpm yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm

        rm ${base_dir}/yum-3.4.3-118.el7.centos.noarch.rpm yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm yum-metadata-parser-1.1.4-10.el7.x86_64.rpm python-iniparse-0.4-9.el7.noarch.rpm
    elif cat /etc/redhat-release | grep -e "CentOS.*6\." >> /dev/null
    then
        #redhat 6
        echo "------------reinstall yum"
        echo "------reinstall python-iniparse"
        wget -c http://mirrors.163.com/centos/6/os/x86_64/Packages/python-iniparse-0.3.1-2.1.el6.noarch.rpm

        rpm -ivh python-iniparse-0.3.1-2.1.el6.noarch.rpm

        echo "------reinstall yum-metadata"
        wget -c http://mirrors.163.com/centos/6/os/x86_64/Packages/yum-metadata-parser-1.1.2-16.el6.x86_64.rpm
        rpm -ivh yum-metadata-parser-1.1.2-16.el6.x86_64.rpm

        echo "------reinstall yum-3.2.29-69 and yum-plugin-fastestmirror"
        wget -c http://mirrors.163.com/centos/6/os/x86_64/Packages/yum-3.2.29-69.el6.centos.noarch.rpm
        wget -c http://mirrors.163.com/centos/6/os/x86_64/Packages/yum-plugin-fastestmirror-1.1.30-30.el6.noarch.rpm
        rpm -ivh yum-3.2.29-69.el6.centos.noarch.rpm yum-plugin-fastestmirror-1.1.30-30.el6.noarch.rpm
        rm python-iniparse-0.3.1-2.1.el6.noarch.rpm yum-metadata-parser-1.1.2-16.el6.x86_64.rpm yum-3.2.29-69.el6.centos.noarch.rpm yum-plugin-fastestmirror-1.1.30-30.el6.noarch.rpm

    else
        echo "operate system with `cat /etc/redhat-release` doesn't support now"
    fi
}

function backup_yum() {
    echo "------------back up old yum repo"
    mkdir -p /etc/yum.repos.d/backup
    mv /etc/yum.repos.d/*.repo /etc/yum.repos.d/backup
}

function install_163mirrors() {
    if cat /etc/redhat-release  | grep -e "Red Hat.*7\." >> /dev/null
    then
        echo "------------configure CentOS7-Base-163.repo"
        curl http://mirrors.163.com/.help/CentOS7-Base-163.repo -o  /etc/yum.repos.d/CentOS7-Base-163.repo
        rpm --import http://mirrors.163.com/centos/RPM-GPG-KEY-CentOS-7
        sed -i "s/\$releasever/7/" /etc/yum.repos.d/CentOS7-Base-163.repo
        yum clean all
        yum makecache

    elif cat /etc/redhat-release | grep -e "CentOS.*6\." >> /dev/null
    then
        echo "------------configure CentOS6-Base-163.repo"
        curl http://mirrors.163.com/.help/CentOS6-Base-163.repo -o /etc/yum.repos.d/CentOS6-Base-163.repo
        rpm --import http://mirrors.163.com/centos/RPM-GPG-KEY-CentOS-6
        sed -i "s/\$releasever/6/" /etc/yum.repos.d/CentOS6-Base-163.repo
        yum clean all
        yum makecache
    else
        echo "operate system with `cat /etc/redhat-release` doesn't support now"
    fi
}

function install_epel() {
    echo "------------configure epel-release"
    yum -y remove epel-release
    yum -y install epel-release
    sed -i "s/#baseurl/baseurl/" /etc/yum.repos.d/epel.repo
    sed -i "s/mirrorlist/#mirrorlist/" /etc/yum.repos.d/epel.repo

    yum clean all
    yum makecache
}


# 3 配置 163源(存在对应repo,则不配置)

function main() {
    reinstall_yum

    backup_yum

    install_163mirrors

    install_epel

