# ELK
基于ELK实时日志分析的最佳实践

## 安装&使用
```sh
# CentOS 7 环境
# es需要java运行时环境，默认安装java 1.8
yum -y install java

# rpm 安装 
wget https://artifacts.elastic.co/downloads/elasticsearch/elasticsearch-7.6.1.rpm
wget https://artifacts.elastic.co/downloads/kibana/kibana-7.6.1-x86_64.rpm
rpm -ivh elasticsearch-7.6.1-x86_64.rpm
rpm -ivh kibana-7.6.1-x86_64.rpm
# 删除旧版本
#   rpm -e [package name]

# 测试
systemctl start elasticsearch.service
curl -X GET 127.0.0.1:9200
#	{
#	  "name" : "localhost.localdomain",
#	  "cluster_name" : "elasticsearch",
#	  "cluster_uuid" : "JkTkVhIsST2zJzclNQO4pQ",
#	  "version" : {
#	    "number" : "7.6.1",
#	    "build_flavor" : "default",
#	    "build_type" : "rpm",
#	    "build_hash" : "aa751e09be0a5072e8570670309b1f12348f023b",
#	    "build_date" : "2020-02-29T00:15:25.529771Z",
#	    "build_snapshot" : false,
#	    "lucene_version" : "8.4.0",
#	    "minimum_wire_compatibility_version" : "6.8.0",
#	    "minimum_index_compatibility_version" : "6.0.0-beta1"
#	  },
#	  "tagline" : "You Know, for Search"
#	}

```

```
// dashboard
http://localhost:5601/app/kibana

// 控制台
http://localhost:5601/app/kibana#/dev_tools/console

可视化
```
## ELK构成
+ Logstash
+ ES 
elasticsearch
事后告警
+ Kibana

## 日志大数据分析及机器学习
ES

## ES的监控告警
替代业务服务的侵入性告警？实时性？