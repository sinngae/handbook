# virtualenv
virtualenv用于隔离不同的python运行时依赖包

## 安装
```sh
pip3 install virtualenv	# 安装virtualenv
pip3 list				# 查看pip3安装的包，包括virtualenv

mkdir mypy
cd mypy
virtualenv venv			# 创建python运行时依赖环境，命名为venv，最新python版本，还是本地最新python
virtualenv venv2.7 --python=python2.7

source venv/bin/activate	# 激活venv环境，即进入venv环境
deactive				# 退出venv环境

rm -rf venv				# 删除venv环境

pip3 install virtualenvwrapper	# 安装virtualenvwrapper
# virtualenvwrapper把python运行依赖环境管理在~/.virtualenv，不用制定工程目录
```
