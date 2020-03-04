# C++的几种数据结构及其实现
|map     |红黑树      |有序|
|list    |双向链表    |无序|
|vector  |数组        |无序    2倍扩容，拷贝|
|set     |红黑树      |有序|
|queue   |单向队列    |无序|
|deque   |双向队列    |无序|
|c++11 |unordered_map hashmap |无序|

#hashtable 哈希表

#skiptable 跳跃表

#B树是什么
balance tree

B树的意思是balance tree平衡树。
红黑树的数据结构

#stl vector和list的区别
vector和数组类似，拥有一段连续的内存空间，且起始地址不变。
高效随机存取，查询速度O(1)，插入和删除的操作，时间复杂度为O(n)。
当内存不够时，会重新申请一块内存空间并拷贝。
list是双向链表，内存空间是不连续的。
通过指针访问数据，随机存取没效率O(n)，能高效插入和删除。
