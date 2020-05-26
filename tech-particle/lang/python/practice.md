# python实战

# `__main__`
```
if __name__ == '__main__' 
 result = main()
```

`__name__`是当前模块名，当模块被直接运行时模块名为`__main__`。这句话的意思就是，当模块被直接运行时，以下代码块将被运行，当模块是被导入时，代码块不被运行。

## Django
Python Web框架，MTV模式

### `'NoneType' object has no attribute 'strip'`
安装的mysql@8.0，创建表test，使用django1.xx访问，报错
```
AttributeError at /testdb/
'NoneType' object has no attribute 'strip'
```
重新安装mysql@5.7，使用`python manage.py migrage model`创建表，创建的表名为model_test，访问没有报错。
误以为是Django和mysql@8.0有问题，实际对django建表部署，另该报错具有误导性。

### `'module' object has no attribute 'ValidationError'`
import jsonschema没有报错，type(jsonschema.validate)却报错找不到定义。
原因是因为当前目录有个文件夹名为jsonschema，优先与site-package里的jsonschema被import。
可以`print jsonschema.__file__`查看当前import的包的路径