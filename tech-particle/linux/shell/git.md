# git 手册
## git安装
源码安装最新版本
···
git clone https://github.com/git/git.git
cd git
make prefix=/usr/local/
make install // 安装到/root/bin?
···

## repo安装
git clone https://gerrit.googlesource.com/git-repo
cp -r git-repo /root/
放到环境变量PATH中

## 分支说明
+ develop分支为了快速开发  
只要是单元测试、联调测试通过的就可以合入develop分支
+ master分支
+ feature分支
+ bugfix分支

## 本地仓库配置
git config user.name "zqren"
git config user.email "zqren@foxmail.com"

## 查看Git仓库地址
git remote -v

## 提交代码
+ 1.新建本地分支 git checkout -b fix_demo origin/develop
+ 2.代码修改，并添加到本地仓库 git add .
+ 3.新建提交到本地分支 git commit -m "commit daily"
+ 4.获取最新的develop分支代码，并解决冲突
<del>git rebase -i [startpoint] [endpoint]
// startpoint和endpoint指commit的id
</del>
git rebase origin/develop <br>
rebase后，只能git push -f origin feature/demo, 再合并到develop？
+ 5.把本地分支推到远程仓库 git push origin fix_demo:fix_demo
+ 6.git网页提交merge request，并通知到同事审查代码

## 查看日志
+ 查看多次commit历史
git log
+ 查看某次commit的所有变更
git show [commit-hash-id]
+ 查看某次commit的文件列表
git show --stat [commit-hash-id]
+ 查看多次commit日志及文件改动列表
git log --name-status 

## 未push的commit备注修改：
git commit --amend //进入修改vim界面，修改备注

## 拉取远程文件
同步远程文件，覆盖本地改动：
git checkout -- filepath

## 分支管理 
+ 查看分支: git branch -a
+ 新建本地分支fix_demo: git checkout -b fix_demo
+ 重命名分支: git branch -m a b
+ 切换到某一分支: git checkout fix_branch
+ 推送本地分支到远程仓库: git push --set-upstream origin fix_demo
+ 拉取远程仓库的指定分支到本地（本地不存在）（如果拉取不成功，需要现git fetch）: git checkout -b fix_demo2 origin/fix_demo2
+ 删除分支: git branch -d fix_demo
+ 关联远程分支到本地分支，这样后git pull可以拉取该远程分支:

git branch --set-upstream-to origin/develop feature/demo

git pull origin feature/test:feature/test

## can not merge问题
当git提交多次时，后提交的代码先合入，先提交的代码会出现can not merge问题，下面是解决方法。
+ 1、先执行命令：git stash  
注：此命令是备份当前的工作区，防止当前工程中已修改的代码出现丢失，同时将工作区中的代码保存到git栈中。
+ 2、再执行命令：git pull
+ 3、最后执行命令：git stash pop  
注：此命令是从git栈中读取命令1保存的内容，恢复工作区。

## 查看代码变动者
git blame -L 160,+10 sha1_file.c 


## 撤销
+ 撤销未commit的add：
```
git reset HEAD .
git reset HEAD -filename
```
+ 撤销未push的commit

## svn cleanup =
```
git reset --hard
git clean -df
```

## 仓库管理
迁移demo项目的feature/fix-xx-xx分支举例：
目标仓库：git@gitlab.sinngae.cn:s_projectx/demo.git
源仓库：git@test-gitlab.sinngage.cn:s_projectx/demo.git
git clone git@gitlab.sinngae.cn:xl_onecloud_firmware/demo.git 
1.源仓库已存在feature/fix-xx-xx分支:
```
git checkout origin/feature/fix-xx-xx -b feature/fix-xx-xx
git remote add upstream git@test-gitlab.sinngae.cn:s_projectx/demo.git
git remote update
git rebase upstream/feature/fix-xx-xx
git push origin feature/fix-xx-xx
```
2.源仓库不存在feature/fix-xx-xx分支:
```
git remote add upstream git@test-gitlab.sinngae.cn:s_projectx/demo.git
git remote update
git checkout upstream/feature/fix-xx-xx -b feature/fix-xx-xx
git push origin feature/fix-xx-xx
```
3.push --mirror
git push --mirror git @gitlab.sinngae.cn:s_projectx/demo.git
cd ..
rm -rf demo
git clone xxx

