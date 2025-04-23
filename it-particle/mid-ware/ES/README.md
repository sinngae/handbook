ElasticSearch
----
ES，弹性搜索。
> Open-source/Restful-API/Json over HTTP/scales massivly/full text search/Lucene based/high availbility/schema free/distributed
> real time, search and analystics Engine.

# 问题引入
三个文本：
```
  文本A:  我有一本书
  文本B:  我是Areno
  文本C:  Areno有一本C++书
```
如何根据关键词“Areno”，找出相关的文本？

+ 如果有百万个文本，平均大小为1MB，怎么做关键词搜索？
    + 海量文本中的关键词检索问题
    + 电商搜索
    + 非结构化数据中的信息检索
+ 信息检索 =》 AI检索？

> 维基百科使用Elasticsearch来进行全文搜做并高亮显示关键词，以及提供search-as-you-type、did-you-mean等搜索建议功能。
> 英国卫报使用Elasticsearch来处理访客日志，以便能将公众对不同文章的反应实时地反馈给各位编辑。
> StackOverflow将全文搜索与地理位置和相关信息进行结合，以提供more-like-this相关问题的展现。
> GitHub使用Elasticsearch来检索超过1300亿行代码。
> 每天，Goldman Sachs使用它来处理5TB数据的索引，还有很多投行使用它来分析股票市场的变动。

## 场景：
+ 日志分析 ELK套件
    + 数据写入量、如何提高吞吐量、降低资源消耗
+ 数据分析和可视化
    + 查询与聚合、快速检索和聚合
+ 全文搜索系统
    + 数据一致性、时效性、准确性
    + 从其他组件同步到ES的机制、同步速度
    + 技术方案与策略
+ 内容推荐系统
+ 文档存储和管理
+ 搜索 + 地理位置

文档检索的后端服务，word/pdf/ppt/html;
DB+ELK检索服务层，ELK日志检索服务，数据分析、可视化展示;


## 功能：
+ 亿级数据量秒级搜索能力
+ 多种文本分析器
+ 实时索引和搜索
+ 复杂的数据分析和聚合操作
+ 布尔、范围等多种查询方式

## 特性：
+ 开源分布式搜索引擎
+ 数据处理平台
+ 基于Lucence
+ 分布式、实时、全文搜索系统
+ 稳定、可靠、高可用、可扩展


# 基础数据结构

+ inverted index, 倒排索引，使用文档的内容索引文档本身
    + 非正排索引（文档ID索引文档）
    + 文档拆词为词项，词项索引文档ID
        + 分词，分词器，IK中文分词器，结巴分词器
    + 将词项按字典序排序，构成词典库
+ term index，目录树，优化词典库的查询
    + 倒排索引的词典很大，只能放在磁盘，查询很慢
    + 词项做前缀提取（英文26字母、汉语拼音、汉字）
    + 构造目录树，体积小、快速获取词项位置
+ stored fields，存储字段，使用文档ID指向的原文档
    + Stored fields，存储字段、行式存储，相对于仅用于搜索但不存储的字段；_source，文档的原始字段，一个大json，也可禁用存储
    + 文档内容，原内容_source 是默认存储的；默认地，字段值被索引为可以搜索的，但是并没有存储
    + 可以指定字段被存储，mapping配置；不建议大量使用，占用磁盘，只用于存储那些需要在搜索结果中直接返回的字段、不支持DocValues的字段
+ Doc Values，值存储，按字段排序、列式存储
    + 序列化的列式存储，适用于聚合、排序、脚本等操作，数字、地理坐标、日期、IP等不分词类型都默认开启，不支持text类型
    + 正排索引、使用磁盘，可禁用以节省磁盘空间
    + text类型 使用时，可临时创建 fielddata，内存中、开销大，

segment，复合文档，完整搜索功能的最小单元，包括上述四种结构。

## lucene，ES的基础
+ 如果新增文档写入同一个segment，需要更新segment内部的多个数据结构
    + 读写并发时，性能肯定受影响
+ 如果segment生成后，就不再修改；新文件写入新的segment
    + 新旧分别负责写读；搜索时，并发搜索多个segment即可
+ segment增长一定数量后，句柄将会耗尽；segment过多，读并发越多，影响检索性能
    + 不定期合并多个小的segment，以控制句柄数。这就是Segment Merging
+ 多个segment 组成一个单机的检索库，这就是lucene

lucene 不支持高性能、高扩展性、高可用

## shard & replica
分布式-弹性：
+ 不同的主题，分为不同 index name，写入不同的lucene
+ 同一个index name可以分为多个Shard，每个Shard本质就是一个独立的Lucene库
    + 读写操作被均衡到多个分片
+ 分片被部署在不同的机器上，提供更高性能，每个机器就是一个节点
    + 增加节点可以缓解机器CPU占用过高的问题
+ 如果一个node挂了，node的分片都不可用了
    + 为了高可用，给每个分片增加几个副本，分别称为主分片和副本分片
    + 副本分片可以支持读操作，还可以在主分片不可用时升级为主分片

每个Node都支持 集群管理、存储数据、处理请求，但是这些功能不用扩容

Index 模板及别名，视图、通配符索引，多个索引检索、大数据量检索、历史数据删除/转储、基于时间创建索引，Mapping定义及字段类型变更、shard数变更。
别名支持了更灵活的检索、索引的管理，插入还是得插入到具体的索引。

## 分片路由
ES如何知道一个文档在那个分片中？

分片路由，这个过程是根据下面这个公式决定的：
shard = hash(routing) % number_of_primary_shards

routing 是一个可变值，默认是文档的 _id ，也可以设置成一个自定义的值。
routing 通过 hash 函数生成一个数字，然后这个数字再除以 number_of_primary_shards （主分片的数量）后得到 余数 。

这个分布在 0 到 number_of_primary_shards-1 之间的余数，就是我们所寻求的文档所在分片的位置。

> 这就是为什么我们要在创建索引的时候就确定好主分片的数量 并且最好永远都不需要改变这个数量：因为如果数量变化了，那么所有之前路由的值都会无效，文档也再也找不到了。

## Node角色
Elasticsearch 7.9之前的版本中的节点类型：
数据节点、协调节点、候选主节点、ingest节点

7.9及之后，节点类型升级为节点角色 Node roles（单节点ES集群，配置为 cdfhilmrstw）。

+ 主从
    + 主节点 master-eligible node
        + 集群管理，索引增删改查、节点管理、节点分片；高可用，至少3个主节点资格节点、其中至少2个非仅投票主节点
        + 分布式，Raft协议，集群高可用
    + 专用候选主节点 Dedicated master-eligibel node
    + 仅投票主节点 Voting-only master-eligibel node
+ 功能
    + 数据预处理节点 Ingest Node
        + 预处理管道组成预处理任务，输入转换
    + 仅协调节点 Coordinating only node
        + 读写负载均衡，路由分发、聚集搜索或聚合结果
    + 远程节点 Remote-eligible node
        + 跨ES集群计算或跨集群复制
    + 机器学习节点 Machine learning node （付费节目）
    + 转换节点 Transform node
        + 透视转换、最新转换，基于现有索引创建新索引、聚集、聚合、取时间，耗时、耗资源
+ 数据，数据节点 Data node
    + 数据存储、数据增删改查、搜索、聚合；多层冷热集群架构，实际落地存储靠的是data_content、其他只是标识
    + 内容数据节点
    + 热数据节点
    + 温数据节点
    + 冷数据节点
    + 冷冻数据节点

可以一个Node具备所有功能，集群管理、存储数据、处理请求；分布式场景，发挥集群硬件性能，不用每个节点都具备/运行所有功能，是浪费的。

# 文档增删改查

## 写入流程
客户端写入，协调节点，ingest输入转换，数据节点 分词、建立索引、存储数据、副本；主Shard响应协调节点一个ACK，协调节点返回写入完成。

分词、过滤、归一化

mapping动态类型推导

## 索引详细流程
+ 1. 接收文档，通常是一个HTTP POST或PUT请求。
+ 2. 路由到正确的分片（Shard），根据`_id`计算分片编号，转发请求到对应的分片。
+ 3. 写入内存中的缓冲区
- 内存缓冲区：
  - 文档首先被写入到分片的内存缓冲区中。内存缓冲区是一个临时存储区域，用于快速处理索引请求。
  - 这一步确保了索引操作的低延迟，因为内存操作比磁盘操作快得多。
+ 4. 写入事务日志（Translog）
- 事务日志：
  - 文档同时被写入到事务日志（Translog）中。事务日志是一个持久化的日志文件，用于记录所有未提交的索引操作。
  - 事务日志确保了数据的持久性，即使在系统故障的情况下，也可以从日志中恢复数据。
+ 5. 刷新到Segment
- 定期刷新：
  - Elasticsearch定期将内存缓冲区中的数据刷新到磁盘上的Segment文件中。这个过程称为刷新（Refresh）。
  - 默认情况下，刷新间隔为1秒，但可以通过`index.refresh_interval`设置进行调整。
- 创建Segment：
  - 刷新操作会将内存中的数据写入一个新的Segment。新Segment被创建后，查询操作可以访问其中的数据。
+ 6. 提交（Commit）
- 定期提交：
  - Elasticsearch定期将事务日志中的操作提交到磁盘上的Segment文件中。这个过程称为提交（Commit）。
  - 提交操作确保了数据的持久性，即使在系统故障的情况下，也可以从磁盘上的Segment文件中恢复数据。
  - 默认情况下，提交操作的频率较低，通常在几分钟一次，但可以通过`index.translog.durability`和`index.translog.flush_threshold_size`等设置进行调整。
+ 7. 清理事务日志
- 清理日志：
  - 在提交操作完成后，事务日志中的操作被标记为已提交，对应的日志条目可以被清理。
  - 这个过程确保了事务日志不会无限增长，节省磁盘空间。
+ 8. Segment合并
- 自动合并：
  - Elasticsearch会定期自动合并小的Segment为较大的Segment，以优化性能和减少磁盘空间占用。
  - 合并操作是后台进行的，不会阻塞索引和查询操作。

## segment merge
Index写入
  新增segment，占用文件句柄、存储
  每次查询都会并发查每个segment，segment越多，查询越慢
后台运行Segment Merging
  选取一些大小近似的segment合并成更大的segment
    合并和Index写入、删除、检索同时发生，不会打断他们的执行
  新的segment写入磁盘，提交一个commit
    commit包含新的segment、排除掉旧的
  新segment提供查询服务
  删除旧segment
大segment的merge占用很多IO和CPU，能够影响检索性能
  ES会限制大segment合并，为检索保留足够的资源

## 删除
数据标记 deleted，逻辑删除；段合并时，逻辑删除变成物理删除

## 更新
`Update = Delete + (Index - Ingest Pipeline)`
文档在 Elasticsearch 中是不可变的。
当 Elasticsearch 更新文档时，它将删除原始文档并为新的待更新的文档建立索引。

这两步操作在每个 Lucene 分片是原子操作，操作会带来删除和索引（索引不调用任何 ingest pipeline 操作）操作的开销。

partial update，部分更新，发生在同一个segment里，性能较高，计数器场景；

## 查询检索
+ 信息检索，搜索，全文搜索、范围搜索、聚合，
+ ES搜索，query phase，scatter分发请求、search并发检索、gather收集、merge结果整合；
+ fetch phase，协调节点获取数据，收集汇聚为单个结果集
+ 搜索细节，检索、结果整合、打分、排序、关键词纠错纠偏
+ 对查询关键词分词，分别做倒排索引匹配算分、汇总得分、

精确匹配检索，term、terms、range、exists
模糊匹配检索，wildcard、prefix、regexp
分词全文检索，match、match_phase
多条件bool检索，must、must_not、should

高亮
分页查询
指定关键词
scroll查询
聚合统计，指标、Bucket、管道
聚合后分页
hisgram日期聚合图像化


## 更多

父子文档
数组类型
Nested嵌套类型
Reindex 重建索引
IK分词器、Jieba分词器
_source、stored_fields区别
slow log
score,  BM25、TF/IDF

ES集群规模和容量规划
基于Ingest实现的PDF文档预处理和索引
ES插件错误排查
ES性能调优
ES跨集群检索
ES + Kibana/Grafana 可视化分析 
ES + Kafka/Mongo/Hadoop 组合场景
ES 打分如何归一化到 0-1


## rollup功能
对于时间序列的数据，聚合成不同的时间粒度到新的索引中。
以此，提高查询速度和响应时间。

## transform节点功能
摘要索引，通过聚合和转换数据，生成新的分析友好格式。
+ 持久化存储在集群中的
+ 两种转换方式，pivot、latest
+ 


## 写入的准确性和一致性保证
+ 主副分片机制
    + 先写主分片，再同步到副分片（副分片写失败不影响，可以重试）
+ 写入一致性，可设置
    + One, 主分片写成功，即为写成功
    + Quorum, 多数分片写成功，才为成功
    + All，所有分片写成功，为成功
+ 事务日志
    + 写入主分片，并记录到事务日志、同步到副分片
    + 数据成功写入后（刷新到磁盘），提交事务日志
    + 异常导致的写入失败，则从事务日志恢复
+ 版本控制
    + 文档更新，版本递增
    + 更新文档可以指定版本号，仅一致时才更新成功

