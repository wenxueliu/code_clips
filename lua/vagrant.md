[原文](http://tchen.me/posts/2013-04-25-engineering-environment-for-smart-team.html)
[改进](http://zhuanlan.zhihu.com/prattle/19693311)
[例子](monnand.me/p/vagrant-docker-coreos/zhCN/)[docker 入门]（http://segmentfault.com/a/1190000000366923）
(http://segmentfault.com/a/1190000000366923）
糟糕的开发环境
========================
1. 开发人员的环境并不统一：dev在osx，ubuntu 11.10和ubuntu 12.04上工作，而ux在windows下工作，协调，解决问题不太方便，尤其是一个bug在A的系统出现，却在自己的环境下无法复用。
    
2. 无法即刻搭建和线上同版本的环境，解决线上问题。小团队节奏很快，当前的工作目录可能和线上版本差几天的代码（diff可能已经是巨量），所以当线上出问题时，將工作环境切换过去非常耗时（尤其是数据库发生变化时）。
    
3. 为新员工构建开发环境耗时且问题重重。这和第一点有些类似，即在ubuntu下工作的构建开发环境的流程在osx下会break。尤其是后期我们不断有开发人员转换系统到osx下。

理想的开发环境
========================

1. 编辑环境和运行/测试环境分离。这意味着开发人员，不管是dev还是ux，可以使用任何她喜欢的系统进行内容的编辑，而其产出可以无缝地运行在另一个统一的环境。无缝是很重要的体验，如果分离意味着在两个系统显示地频繁切换，那还不如不分离；在此基础上的统一的环境则让大家在同一个上下文中交流。

2. 开发人员可以同时工作在好几个版本下。假设以两周为周期，一周开发，一周测试，然后就上线，这意味着在任何一周，开发人员同时工作在3个不同的branch上，以week 3第一天为例：Dalian已经部署到线上，Edingburgh交付测试，而Florence正在开发中。开发人员能够无痛地在这三个环境中任意切换，就像任务调度一样，保存上下文，切换到另一个branch，开始工作。作为小团队，我们不希望甚至不可能将有限的人员切成三份来运作，所以应该通过工具支持这种开发状态。

3. 能很好地支持持续集成。travis-ci.org 跑跑开源项目还可以，但商业项目就免了，而且其每次构建都rebuild整套环境这个效率太低。


vagrant 模型
========================

在这个模型中，大家交流的基础是虚拟机。虚拟机随时被创建，随时又销毁，有一个box服务器 vagrant repo 来统一存储所有box并提供上传/下载服务。box服务器提供两类box：

    基准box。每天半夜从github pull相应branch的代码，并辅以对应的database，自动打包成一个基准box。每个活跃的branch每天都会有一个新的基准box。基准box保存一周足矣。大家新的一天工作的基础是基准box。
    PR box。解决线上问题时，和QA，CI交流使用的box。

任意一个box都是一个沙箱，它包含和线上环境同版本的操作系统，运行环境。同时里面有对应branch的代码库和数据库。数据库采用线上数据库的一个子集，可以让系统正常运行即可。box和用户的host OS间可以共享目录，比如说代码的目录，这样可以让用户通过host OS上的个性化编辑环境撰写代码。此外，box里的port和host OS的port要能一一映射，这样用户完全具有本地的测试体验。

适用场景
========================
这样的开发环境能满足本地办公团队，甚至远程办公团队的需要。

    同一个地点办公的团队，vagrant repo 服务器可以放在本地，以获得最好的下载速度。
    异地办公或者服务于开源项目的松散团队，可以把 bagrant repo 放在一个公网服务器，让参与者都能访问（安全性不在本文讨论）。访问速度的问题可以通过本地缓存来解决，这样在多人下载同一个box时会有近乎本地访问的体验。
    

docker 能做什么？
    使用Linux container，使得"VM"的创建和销毁在秒级就能完成。由于只是做了网络和进程的隔离，"VM"的运行几乎没有overhead。
    使用AUFS，可以以递进的方式创建"VM" —— 一个"VM"叠在另一个"VM"上，就像使用git增量开发一样。
    软件的运行环境（image）和软件本身（container）分离，和数据也分离。

头两点让docker在系统中的footprint很小，使用或者不使用docker对应用程序来说几乎没有差别。最后一点是最关键的，它让你能灵活地基于某个现存的image，和最新的软件版本，最新的线上样本数据一起，构建一个container。

vagrant无法做到这一点。一旦你创建了一个VM，你的环境，应用程序和数据都被绑定到一起了，同一个环境，不同的应用程序版本（或数据），需要创建不同的VM。




vagrant可以帮助用户管理/部署虚拟机的程序。

docker是一个帮助用户创建/运行/管理基于lxc的linux container的程序。

coreos是一个专门为运行linux container而设计的发行版。

虚拟机里运行postegersql
==============================
先说一下这个例子要做什么：我们要利用vagrant，创建并运行一个 virtualbox 虚拟机，并且在虚拟机里运行 coreos 系统，同时，在这个 coreos 的虚拟机里，利用docker 创建一个linux container，里面运行 postgresql。

首先，要安装 vagrant 和virtualbox。具体安装细节我就不说了，都很容易。

接下来，让vagrant添加一个虚拟机镜像：

$ vagrant box add coreos http://storage.core-os.net/coreos/amd64-generic/dev-channel/coreos_production_vagrant.box

这条命令会让vagrant从指定的url下载一个虚拟机镜像文件。上面指定的URL是coreos官方提供的虚拟机镜像。这个镜像（在vagrant里叫box）被命名为coreos。

然后，创建一个基于coreos镜像的虚拟机：

$ mkdir postgresvm
$ vagrant init coreos

第一条命令创建一个单独的文件夹来放这个虚拟机相关的东西，第二条命令创建了一个基于coreos镜像的虚拟机。创建之后，可以在当前文件夹下看到一个名为Vagrantfile的文件。

打开Vagrantfile，找到这一行：config.vm.network :private_network, ip: "192.168.33.10"，去掉这行的注释（就是删除行首的井号）。保存文件。这样，这台虚拟机的IP地址就是192.168.33.10。注意：这个地址只对宿主机可见（运行虚拟机的物理计算机）。如果需要配置别的网络环境，可以参见vagrant的文档。

下面，运行这台虚拟机：

	$ vagrant up
	$ vagrant ssh

第一条命令开启了这个虚拟机，第二条命令通过ssh登录进了这个虚拟机。

前面已经说了，coreos是针对运行docker的container而设计的。所以这台虚拟机里已经安装好了docker。下一步就是安装postgresql。正常说来，就是要apt-get或者yum啥的来安装然后配置什么的。不过利用docker，你可以下载别人创建好的container。什么是container呢？可以把它简单地理解为另一种轻量的虚拟化技术。简单说，一个container就是一个虚拟机。我们想要安装一个postgresql，那么就可以看看有没有人创建好了运行postgresql的container：

	$ sudo docker search postgresql
	Found 25 results matching your query ("postgresql")
	NAME                                   DESCRIPTION
	jpetazzo/pgsql                         A random PostgreSQL service.
	synthomat/pgsql                        PostgreSQL 9.1 Image
	zaiste/postgresql                      PostgreSQL 9.2 - https://gist.github.com/zaiste/5735631
	...

可以看到有25个container包含了postgresql这个关键词。我用的是zaiste/postgresql，下面用docker来下载并安装这个container：

	$ sudo docker pull zaiste/postgresql

安装完之后，我们就可以运行这个 container了。这个container里安装了postgresql，并且运行在5432这个端口上，所以我们需要把container的5432端口映射到本地机（coreos)的5432端口上，同时，运行这个container：

	$ sudo docker run -d -p 5432:5432 zaiste/postgresql

这个命令里，-d告诉docker运行为daemon；-p 5432:5432 告诉docker把本地端口5432留给container的5432端口。运行完这个命令，我们就已经在coreos上运行了postgresql了。

下一步，在宿主机上连接 coreos 里运行的 postgresql。宿主机就是运行着虚拟机的物理主机：

	$ psql -h 192.168.33.10 -p 5432 -d docker -U docker -W

输入密码：docker。这样，就连上了虚拟机中的postgresql。而那个postgresql则是运行在虚拟机里的一个linux container中。
虚拟机里跑container，这不蛋疼吗？

前面说了，container是一种非常轻量级的虚拟化技术，所以所以并不像一般想象中的虚拟机那样吃资源。另外，可以试想这样一个情景：在amazon的ec2上配置一个postgresql环境。要知道，ec2是按小时收费的。如果能把配置环境的步骤简化，那么就相当于为用户省钱。一条docker pull ...就可以把整个环境都下载下来，这样可以很大地节约人力成本和时间成本。而配置几个amazon的ec2服务器，让它们之间协同工作，则可以利用vagrant的配置文件完成。同时，利用coreos里的etcd程序来发现节点间的服务。

其他我就不多说了，三个程序的文档都很全，而且都是自由/开源的。它们的很多功能我的这篇博客都还没涉及到呢。

