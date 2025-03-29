Kafka知识地图
	Broker
		配置项
			磁盘日志相关
				log.dirs
					Broker需要使用的若干个文件目录路径，逗号间隔（必须配置）
					最好配置到不同的物理磁盘上
						提升读写性能
						故障转移Failover
						不用RAID
				log.dir
					较早版本引入，不需要配置
			ZK相关
				zookeeper.connect
					ZK配置，逗号间隔
			Broker-Client连接相关
				listeners
					监听器，broker之间、client连接broker的协议
				advertised.listeners
					broker对外发布的监听器，用于外网访问
					技巧：Kafka Broker机器上配置了双网卡，一块网卡用于内网访问（即我们常说的内网IP）；
另一个块用于外网访问。那么你可以配置listeners为内网IP，advertised.listeners为外网IP
				host.name / host.port
					不再使用
				listener.security.protocol.map
					监听器协议底层使用了哪种安全协议
					如CONTROLLER:PLAINTEXT表示CONTROLLER自定义协议底层使用明文不加密传输数据
			topic管理
				auto.create.topics.enable
					是否允许自动创建topic
					推荐false，避免误拼写引起的多于topic
				unclean.leader.election.enable
					是否允许Unclean Leader选举
					只有保存数据多的副本才能参与partion leader选举
如果仅有落后太多的副本，进行的选举即unclean选举
如果允许unclean选举，则数据有可能丢失
如果不允许unclean选举，则仅有落后太多的副本则该partition不可用
					落后数量取决于replica.lag.time.max.ms
					默认false，推荐显式设置为false
				auto.leader.rebalance.enable
					是否允许Leader自动负载均衡选举，即定期换leader，使leader在broker均匀分布
					如果分布不均超过leader.imbalance.per.broker.percentage，则触发选举
默认10%
					换leader代价太高，client连接都要重新建立，推荐false
					如果一个broker重启，没有分区leader；
true时，会做leader均衡；
false时，就永远不会做leader均衡；
				auto.offset.reset
					consumer获取不到上次的offset，或读取到offset不可用时，采取的策略，如earliest / latest
			数据留存
				log.retention.{hours|minutes|ms}
					消息数据保存的时间
					优先级：ms>min>hour
				log.retention.bytes
					broker保存消息的磁盘容量大小
					-1标示不限制
				message.max.bytes
					broker能够接收的最大消息大小
					该设置不会预占用空间
					需要broker/consumer/producer设置一致，或以最小的为准
replica.fetch.max.bytes
consumer fetch.message.max.bytes
			topic相关配置
				retention.ms
					topic消息保存的时间
				retention.bytes
					topic预留的磁盘空间，-1表示无限使用磁盘空间
				max.message.bytes
					topic消息最大限制
			JVM参数
			操作系统参数
				文件描述符限制
				文件系统类型
				swap空间
				提交时间
					flush落磁盘时间
		消息格式
			0.11.0之前，称作message set/message
0.11.0以后，称作record batch/record
			kafka网络传输和文件读写是以批次batch为单位的
			8字节offset；4字节消息长度；
			4字节CRC；1字节版本号；1字节属性；8字节时间戳；
4字节key长度；key；4字节value长度；value；
				v1.x引入时间戳，4字节producer/create_time；4字节broker/log_append_time
		topic
		partition
			leader/follower选举机制
			分区分配策略
				同一分区的不同副本均衡分布
				同一主题的不同分区leader均衡分布
			日志同步机制
			添加分区，不会有消息迁移
			删除分区，需要借助工具，人工迁移消息
		replica副本机制
			提供备份机制、分布式伸缩性、CDN效果？
			一个追加写消息的日志提交机制，不同topic的副本分散保存在不同的broker上
副本不用于读取
			leader选举
			In-Sync Replica （ISR）与leader保持同步的副本
				有时只有leader这一个副本
		磁盘文件
			日志文件索引
		时间轮
		与客户端通讯协议
		监听器
			若干个逗号分隔的三元组，每个三元组的格式为<协议名称，主机名，端口号>
协议名称可能是标准的名字，比如 PLAINTEXT 表示明文传输、SSL 表示使用 SSL 或 TLS 加密传输等；
也可能是你自己定义的协议名字，比如CONTROLLER: //localhost:9092
		压缩算法
			节省带宽
		failover
			1.1之前，磁盘故障会引起broker宕机
1.1后，磁盘故障后broker做故障转移处理
	producer
		配置项
		生产均衡
			轮询策略
			随机策略
			按关键值策略，同一关键值有序
		压缩算法
			producer和broker使用相同的压缩算法，否则会重新压缩
			producer压缩、broker端保持、consumer端解压缩
			吞吐量：LZ4 > Snappy > zstd/gzip
			压缩比： zstd > LZ4 > gzip > Snappy
			最好producerCPU资源富余，可节省broker的空间时间
		拦截器
		幂等生产者
	consumer
		配置项
		consumer group
			消费均衡 rebalance
		__consumer_offsets
		位移消费
		事务
		长轮询
		high level api
	特点
		高可用
			冗余
				多磁盘
				免RAID
				多partition，副本间复制
			producer均衡？
			consumer均衡
		高吞吐
			磁盘顺序读取，多磁盘，多分区读取
			批量消费
	golang客户端：sarama/sarama cluster
	最佳实践
		分区数量配置多少合适？
		无消息丢失配置
		子主题 3
	Kafka 监控
	Kafka Spark
	Kafka Streams
	Kafka Connect
Broker
	配置项
		磁盘日志相关
			log.dirs
				Broker需要使用的若干个文件目录路径，逗号间隔（必须配置）
				最好配置到不同的物理磁盘上
					提升读写性能
					故障转移Failover
					不用RAID
			log.dir
				较早版本引入，不需要配置
		ZK相关
			zookeeper.connect
				ZK配置，逗号间隔
		Broker-Client连接相关
			listeners
				监听器，broker之间、client连接broker的协议
			advertised.listeners
				broker对外发布的监听器，用于外网访问
				技巧：Kafka Broker机器上配置了双网卡，一块网卡用于内网访问（即我们常说的内网IP）；
另一个块用于外网访问。那么你可以配置listeners为内网IP，advertised.listeners为外网IP
			host.name / host.port
				不再使用
			listener.security.protocol.map
				监听器协议底层使用了哪种安全协议
				如CONTROLLER:PLAINTEXT表示CONTROLLER自定义协议底层使用明文不加密传输数据
		topic管理
			auto.create.topics.enable
				是否允许自动创建topic
				推荐false，避免误拼写引起的多于topic
			unclean.leader.election.enable
				是否允许Unclean Leader选举
				只有保存数据多的副本才能参与partion leader选举
如果仅有落后太多的副本，进行的选举即unclean选举
如果允许unclean选举，则数据有可能丢失
如果不允许unclean选举，则仅有落后太多的副本则该partition不可用
				落后数量取决于replica.lag.time.max.ms
				默认false，推荐显式设置为false
			auto.leader.rebalance.enable
				是否允许Leader自动负载均衡选举，即定期换leader，使leader在broker均匀分布
				如果分布不均超过leader.imbalance.per.broker.percentage，则触发选举
默认10%
				换leader代价太高，client连接都要重新建立，推荐false
				如果一个broker重启，没有分区leader；
true时，会做leader均衡；
false时，就永远不会做leader均衡；
			auto.offset.reset
				consumer获取不到上次的offset，或读取到offset不可用时，采取的策略，如earliest / latest
		数据留存
			log.retention.{hours|minutes|ms}
				消息数据保存的时间
				优先级：ms>min>hour
			log.retention.bytes
				broker保存消息的磁盘容量大小
				-1标示不限制
			message.max.bytes
				broker能够接收的最大消息大小
				该设置不会预占用空间
				需要broker/consumer/producer设置一致，或以最小的为准
replica.fetch.max.bytes
consumer fetch.message.max.bytes
		topic相关配置
			retention.ms
				topic消息保存的时间
			retention.bytes
				topic预留的磁盘空间，-1表示无限使用磁盘空间
			max.message.bytes
				topic消息最大限制
		JVM参数
		操作系统参数
			文件描述符限制
			文件系统类型
			swap空间
			提交时间
				flush落磁盘时间
	消息格式
		0.11.0之前，称作message set/message
0.11.0以后，称作record batch/record
		kafka网络传输和文件读写是以批次batch为单位的
		8字节offset；4字节消息长度；
		4字节CRC；1字节版本号；1字节属性；8字节时间戳；
4字节key长度；key；4字节value长度；value；
			v1.x引入时间戳，4字节producer/create_time；4字节broker/log_append_time
	topic
	partition
		leader/follower选举机制
		分区分配策略
			同一分区的不同副本均衡分布
			同一主题的不同分区leader均衡分布
		日志同步机制
		添加分区，不会有消息迁移
		删除分区，需要借助工具，人工迁移消息
	replica副本机制
		提供备份机制、分布式伸缩性、CDN效果？
		一个追加写消息的日志提交机制，不同topic的副本分散保存在不同的broker上
副本不用于读取
		leader选举
		In-Sync Replica （ISR）与leader保持同步的副本
			有时只有leader这一个副本
	磁盘文件
		日志文件索引
	时间轮
	与客户端通讯协议
	监听器
		若干个逗号分隔的三元组，每个三元组的格式为<协议名称，主机名，端口号>
协议名称可能是标准的名字，比如 PLAINTEXT 表示明文传输、SSL 表示使用 SSL 或 TLS 加密传输等；
也可能是你自己定义的协议名字，比如CONTROLLER: //localhost:9092
	压缩算法
		节省带宽
	failover
		1.1之前，磁盘故障会引起broker宕机
1.1后，磁盘故障后broker做故障转移处理
producer
	配置项
	生产均衡
		轮询策略
		随机策略
		按关键值策略，同一关键值有序
	压缩算法
		producer和broker使用相同的压缩算法，否则会重新压缩
		producer压缩、broker端保持、consumer端解压缩
		吞吐量：LZ4 > Snappy > zstd/gzip
		压缩比： zstd > LZ4 > gzip > Snappy
		最好producerCPU资源富余，可节省broker的空间时间
	拦截器
	幂等生产者
consumer
	配置项
	consumer group
		消费均衡 rebalance
	__consumer_offsets
	位移消费
	事务
	长轮询
	high level api
特点
	高可用
		冗余
			多磁盘
			免RAID
			多partition，副本间复制
		producer均衡？
		consumer均衡
	高吞吐
		磁盘顺序读取，多磁盘，多分区读取
		批量消费
golang客户端：sarama/sarama cluster
最佳实践
	分区数量配置多少合适？
	无消息丢失配置
	子主题 3
Kafka 监控
Kafka Spark
Kafka Streams
Kafka Connect