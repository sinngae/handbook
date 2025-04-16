# archlinux & manjaro

## pacman
```
sudo pacman -Su
pacman -S package_name        # 安装软件  
pacman -S extra/package_name  # 安装不同仓库中的版本
pacman -Syu                   # 升级整个系统，y是更新数据库，yy是强制更新，u是升级软件
pacman -Ss string             # 在包数据库中查询软件
pacman -Si package_name       # 显示软件的详细信息
pacman -Sc                    # 清除软件缓存，即/var/cache/pacman/pkg目录下的文件
pacman -R package_name        # 删除单个软件
pacman -Rs package_name       # 删除指定软件及其没有被其他已安装软件使用的依赖关系
pacman -Qs string             # 查询已安装的软件包
pacman -Qi package_name       # 查询本地安装包的详细信息
pacman -Ql package_name       # 获取已安装软件所包含的文件的列表
pacman -U package.tar.zx      # 从本地文件安装
pactree package_name          # 显示软件的依赖树
```