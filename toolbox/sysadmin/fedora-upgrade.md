# fedora-upgrade

同事拷贝了一个fedora 7用，当前版本已经fedora 32了，本想凑合着用，但实在要啥啥没有，就硬着头皮升级了一下系统版本。曲折过程按下不论，只记录通向罗马的大道。
## upgrade-to-fedora9
```sh
yum install preupgrade # preupgrade是redhat-fedora早期的升级工具，现在已不再维护，但稍微改动后仍可用于fedora 7升级到fedora 9

cd /usr/share/preupgrade
vim releases.list
# 注释掉[Fedora 9 (Sulphur)]下mirrorlist和installmirrorlist
# 在其后添加：
#   baseurl=http://ftp.cuhk.edu.hk/pub/linux/fedora-archive/fedora/linux/releases/9/Fedora/i386/os/
# 这个url是从https://admin.fedoraproject.org/mirrormanager/获取的香港某大学仍在维护的镜像服务
# redhat也有http://download-ib01.fedoraproject.org/pub/可以搜索

preupgrade
# 进入图形化界面，选择fedora 9，安装需要等待一段时间后，重启后选择upgrade
```
升级完成后，操作系统确实升级上来了，但是图形化界面不能操作，之前安装的DB2也不能开启启动了，很可能需要重新安装。
也许再修复一下，更新最新软件就好了，但是已经不想扫雷了...
所以升级操作系统版本不是个好选择，因为操作系统之所以升级版本，肯定是因为有些许不兼容，何必死磕这个呢，直接安装最新的。它不好用么，它不香么？
## 附录
### 新增yum的repo
```sh
cd /etc/yum.repo.d
# rpm-repo-yum：服务器上按约定存储文件及配置文件，yum客户端先取配置文件，再取文件。
# repo支持两种mirrorlist和baseurl，前者先拉取baseurl列表，后者拉取配置文件，最后拉取目标文件。
#   [name]  对应/var/cache/yum下的文件夹名
#   name 随意命名
#   $releasever release verion
#   $basearch   base arch
#   mirrorlist
#   baseurl
#   enable
#   gpgcheck    1 - 校验GPG；0 - 否
vim my.repo
# 写入：
#   [fedora]
#   name=Fedora $releasever - $basearch
#   #mirrorlist=https://archsives.fedoraproject.org/pub/archive/fedora/linux/releases/9/Fedora/i386/iso/
#   baseurl=https://archives.fedoraproject.org/pub/archive/fedora/linux/releases/$releasever/Fedora/$basearch/os/
#   enabled=1
#   gpgcheck=0
```
