# IPC
inter process communication

## 常用命令
```sh
ipcs -a  # 输出系统中所有的进程间通讯方式
ipcs -m  # 输出共享内存信息
ipcs -q  # 输出消息队列信息
ipcs -s  # 输出信号信息

ipcs -t # 输出详细时间
ipcs -p # 输出进程id
ipcs -c # 输出创建者/拥有者
ipcs -l # 输出系统限制
ipcs -u # 输出ipc状态

ipcrm -M shmkey # 移出shmkey创建的共享内存
ipcrm -m shmid  # 移出shmid标识的共享内存
ipcrm -Q msgkey # 移出msgkey创建的消息队列
ipcrm -q msgid  # 移出msgid标识的消息队列
ipcrm -S semkey # 移出semkey创建的信号
ipcrm -s semid  # 移出semid标识的信号
```