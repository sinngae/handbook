## samba
yum install -y samba samba-client<br>
启动：systemctl start smb nmb           // NMB对客户端提供NetBIOS服务<br>
查看：systemctl status smb nmb<br>

cat /etc/samba/smb.conf.exmaple<br>
对应修改/etc/samba/smb.conf主要是添加public<br>

## 防火墙通过
firewall-cmd --permanent --add-service=smb<br>
systemctl restart firewalld.service<br>

##开机启动
systemctl enable smb.service<br>
systemctl enable nmb.service<br>
