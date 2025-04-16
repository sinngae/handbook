# harbor 使用
```shell
#推送之前先登录Harbor
docker login 
docker login registry.test.myop.com 
# > admin 
# > Harbor12345 
# 提示success登录成功 

# 查看自己有哪些镜像;
docker images 

#把需要上传到Harbor的镜像运行如下命令就可以了 
#镜像打标签 
docker tag 镜像名:标签 私服地址/仓库项目名/镜像名:标签 

#推送到私服 
docker push 私服地址/仓库项目名/镜像名：标签 

#从私服拉取镜像 
docker pull 私服地址/仓库项目名/镜像名：标签
```
