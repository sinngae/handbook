# git 手册

## 安装
```sh
# 源码安装最新版本
git clone https://github.com/git/git.git
cd git
make prefix=/usr/local/
make install # 安装到/root/bin?

# repo安装
git clone https://gerrit.googlesource.com/git-repo
cp -r git-repo /root/
# 放到环境变量PATH中(略)

# 配置
#   本地仓库配置
git config user.name "zqren"
git config user.email "zqren@foxmail.com"
#   颜色
git config --global color.status auto
git config --global color.diff auto
git config --global color.branch auto
git config --global color.interactive auto

#   https转ssh方式，用于go get/vgo 等需要使用ssh下载代码的时候
#       cat ~/.gitconfig查看
git config --global url.'git@github.com:'.insteadOf 'https://github.com/'
```
## gitconfig
`vim ~/.gitconfig`
```txt
[user]
        email = songam@xxxx.com
        name = songam
[filter "lfs"] # large file storage
        clean = git-lfs clean -- %f
        smudge = git-lfs smudge -- %f
        process = git-lfs filter-process
        required = true
 
[merge]
    summary = true
    tool = vimdiff
[diff]
    renames = copy
[color]
    diff = auto
    status = true
    branch = auto
    interactive = auto
    ui = auto
    log = true
[status]
    submodulesummary = -1
[mergetool "vimdiff"]
    cmd = "vim --noplugin \"$PWD/$MERGED\" \
        +\":split $PWD/$REMOTE\" +\":set buftype=nowrite\" \
        +\":vertical diffsplit $PWD/$LOCAL\" +\":set buftype=nowrite\" \
        +\":vertical diffsplit $PWD/$BASE\" +\":set buftype=nowrite\" \
        +\":wincmd l\""
[format]
    numbered = auto
[alias]
    co = checkout
    ci = commit
    st = status
    pl = pull
    ps = push
    dt = difftool
    l = log --stat
    cp = cherry-pick
    ca = commit -a
    br = branch
```

## 分支说明
+ develop分支为了快速开发  
只要是单元测试、联调测试通过的就可以合入develop分支
+ master分支
+ feature分支
+ bugfix分支

**gitflow**

## git fork
```sh
# 在github网页上需要提交代码的项目上，点击fork，就会在自己的github仓库有个该项目的分裂分支。
# 在本地拉取自己的代码，做修改或创建，提交并推送到自己的仓库
# 在自己的分裂分支上，点击push request，就会在原项目提交一个变动的类似merge request的push request 
```

## git command
```sh
# 查看Git仓库地址
git remote -v

### 2.提交代码
# 新建本地分支
git checkout -b fix_demo origin/develop
# 代码修改，并添加到本地仓库
git add .
# 新建提交到本地分支
git commit -m "commit daily"
# 获取最新的develop分支代码，并解决冲突

git rebase -i [startpoint] [endpoint] # startpoint和endpoint指commit的id
git rebase origin/develop
#   rebase后，只能git push -f origin feature/demo, 再合并到develop？
# 把本地分支推到远程仓库
git push origin fix_demo:fix_demo
# git网页提交merge request，并通知到同事审查代码

# 提交到一个已解决冲突的分支上，然后再提merge request给同事
git push origin fix_demo:fix_demo_MC_fixed

# 把本地分支推送到远程仓库的同名分支（git的current模式（对比git push的其他default行为：nothing, current, upstream, simple, matching））
git push origin HEAD

### 3.查看日志
# 查看多次commit历史
git log
# 查看某次commit的所有变更
git show [commit-hash-id]
# 查看某次commit的文件列表
git show --stat [commit-hash-id]
# 查看多次commit日志及文件改动列表
git log --name-status 

### 4.commit
# 未push的commit备注修改：
git commit --amend //进入修改vim界面，修改备注

### 5.tag
# 创建tag 
git tag v0.1.1 [commit id]
# 删除tag
git tag -d v0.1.1
# 查询tag
git tag
# 推送远程到远程仓库
git push origin --tags

# 拉取远程文件
#   同步远程文件，覆盖本地改动：
git checkout -- filepath

## 分支管理 
# 查看分支:
git branch -a
# 新建本地分支fix_demo
git checkout -b fix_demo # 基于当前分支？
# 重命名分支
git branch -m a b
# 切换到某一分支
git checkout fix_branch
# 推送本地分支到远程仓库
git push --set-upstream origin fix_demo
# 拉取远程仓库的指定分支到本地（本地不存在）（如果拉取不成功，需要现git fetch）
git checkout -b fix_demo2 origin/fix_demo2
# 删除分支
git branch -d fix_demo
# 关联远程分支到本地分支，这样后git pull默认拉取该远程分支
git branch --set-upstream-to origin/develop feature/demo
# 拉取指定分支
git pull origin feature/test:feature/test

## can not merge问题
#   当git提交多次时，后提交的代码先合入，先提交的代码会出现can not merge问题，下面是解决方法。
# 1、先执行命令
git stash  
#   注：此命令是备份当前的工作区，防止当前工程中已修改的代码出现丢失，同时将工作区中的代码保存到git栈中。
# 2、再执行命
git pull
# 3、最后执行命令
git stash pop  
#   注：此命令是从git栈中读取命令1保存的内容，恢复工作区。

## 查看所有的stash保存点
git stash list
## 清除所有的保存点
git stash clear 
## 添加保存点，标记为message
git stash save "message"
## 使用保存点
git stash apply --index

## 查看代码变动者
git blame -L 160,+10 sha1_file.c 
git log -c -S"code to find" /path/to/file ## 被删除的代码也能找到相关的commit id

## 撤销未commit的add
git reset HEAD .            # 撤销所有
git reset HEAD -filename    # 撤销指定文件

## 撤销未push的commit
###git reflog 查看日志
git reset HEAD@{1} 

## 撤销未push的merge conflict
git reset --hard
git clean -n # 提示哪些将被删除
git clean -f # 删除
git clean -f . # 指定删除目录为.

## svn cleanup 本地全clear，与remote同步
git reset --hard
git clean -df

## 仓库管理
# 迁移demo项目的feature/fix-xx-xx分支举例：
# 目标仓库：git@gitlab.sinngae.cn:s_projectx/demo.git
# 源仓库：git@test-gitlab.sinngage.cn:s_projectx/demo.git
git clone git@gitlab.sinngae.cn:s_projectx/demo.git 
# 1.源仓库已存在feature/fix-xx-xx分支:
git checkout origin/feature/fix-xx-xx -b feature/fix-xx-xx
git remote add upstream git@test-gitlab.sinngae.cn:s_projectx/demo.git
git remote update
git rebase upstream/feature/fix-xx-xx
git push origin feature/fix-xx-xx
# 2.源仓库不存在feature/fix-xx-xx分支:
git remote add upstream git@test-gitlab.sinngae.cn:s_projectx/demo.git
git remote update
git checkout upstream/feature/fix-xx-xx -b feature/fix-xx-xx
git push origin feature/fix-xx-xx
# 3.push --mirror
git push --mirror git@gitlab.sinngae.cn:s_projectx/demo.git
cd ..
rm -rf demo
git clone xxx

# 查看commit id所在branch
git branch -r --contains COMMIT_ID

# 本地merge (当前分支：branch/merge/to)
git merge --no-ff "branch/merge/from"

# 查看
git reflog show --date=iso "branch/to/investigate"
```

## git自动化部署
1. develop分支提交自动测试、自动部署到测试环境
2. master分支tag自动部署到生产环境？不，测试环境（或者生产环境的测试节点）
3. release分支？
3. feature分支无自动化任务，鼓励提交代码到仓库，即使代码还未开发完整。

## mind-map
[mind-map](git.png)
(来源于网络)
