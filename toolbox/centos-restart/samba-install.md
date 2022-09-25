# samba
```sh
yum install -y samba samba-client
# 启动：systemctl start smb nmb           // NMB对客户端提供NetBIOS服务
# 查看：systemctl status smb nmb

cat /etc/samba/smb.conf.exmaple
# 对应修改/etc/samba/smb.conf主要是添加public

# 防火墙通过
firewall-cmd --permanent --add-service=smb
systemctl restart firewalld.service

# 开机启动
systemctl enable smb.service
systemctl enable nmb.service
```