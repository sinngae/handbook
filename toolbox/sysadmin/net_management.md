## 开放端口访问
```sh
firewall-cmd --query-port=8000/tcp          # 查看端口是否开放
firewall-cmd --get-active-zones             # 查看开放区域是否是public ens33
firewall-cmd --zone=public --add-port=8000/tcp --permanent  # 设置端口永久开放
firewall-cmd --reload					    # 重启防火墙
firewall-cmd --remove-port=3000/tcp --permanent		        # 关闭端口
firewall-cmd --list-all					    # 检查防火墙规则
```

## 配置host
vim /etc/hosts添加`127.0.0.1 www.baidu.com`
