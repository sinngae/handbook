#########################################
# File Name: clear_none_image.sh
#		clear docker images and containers which are marked none
# Author: Sinngae
# mail: zqren@foxmail.com
# Created Time: Wed 10 Apr 2019 09:20:38 PM CST
########################################
#!/bin/bash

for i in `docker container list -a|grep Exited|awk '{print $1}'`;
do
	echo $i
	docker rm $i
done

for i in `docker image list|grep none|awk '{print $3}'`;
do
	echo $i
	docker rmi $i
done


