## 查找新磁盘
fdisk -l<br>
## 磁盘分区
### 进入fdisk模式
fdisk /dev/sdx<br>
	n<br>
	回车，回车，回车<br>
	w<br>
## ls -l /dev/|grep sdx
## 格式化磁盘
mkfs -t ext3 /dev/sdx1
## 挂载磁盘
mkdir /data<br>
mount /dev/sdx1 /data<br>
## 设置开机启动挂载
vi /etc/fstab<br>
追加 /dev/sdx1 /data ext3 defaults 1 2<br>

