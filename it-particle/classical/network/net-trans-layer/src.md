# 传输控制的基石 struct sock_common
```c
/**
 * struct sock_common - minimal network layer representation of sockets
 * @skc_family: network address family
 * @skc_state: Connection state
 * @skc_reuse: sSO_REUSEADDR setting
 * @skc_bound_dev_if: bound device index if != 0
 * @skc_node: main hash linkage for various protocol lookup tables
 * @skc_bind_node: bind hash linkage for various protocol lookup tables
 * @skc_refcnt:reference count
 * @skc_hash: hash value used with various protocol lookup tables
 * @skc_prot: protocol handlers inside a network family
 * @skc_net: reference to the network namespace of this socket
 * This is the minimal network layer representation of sockets, the header
 * for struct sock and struct inet_timewait_sock.
 */
struct sock_common {
  //地址族
  unsigned short skc_family;
  //传输层的状态，等同于TCP的状态，但是UDP也使用TCP定义的部分状态，如TCP_CLOSE
  volatile unsigned char skc_state;
  //表示该socket的地址和端口是否可以被重用，就是SO_REUSEADDR选项配置的值，默认值为0
  unsigned char skc_reuse;
  int  skc_bound_dev_if;
  //对于TCP，该成员用于将所有的TCP传输控制块链接到哈希表tcphashinfo中
  //对于UDP，该成果用于将所有的UDP传输控制块链接到哈希表udphash中
  struct hlist_node skc_node;
  //该成员将传输控制块链接到已绑定端口信息结构中，只用于面向连接的套接字，如TCP
  struct hlist_node skc_bind_node;
  //引用计数，只有当该引用计数为0后，传输控制块才能被释放
  atomic_t skc_refcnt;
  //TCP的ehash表的哈希值，由于计算哈希值比较耗时，所以这里用一个单独的成员来记录
  unsigned int skc_hash;
  //指向传输层协议提供给套接口层的函数接口
  struct proto *skc_prot;
  struct net *skc_net;
};
```

# 通用传输块 struct sock
```c
struct sock {
	/*
	 * Now struct inet_timewait_sock also uses sock_common, so please just
	 * don't add nothing before this first member (__sk_common) --acme
	*/
	struct sock_common __sk_common;
	//为了方便访问，对于sock_common中的每一个成功定义了对应宏
#define sk_family __sk_common.skc_family
#define sk_state __sk_common.skc_state
#define sk_reuse __sk_common.skc_reuse
#define sk_bound_dev_if __sk_common.skc_bound_dev_if
#define sk_node __sk_common.skc_node
#define sk_bind_node __sk_common.skc_bind_node
#define sk_refcnt __sk_common.skc_refcnt
#define sk_hash __sk_common.skc_hash
#define sk_prot __sk_common.skc_prot
#define sk_net __sk_common.skc_net
	// 传输层的关闭状态
	unsigned char sk_shutdown : 2,
		sk_no_check : 2,
	//标识应用程序是否对当前传输控制块做过哪些操作，可取的值如下：
	//SOCK_SNDBUF_LOCK------用户已经通过套接口选项设置了发送缓冲区大小
	//SOCK_RCVBUF_LOCK------用户已经通过套接口选项设置了接收缓冲区大小
	//SOCK_BINDADDR_LOCK----该TCB已经绑定到了一个具体的本地地址上
	//SOCK_BINDPORT_LOCK----该TCB已经绑定到了一个具体的本地端口上
		sk_userlocks : 4;
	unsigned char sk_protocol;
	unsigned short sk_type;
	//该套接字接收缓冲区的最大限制，以字节为单位
	int sk_rcvbuf;
	socket_lock_t sk_lock;
	struct {
		struct sk_buff *head;
		struct sk_buff *tail;
	} sk_backlog;
	wait_queue_head_t *sk_sleep;
	struct dst_entry *sk_dst_cache;
	struct xfrm_policy *sk_policy[2];
	rwlock_t sk_dst_lock;
	//TCP层从底层收到数据并处理后，就会将该数据放入字节的接收队列中等待用户空间程序读取，
	//只要TCP协议栈已经处理了收到的数据包，那么就会将其占用空间累加到该变量上，所以，该
	//变量代表接收方向上，该套接字占用的空间，用户程序读取数据后，该值会相应的减小
	atomic_t sk_rmem_alloc;
	atomic_t sk_wmem_alloc;
	atomic_t sk_omem_alloc;
	int sk_sndbuf;
	//接收队列，放入该队列的SKB已经被协议栈处理完毕，等待用户程序读取
	struct sk_buff_head sk_receive_queue;
	//数据发送队列,包括重传数据，sk_send_head指针将重传队列与待发送队列分割开来
	struct sk_buff_head sk_write_queue;
	struct sk_buff_head sk_async_wait_queue;
	//发送队列中所有数据的总长度，是skb->truesize的累加，所以不仅仅包含了数据部分，
	//还包含了SKB控制部分
	int sk_wmem_queued;
	//预分配的内存大小，单位为字节。由于真正在分配内存的时候，并非是按照实际需求的大小分配，
	//而是按照物理页大小进行分配，所以每次分配都会产生余量，这部分余量就是预分配的内存，后续
	//分配如果余量已经能够满足需求，那么就不会再进行分配
	int sk_forward_alloc;
	gfp_t sk_allocation;
	//路由缓存中保存的能力，记录了底层程序能够支持的一些特性，由sk_setup_caps()设置
	int sk_route_caps;
	//该套接字的GSO类型，底层软件需要根据L4的类型在分段时做区分处理，TCPv4为SKB_GSO_TCPV4
	int sk_gso_type;
	int sk_rcvlowat;
	unsigned long sk_flags;
	unsigned long sk_lingertime;
	struct sk_buff_head sk_error_queue;
	struct proto *sk_prot_creator;
	rwlock_t sk_callback_lock;
	int sk_err,
		sk_err_soft;
	//在接收方向上，每当由于内存问题丢弃一个数据包，该变量累加1
	atomic_t sk_drops;
	//如果该TCB属于一个监听套接字，那么该字段记录当前accpet连接队列中的请求套接字的数目
	unsigned short sk_ack_backlog;
	//记录listen()系统调用时传入的的backlog，表示accept连接队列中可容纳的请求套接字的上限，
	//具体见《TCP之listen()系统调用》
	unsigned short sk_max_ack_backlog;
	__u32 sk_priority;
	struct ucred sk_peercred;
	long sk_rcvtimeo;
	long sk_sndtimeo;
	struct sk_filter *sk_filter;
	void *sk_protinfo;
	struct timer_list sk_timer;
	ktime_t sk_stamp;
	struct socket *sk_socket;
	void *sk_user_data;
	// 当支持分片时，会使用skb的frag_list[]来保存数据，这些数据实际保存在了页面中，
	// 每次分配都会为该TCB分配一个完整页面，该字段就指向最后一次分配的页面，类似的，下面的
	// sk_sndmsg_off指向该页面的偏移，后续如果要继续填充，则从该偏移处开始。用完一页后继续分配
	struct page *sk_sndmsg_page;
	//该指针将发送队列sk_write_queue分为前后两部分，前面部分为重传队列，
	//即已经发送但是尚未收到ACK的段，后面部分为加入发送缓冲区但是还没有发送的段
	struct sk_buff *sk_send_head;
	__u32 sk_sndmsg_off;
	int sk_write_pending;
	void *sk_security;
	__u32 sk_mark;
	/* XXX 4 bytes hole on 64 bit */
	void (*sk_state_change)(struct sock *sk);
	//当传输层收到数据并且将数据放入到接收队列中时，会回调该函数。该函数的默认实现是
	//sock_def_readable()，其逻辑就是唤醒那么正在等待数据的进程。传输层协议也可以指定更加特殊的
	void (*sk_data_ready)(struct sock *sk, int bytes);
	void (*sk_write_space)(struct sock *sk);
	void (*sk_error_report)(struct sock *sk);
	 int (*sk_backlog_rcv)(struct sock *sk,
	 struct sk_buff *skb);
	void (*sk_destruct)(struct sock *sk);
};
```

# AF_INET协议族 传输控制块 struct inet_sock

# TCP 传输控制块 struct tcp_sock
```c
struct tcp_sock {
	/* inet_connection_sock has to be the first member of tcp_sock */
	struct inet_connection_sock inet_conn;
	//TCP首部长度，包括选项部分
	u16 tcp_header_len; /* Bytes of tcp header to send */
	//TCP发送数据构造skb时用于指定一个skb能够容纳的数据量，每次发送数据时都会根据当前的MSS
	//计算该值(见tcp_current_mss())。如果网卡不支持TSO，那么该值就是MSS，否则为MSS的整数倍
	u16 xmit_size_goal; /* Goal for segmenting output packets */

	/*
	 * Header prediction flags
	 * 0x5?10 << 16 + snd_wnd in net byte order
	 */
	__be32 pred_flags;

	/*
	 * RFC793 variables by their proper names. This means you can
	 * read the code and the spec side by side (and laugh ...)
	 * See RFC793 and RFC1122. The RFC writes these in capitals.
	 */
	//希望收到的下一个段的序号
	u32 rcv_nxt; /* What we want to receive next */
	//用户空间下一次读时，应该从该序号开始拷贝
	u32 copied_seq; /* Head of yet unread data */
	//记录上次想对端发送数据包时携带的ACK序号，即rcv_nxt，该值只有在想对端发送数据包时才更新，
	//所以其有时候会稍微滞后rcv_nxt一点
	u32 rcv_wup; /* rcv_nxt on last window update sent */
	//下一个要发送的段的序号
	u32 snd_nxt; /* Next sequence we send */
	//已经发送的段中，未被确认的最小序号
	u32 snd_una; /* First byte we want an ack for */
	//最近发送的小包（长度小于MSS的段）的最后一个序号，每次发送小包数据时更新该值，主要用于Nagle算法
	u32 snd_sml; /* Last byte of the most recently transmitted small packet */
	//最后一次收到ACK段的时间戳，用于保活
	u32 rcv_tstamp; /* timestamp of last received ACK (for keepalives) */
	//最后一次发送数据包的时间
	u32 lsndtime; /* timestamp of last sent data packet (for restart window) */

	//该结构用于将内核中的数据拷贝到用户空间中
	/* Data for direct copy to user */
	struct {
        //prequeue队列
        struct sk_buff_head prequeue;
        //未启用sysctl_low_latency情况下，当前正在读取该TCP流的进程，
        //如果为NULL，则表示当前没有进程正在读取该TCP流
        struct task_struct *task;
        //未启用sysctl_low_latency情况下，当前正在读取该TCP流的进程提供的用户空间缓存地址
        struct iovec *iov;
        //prequeue队列已经消耗的内存大小
        int memory;
        //用户空间内存当前剩余可用空间大小
        int len;
        //不考虑DMA部分
#ifdef CONFIG_NET_DMA
        /* members for async copy */
        struct dma_chan *dma_chan;
        int wakeup;
        struct dma_pinned_list *pinned_list;
        dma_cookie_t dma_cookie;
#endif
	} ucopy;

	//记录最近导致发送窗口更新的ACK段的序号，用来判断后续收到ACK段是否需要更新发送窗口。
	//当收到的ACK段的序号大于该值时，表示需要更新发送窗口
	u32 snd_wl1; /* Sequence for window update */
	//记录当前发送窗口的大小，即接收方通告的最新的接收窗口大小
	u32 snd_wnd; /* The window we expect to receive */
	//记录当前收到的接收方通告过的最大接收窗口值
	u32 max_window; /* Maximal window ever seen from peer */
	u32 mss_cache; /* Cached effective mss, not including SACKS */
	u32 window_clamp; /* Maximal window to advertise */
	u32 rcv_ssthresh; /* Current window clamp */
	u32 frto_highmark; /* snd_nxt when RTO occurred */
	u8 reordering; /* Packet reordering metric. */
	u8 frto_counter; /* Number of new acks after RTO */
	u8 nonagle; /* Disable Nagle algorithm? */
	u8 keepalive_probes; /* num of allowed keep alive probes */

	/* RTT measurement */
	u32 srtt; /* smoothed round trip time << 3 */
	u32 mdev; /* medium deviation */
	u32 mdev_max; /* maximal mdev for the last rtt period */
	u32 rttvar; /* smoothed mdev_max */
	u32 rtt_seq; /* sequence number to update rttvar */
	u32 packets_out; /* Packets which are "in flight" */
	u32 retrans_out; /* Retransmitted packets out */
	/*
	 * Options received (usually on last packet, some only on SYN packets).
	 */
	//保存接收到的TCP选项信息,每接收到一个携带了选项的输入报文都会更新该成员内容
	//rx_opt.user_mss: 应用程序通过套接字选项TCP_MAXSEG设定的本端发送MSS值，如果不设定，该值为0
	//rx_opt.mss_clamp: 初始化为536，当收到对端通过的MSS时，取min(rx_opt.user_mss, 对端通告值)
	struct tcp_options_received rx_opt;

	/*
	 * Slow start and congestion control (see also Nagle, and Karn & Partridge)
	 */
	u32 snd_ssthresh; /* Slow start size threshold */
	u32 snd_cwnd; /* Sending congestion window */
	u32 snd_cwnd_cnt; /* Linear increase counter */
	u32 snd_cwnd_clamp; /* Do not allow snd_cwnd to grow above this */
	u32 snd_cwnd_used;
	u32 snd_cwnd_stamp;

	struct sk_buff_head out_of_order_queue; /* Out of order segments go here */
	u32 rcv_wnd; /* Current receiver window */
	//已加入到发送队列中的的最后一个字节的数据
	u32 write_seq; /* Tail(+1) of data held in tcp send buffer */
	u32 pushed_seq; /* Last pushed seq, required to talk to windows */
	
    /* SACKs data */
	struct tcp_sack_block duplicate_sack[1]; /* D-SACK block */
	//用于保存生成的SACK块，下次发送时，会根据该数组内容构造SACK选项。
	//由于最多可以有4个SACK块，所以数组长度定义为了4
	struct tcp_sack_block selective_acks[4]; /* The SACKS themselves*/
	struct tcp_sack_block recv_sack_cache[4];
	struct sk_buff *highest_sack; /* highest skb with SACK received
	 * (validity guaranteed only if
	 * sacked_out > 0)
	
	/* from STCP, retrans queue hinting */
	struct sk_buff* lost_skb_hint;
	struct sk_buff *scoreboard_skb_hint;
	struct sk_buff *retransmit_skb_hint;
	struct sk_buff *forward_skb_hint;
	int lost_cnt_hint;
	int retransmit_cnt_hint;

	u32 lost_retrans_low; /* Sent seq after any rxmit (lowest) */
	//广告MSS，本端告诉对端的MSS值，即SYN和SYN+ACK段中携带的MSS选项值，等于MTU-40
	u16 advmss; /* Advertised MSS */
	u16 prior_ssthresh; /* ssthresh saved at recovery start */
	u32 lost_out; /* Lost packets */
	u32 sacked_out; /* SACK'd packets */
	u32 fackets_out; /* FACK'd packets */
	u32 high_seq; /* snd_nxt at onset of congestion */
	u32 retrans_stamp; /* Timestamp of the last retransmit,
	 * also used in SYN-SENT to remember stamp of
	 * the first SYN. */
	u32 undo_marker; /* tracking retrans started here. */
	int undo_retrans; /* number of undoable retransmissions. */
	u32 urg_seq; /* Seq of received urgent pointer */
	u16 urg_data; /* Saved octet of OOB data and control flags */
	u8 urg_mode; /* In urgent mode */
	u8 ecn_flags; /* ECN status bits. */
	u32 snd_up; /* Urgent pointer */
	u32 total_retrans; /* Total retransmits for entire connection */
	u32 bytes_acked; /* Appropriate Byte Counting - RFC3465 */
	unsigned int keepalive_time; /* time before keep alive takes place */
	unsigned int keepalive_intvl; /* time interval between keep alive probes */
	int linger2;
	unsigned long last_synq_overflow;
	u32 tso_deferred;
	/* Receiver side RTT estimation */
	struct {
        u32 rtt;
        u32 seq;
        u32 time;
	} rcv_rtt_est;
	/* Receiver queue space */
	struct {
        int space;
        u32 seq;
        u32 time;
	} rcvq_space;
	/* TCP-specific MTU probe information. */
	struct {
        u32 probe_seq_start;
        u32 probe_seq_end;
	} mtu_probe;
#ifdef CONFIG_TCP_MD5SIG
	/* TCP AF-Specific parts; only used by MD5 Signature support so far */
	struct tcp_sock_af_ops *af_specific;
	/* TCP MD5 Signagure Option information */
	struct tcp_md5sig_info *md5sig_info;
#endif
};
```
## TCP 连接请求块 struct tcp_request_sock

# UDP 传输控制块 struct udp_sock
```c
struct udp_sock {
	/* inet_sock has to be the first member */
	struct inet_sock inet;
	// 和MSG_MORE标记、UDP_CROK选项有关，用于控制UDP的发送过程，具体用法见udp发送过程分析
	int pending;
	// 记录是否设定了UDP_CROK选项，取值0或1
	unsigned int corkflag;
	 __u16 encap_type; /* Is this an Encapsulation socket? */
	/*
	 * Following member retains the information to create a UDP header
	 * when the socket is uncorked.
	 */
	// len记录了当前该传输控制块上处于pending的数据量
	__u16 len;
	/*
	 * Fields specific to UDP-Lite.
	 */
	__u16 pcslen;
	__u16 pcrlen;
	/* indicator bits used by pcflag: */
#define UDPLITE_BIT 0x1 /* set by udplite proto init function */
#define UDPLITE_SEND_CC 0x2 /* set via udplite setsockopt */
#define UDPLITE_RECV_CC 0x4 /* set via udplite setsocktopt */
	__u8 pcflag; /* marks socket as UDP-Lite if > 0 */
	__u8 unused[3];
	/*
	 * For encapsulation sockets.
	 */
	int (*encap_rcv)(struct sock *sk, struct sk_buff *skb);
};
```