# Disk Management

## 扩展VMware虚拟机的磁盘空间(非增加一个硬盘)
扩展VMware虚拟机的磁盘空间，不是增加一个虚拟硬盘。（增加虚拟硬盘很简单，在VM中图形化操作一下即可)
起初分配给虚拟机一定磁盘大小，慢慢地发现空间不够用，需要增加磁盘空间，却又不想分成多磁盘，就需要本段的操作。

### 扩展vmware磁盘文件
+ 1.打开windows终端CMD
+ 2.进入VMware的安装目录（默认X:\ProgramFiles\VMware\VMwareWorkstation）
+ 3.输入命令vmware-vdiskmanager.exe -x 100GB "X:\VM\CentOS7\hjg000.vmdk"

注意：如果虚拟机有还原点，要删除后才能扩展，否则会提示不能扩展(未测试)。虚拟机要处于关机状态。
1）参数"-X"表示要扩展虚拟机硬盘空间；
2）紧随其后的数字是扩展后的磁盘的大小（是指扩展后磁盘总的大小，包含了原先的磁盘容量）；
3）最后一个参数是虚拟机磁盘文件的完整路径（如果路径中有空格，必须用双引号括起来）；
4）只能改大不能改小（未测试）；

+ 4.按回车键开始扩展磁盘

等待命令行窗口会出现"Grow 100 done"，千万不要关闭窗口，此时"X:\VM\CentOS7"目录下会出现一个hjg000.vmdk.dfgshkgrw-tmp的TMP文件，文件大小不断增加，此时程序正在做增加磁盘空间处理，运行完成后，此文件自动删除。
执行完毕，关闭命令提示符窗口，重新启动VMware，会发现虚拟机硬盘空间已变成100GB了。
+ 5.最后还需要，对虚拟机分区进行调整

### 扩展VMWare虚拟机磁盘空间
+ 1.关闭Vmware的Linux系统，在VMWare菜单中设置：VM->Settings...->Hardware->HardDisk->Utilities->Expand。本文假设新增加了100G。
+ 2.对新增加的硬盘进行分区、格式化

上面已经知道增加了空间的硬盘是/dev/sda。分区：
```sh
fdisk /dev/sda		# 操作/dev/sda的分区表
p					# 查看已分区数量（我看到有两个/dev/sda1/dev/sda2）
n					# 新增加一个分区
p					# 分区类型我们选择为主分区
					# 分区号选3（因为1,2已经用过了，见上）
回车				# 默认（起始扇区）
回车				# 默认（结束扇区）
t					# 修改分区类型
					# 选分区3
8e					# 修改为LVM（8e就是LVM）
w					# 写分区表
q					# 完成，退出fdisk命令
```
重启，开机后，格式化：
`mkfs.ext3 /dev/sda3`
+ 3.添加新LVM到已有的LVM组，实现扩容
```sh
lvm												# 进入lvm管理
lvm>pvcreate i /dev/sda3						# 这是初始化刚才的分区
lvm>vgextend vg_dc01 /dev/sda3					# 将初始化过的分区加入到虚拟卷组vg_dc01(mapper/VolGroup-lv_root)
lvm>lvextend -L +29.9G /dev/vg_dc01/lv_root		# 扩展已有卷的容量
lvm>pvdisplay									# 查看卷容量，这时你会看到一个很大的卷了
lvm>quit										# 退出
```
上面那个29.9G怎么来的呢？因为你在VMWare新增加了30G，但这些空间不能全被LVM用了，你可以在上面的lvextend操作中设置为30.1G，就展示为30G了。
以上只是卷扩容了，下面是文件系统的真正扩容，输入以下命令：
`resize2fs /dev/vg_dc01/lv_root`

### 格式化磁盘及开机挂载
```sh
fdisk -l					# 查找新磁盘
# 磁盘分区
fdisk /dev/sdx				# 进入fdisk模式
	n						# 回车，回车，回车
	w

ls -l /dev/|grep sd
mkfs -t ext3 /dev/sdx1a		# 格式化磁盘

# 挂载磁盘
mkdir /data
mount /dev/sdx1 /data

# 设置开机启动挂载
vi /etc/fstab
#	追加 /dev/sdx1 /data ext3 defaults 1 2
```

## 查看挂载点：
```sh
df -h
#	文件系统 容量 已用 可用 已用 %%挂载点
#	/dev/mapper/vg_dc01-lv_root	47G		12G		34G		25%		/
#	tmpfs						504M	88K		504M	1%		/dev/shm
#	/dev/sda1					485M	31M		429M	7%		/boot

df -h			# 查看一下磁盘
du -h -d 1		# 查看各文件夹大小
```

