#! /bin/bash

#其他源

#卸载yum
rpm -aq | grep yum | xargs rpm -e --nodeps

#1 重新安装yum
# redhat 6 #echo "------------reinstall yum"
#    echo "------reinstall python-iniparse"
#    wget http://mirrors.163.com/centos/6/os/x86_64/Packages/python-iniparse-0.3.1-2.1.el6.noarch.rpm
#
#    rpm -ivh python-iniparse-0.3.1-2.1.el6.noarch.rpm
#
#    echo "------reinstall yum-metadata"
#    wget http://mirrors.163.com/centos/6/os/x86_64/Packages/yum-metadata-parser-1.1.2-16.el6.x86_64.rpm
#    rpm -ivh yum-metadata-parser-1.1.2-16.el6.x86_64.rpm
#
#    echo "------reinstall yum-3.2.29-69 and yum-plugin-fastestmirror"
#    wget http://mirrors.163.com/centos/6/os/x86_64/Packages/yum-3.2.29-69.el6.centos.noarch.rpm
#    wget http://mirrors.163.com/centos/6/os/x86_64/Packages/yum-plugin-fastestmirror-1.1.30-30.el6.noarch.rpm
#    rpm -ivh yum-3.2.29-69.el6.centos.noarch.rpm yum-plugin-fastestmirror-1.1.30-30.el6.noarch.rpm
#    rm python-iniparse-0.3.1-2.1.el6.noarch.rpm yum-metadata-parser-1.1.2-16.el6.x86_64.rpm yum-3.2.29-69.el6.centos.noarch.rpm yum-plugin-fastestmirror-1.1.30-30.el6.noarch.rpm


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
    echo "------reinstall python-iniparse"
    wget http://vault.centos.org/7.0.1406/os/x86_64/Packages/python-iniparse-0.4-9.el7.noarch.rpm
    rpm -ivh python-iniparse-0.4-9.el7.noarch.rpm

    echo "------reinstall yum-metadata"
    wget http://vault.centos.org/7.0.1406/os/x86_64/Packages/yum-metadata-parser-1.1.4-10.el7.x86_64.rpm
    rpm -ivh yum-metadata-parser-1.1.4-10.el7.x86_64.rpm


    echo "------reinstall yum-3.4.3 yum-plugin-fastestmirror"
    wget http://vault.centos.org/7.0.1406/os/x86_64/Packages/yum-3.4.3-118.el7.centos.noarch.rpm

    wget http://vault.centos.org/7.0.1406/os/x86_64/Packages/yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm

    rpm -ivh yum-3.4.3-118.el7.centos.noarch.rpm yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm

    rm yum-3.4.3-118.el7.centos.noarch.rpm yum-plugin-fastestmirror-1.1.31-24.el7.noarch.rpm yum-metadata-parser-1.1.4-10.el7.x86_64.rpm python-iniparse-0.4-9.el7.noarch.rpm


# 2 备份源 (NOTE:所有源repo文件备份到backup文件夹，所以从新配置所有源repo)
echo "------------back up old yum repo"
cd /etc/yum.repos.d/
mkdir -p backup
mv /etc/yum.repos.d/*.repo ./backup

# 3 配置 163源(存在对应repo,则不配置)
#centos 6
#echo "------------configure CentOS6-Base-163.repo"
#wget http://mirrors.163.com/.help/CentOS6-Base-163.repo
#rpm --import http://mirrors.163.com/centos/RPM-GPG-KEY-CentOS-6
#sed -i "s/\$releasever/6/" CentOS6-Base-163.repo
#yum clean all
#yum makecache

#centos 7

echo "------------configure CentOS6-Base-163.repo"
wget http://mirrors.163.com/.help/CentOS7-Base-163.repo
rpm --import http://mirrors.163.com/centos/RPM-GPG-KEY-CentOS-7
sed -i "s/\$releasever/7/" CentOS7-Base-163.repo    #修改 字符串 与centos 版本一致
yum clean all
yum makecache


# 4 配置epel源(无论是否已经安装epel,都重新卸载再安装)
echo "------------configure epel-release"
yum -y remove epel-release
yum -y install epel-release
sed -i "s/#baseurl/baseurl/" /etc/yum.repos.d/epel.repo
sed -i "s/mirrorlist/#mirrorlist/" /etc/yum.repos.d/epel.repo

# 缓存源软件列表
yum makecache



