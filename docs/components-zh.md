以下是 `Supporting Components` 各模块的详细说明及参数位置，可用于文档目录结构中：

---

### **Data Container**

#### `array`
- **描述**：动态数组，支持栈操作（如 push/pop）。
- **主要函数**：
  - [bfdev_array_init()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/array.h#L57-L62)：初始化数组。
  - [bfdev_array_push()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/array.h#L173-L174)：向数组末尾添加元素。
  - [bfdev_array_pop()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/array.h#L176-L177)：从数组末尾移除元素。
  - [bfdev_array_splice()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/array.h#L238-L240)：在指定索引处插入/删除元素。
- **头文件**：`<bfdev/array.h>`
- **源文件**：[src/array.c](https://github.com/openbfdev/bfdev/blob/master/src/array.c)

#### `bloom`
- **描述**：布隆过滤器，用于快速判断一个元素是否可能存在于集合中。
- **主要函数**：
  - `bfdev_bloom_init()`：初始化布隆过滤器。
  - `bfdev_bloom_add()`：将元素加入过滤器。
  - `bfdev_bloom_check()`：检查元素是否存在。
- **头文件**：`<bfdev/bloom.h>`
- **源文件**：[src/bloom.c](https://github.com/openbfdev/bfdev/blob/master/src/bloom.c)

#### `btree`
- **描述**：B+ 树实现，适用于大规模数据存储和查找。
- **主要函数**：
  - [bfdev_btree_init()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/btree.h#L60-L65)：初始化 B+ 树。
  - [bfdev_btree_insert()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/btree.h#L77-L78)：插入键值对。
  - `bfdev_btree_search()`：根据键查找值。
- **头文件**：`<bfdev/btree.h>`
- **源文件**：[src/btree.c](https://github.com/openbfdev/bfdev/blob/master/src/btree.c)

#### `circle`
- **描述**：循环队列，适用于固定大小的先进先出场景。
- **主要函数**：
  - `bfdev_circle_init()`：初始化循环队列。
  - `bfdev_circle_enqueue()`：入队操作。
  - `bfdev_circle_dequeue()`：出队操作。
- **头文件**：`<bfdev/circle.h>`
- **源文件**：`src/circle.c`

#### `fifo`
- **描述**：无锁 FIFO 队列，适合单读单写线程安全场景。
- **主要函数**：
  - `bfdev_fifo_init()`：初始化 FIFO 队列。
  - [bfdev_fifo_put()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/fifo.h#L361-L384)：入队操作。
  - [bfdev_fifo_get()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/fifo.h#L332-L355)：出队操作。
- **头文件**：`<bfdev/fifo.h>`
- **源文件**：[src/fifo.c](https://github.com/openbfdev/bfdev/blob/master/src/fifo.c)

#### `hashmap`
- **描述**：哈希表实现，支持动态扩容与重新哈希。
- **主要函数**：
  - [bfdev_hashmap_init()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/hashmap.h#L75-L80)：初始化哈希表。
  - `bfdev_hashmap_put()`：插入键值对。
  - `bfdev_hashmap_get()`：获取键对应的值。
- **头文件**：`<bfdev/hashmap.h>`
- **源文件**：[src/hashmap.c](https://github.com/openbfdev/bfdev/blob/master/src/hashmap.c)

#### `hashtbl`
- **描述**：哈希表工具函数集合，提供通用哈希计算。
- **主要函数**：
  - [bfdev_hashtbl_init()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/hashtbl.h#L45-L52)：初始化哈希表。
  - `bfdev_hashtbl_hash()`：计算哈希值。
- **头文件**：`<bfdev/hashtbl.h>`
- **源文件**：`src/hashtbl.c`

#### `heap`
- **描述**：二叉堆，支持最大堆或最小堆。
- **主要函数**：
  - [bfdev_heap_init()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/heap.h#L78-L82)：初始化堆。
  - [bfdev_heap_insert()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/heap.h#L181-L189)：插入元素。
  - `bfdev_heap_extract()`：提取堆顶元素。
- **头文件**：`<bfdev/heap.h>`
- **源文件**：[src/heap.c](https://github.com/openbfdev/bfdev/blob/master/src/heap.c)

#### `hlist`
- **描述**：哈希链表，基于哈希桶组织的链表结构。
- **主要函数**：
  - `bfdev_hlist_add()`：向链表中添加节点。
  - [bfdev_hlist_del()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/hlist.h#L155-L166)：从链表中删除节点。
- **头文件**：`<bfdev/hlist.h>`
- **源文件**：`src/hlist.c`

#### `ilist`
- **描述**：索引链表，通过索引访问链表节点。
- **主要函数**：
  - [bfdev_ilist_add()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/ilist.h#L51-L53)：添加节点。
  - `bfdev_ilist_get()`：通过索引获取节点。
- **头文件**：`<bfdev/ilist.h>`
- **源文件**：[src/ilist.c](https://github.com/openbfdev/bfdev/blob/master/src/ilist.c)

#### `list`
- **描述**：双向链表，常用基础链表结构。
- **主要函数**：
  - [bfdev_list_add()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/list.h#L81-L85)：添加节点。
  - [bfdev_list_del()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/list.h#L113-L124)：删除节点。
- **头文件**：`<bfdev/list.h>`
- **源文件**：`src/list.c`

#### `llist`
- **描述**：无锁链表，适用于并发环境。
- **主要函数**：
  - [bfdev_llist_add()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/llist.h#L41-L45)：添加节点。
  - [bfdev_llist_del()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/llist.h#L31-L32)：删除节点。
- **头文件**：`<bfdev/llist.h>`
- **源文件**：[src/llist.c](https://github.com/openbfdev/bfdev/blob/master/src/llist.c)

#### `radix`
- **描述**：基数树，适用于高效 IP 地址查找等场景。
- **主要函数**：
  - `bfdev_radix_insert()`：插入键。
  - `bfdev_radix_lookup()`：查找键。
- **头文件**：`<bfdev/radix.h>`
- **源文件**：[src/radix.c](https://github.com/openbfdev/bfdev/blob/master/src/radix.c)

#### `rbtree`
- **描述**：红黑树，自平衡二叉搜索树。
- **主要函数**：
  - `bfdev_rbtree_insert()`：插入节点。
  - `bfdev_rbtree_delete()`：删除节点。
- **头文件**：`<bfdev/rbtree.h>`
- **源文件**：[src/rbtree.c](https://github.com/openbfdev/bfdev/blob/master/src/rbtree.c)

#### `ringbuf`
- **描述**：环形缓冲区，适用于流式数据处理。
- **主要函数**：
  - `bfdev_ringbuf_init()`：初始化环形缓冲区。
  - `bfdev_ringbuf_write()`：写入数据。
  - `bfdev_ringbuf_read()`：读取数据。
- **头文件**：`<bfdev/ringbuf.h>`
- **源文件**：[src/ringbuf.c](https://github.com/openbfdev/bfdev/blob/master/src/ringbuf.c)

#### `segtree`
- **描述**：线段树，适用于区间查询与更新。
- **主要函数**：
  - `bfdev_segtree_init()`：初始化线段树。
  - `bfdev_segtree_query()`：区间查询。
- **头文件**：`<bfdev/segtree.h>`
- **源文件**：[src/segtree.c](https://github.com/openbfdev/bfdev/blob/master/src/segtree.c)

#### `skiplist`
- **描述**：跳表，支持高效的查找、插入和删除。
- **主要函数**：
  - [bfdev_skiplist_insert()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/skiplist.h#L47-L49)：插入节点。
  - `bfdev_skiplist_search()`：查找节点。
- **头文件**：`<bfdev/skiplist.h>`
- **源文件**：[src/skiplist.c](https://github.com/openbfdev/bfdev/blob/master/src/skiplist.c)

#### `slist`
- **描述**：单向链表，内存占用小，适用于轻量级场景。
- **主要函数**：
  - [bfdev_slist_add()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/slist.h#L55-L65)：添加节点。
  - [bfdev_slist_del()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/slist.h#L72-L88)：删除节点。
- **头文件**：`<bfdev/slist.h>`
- **源文件**：`src/slist.c`

---

### **Bit Operation**

#### `bitrev`
- **描述**：位反转操作，适用于字节或整型数据。
- **主要函数**：
  - [bfdev_bitrev8()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/bitrev.h#L179-L185)：8 位位反转。
  - [bfdev_bitrev16()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/bitrev.h#L186-L192)：16 位位反转。
  - [bfdev_bitrev32()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/bitrev.h#L193-L199)：32 位位反转。
- **头文件**：`<bfdev/bitrev.h>`
- **源文件**：[src/bitrev.c](https://github.com/openbfdev/bfdev/blob/master/src/bitrev.c)

#### `bitwalk`
- **描述**：位图迭代器，用于遍历设置位（1）的位置。
- **主要函数**：
  - `bfdev_bitwalk_first()`：获取第一个置位索引。
  - `bfdev_bitwalk_next()`：获取下一个置位索引。
- **头文件**：`<bfdev/bitwalk.h>`
- **源文件**：[src/bitwalk.c](https://github.com/openbfdev/bfdev/blob/master/src/bitwalk.c)

#### `popcount`
- **描述**：统计给定数值中 1 的位数。
- **主要函数**：
  - [bfdev_popcount8()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/popcount.h#L174-L180)：统计 8 位值中的 1 的个数。
  - [bfdev_popcount32()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/popcount.h#L188-L194)：统计 32 位值中的 1 的个数。
- **头文件**：`<bfdev/popcount.h>`
- **源文件**：[src/popcount.c](https://github.com/openbfdev/bfdev/blob/master/src/popcount.c)

---

### **Architecture**

#### `atomic`
- **描述**：原子操作接口，保证多线程安全。
- **主要函数**：
  - `bfdev_atomic_inc()`：原子递增。
  - `bfdev_atomic_dec()`：原子递减。
  - `bfdev_atomic_xchg()`：原子交换。
- **头文件**：`<bfdev/atomic.h>`
- **源文件**：[src/atomic.c](https://github.com/openbfdev/bfdev/blob/master/src/atomic.c)

#### `byteorder`
- **描述**：字节序转换工具，支持大小端互换。
- **主要函数**：
  - [bfdev_cpu_to_be16()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/byteorder/big-endian.h#L47-L48)：CPU 字节序转大端 16 位。
  - [bfdev_be16_to_cpu()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/byteorder/big-endian.h#L44-L45)：大端 16 位转 CPU 字节序。
- **头文件**：`<bfdev/byteorder.h>`
- **源文件**：[src/byteorder.c](https://github.com/openbfdev/bfdev/blob/master/src/byteorder.c)

#### `cmpxchg`
- **描述**：比较并交换（CAS），用于无锁编程。
- **主要函数**：
  - `bfdev_cmpxchg()`：比较并交换指定内存地址的内容。
- **头文件**：`<bfdev/cmpxchg.h>`
- **源文件**：[src/cmpxchg.c](https://github.com/openbfdev/bfdev/blob/master/src/cmpxchg.c)

#### `swab`
- **描述**：字节交换，用于双字节或四字节数据交换。
- **主要函数**：
  - [bfdev_swab16()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/swab.h#L160-L165)：16 位字节交换。
  - [bfdev_swab32()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/swab.h#L174-L179)：32 位字节交换。
- **头文件**：`<bfdev/swab.h>`
- **源文件**：[src/swab.c](https://github.com/openbfdev/bfdev/blob/master/src/swab.c)

---

### **Memory Allocator**

#### `allocator`
- **描述**：通用内存分配兼容层，封装多种分配器接口。
- **主要函数**：
  - [bfdev_malloc()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/allocator.h#L79-L80)：内存分配。
  - [bfdev_free()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/allocator.h#L89-L90)：释放内存。
- **头文件**：`<bfdev/allocator.h>`
- **源文件**：[src/allocator.c](https://github.com/openbfdev/bfdev/blob/master/src/allocator.c)

#### `allocpool`
- **描述**：高性能内存池，优化频繁小块内存申请。
- **主要函数**：
  - `bfdev_allocpool_create()`：创建内存池。
  - [bfdev_allocpool_alloc()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/allocpool.h#L62-L64)：从池中分配内存。
- **头文件**：`<bfdev/allocpool.h>`
- **源文件**：[src/allocpool.c](https://github.com/openbfdev/bfdev/blob/master/src/allocpool.c)

#### `memalloc`
- **描述**：内存分配算法实现，如 slab、buddy 等。
- **主要函数**：
  - [bfdev_memalloc_init()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/memalloc.h#L94-L96)：初始化内存分配器。
  - `bfdev_memalloc_alloc()`：动态内存分配。
- **头文件**：`<bfdev/memalloc.h>`
- **源文件**：[src/memalloc.c](https://github.com/openbfdev/bfdev/blob/master/src/memalloc.c)

---

### **String Process**

#### `argv`
- **描述**：命令行参数分割工具。
- **主要函数**：
  - `bfdev_argv_split()`：按空格分隔字符串为参数数组。
- **头文件**：`<bfdev/argv.h>`
- **源文件**：[src/argv.c](https://github.com/openbfdev/bfdev/blob/master/src/argv.c)

#### `fsm`
- **描述**：有限状态机框架，用于解析复杂输入流。
- **主要函数**：
  - [bfdev_fsm_handle()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/fsm.h#L206-L207)：处理状态迁移事件。
- **头文件**：`<bfdev/fsm.h>`
- **源文件**：[src/fsm.c](https://github.com/openbfdev/bfdev/blob/master/src/fsm.c)

#### `scnprintf`
- **描述**：安全格式化输出，防止缓冲区溢出。
- **主要函数**：
  - `bfdev_scnprintf()`：格式化写入指定长度缓冲区。
- **头文件**：`<bfdev/scnprintf.h>`
- **源文件**：[src/scnprintf.c](https://github.com/openbfdev/bfdev/blob/master/src/scnprintf.c)

---

### **Mathematics**

#### `mpi`
- **描述**：大整数运算库，支持加减乘除等操作。
- **主要函数**：
  - [bfdev_mpi_add()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/mpi.h#L64-L66)：大整数加法。
  - [bfdev_mpi_mul()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/mpi.h#L72-L74)：大整数乘法。
- **头文件**：`<bfdev/mpi.h>`
- **源文件**：[src/mpi.c](https://github.com/openbfdev/bfdev/blob/master/src/mpi.c)

#### `log2`
- **描述**：计算 log₂ 值，支持动态和常量优化。
- **主要函数**：
  - [bfdev_ilog2()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/log2.h#L151-L159)：计算 32 位整数的 log₂。
- **头文件**：`<bfdev/log2.h>`
- **源文件**：[src/log2.c](https://github.com/openbfdev/bfdev/blob/master/src/log2.c)

---

### **Rate limiting**

#### `ratelimit`
- **描述**：固定窗口计数限速器，控制单位时间请求频率。
- **主要函数**：
  - `bfdev_ratelimit_check()`：检查是否超过速率限制。
- **头文件**：`<bfdev/ratelimit.h>`
- **源文件**：[src/ratelimit.c](https://github.com/openbfdev/bfdev/blob/master/src/ratelimit.c)

#### `tokenbucket`
- **描述**：令牌桶限速器，支持突发流量。
- **主要函数**：
  - `bfdev_tokenbucket_consume()`：消耗令牌。
- **头文件**：`<bfdev/tokenbucket.h>`
- **源文件**：[src/tokenbucket.c](https://github.com/openbfdev/bfdev/blob/master/src/tokenbucket.c)

---

### **Cache**

#### `lfu`
- **描述**：最少使用缓存淘汰策略。
- **主要函数**：
  - `bfdev_cache_lfu_create()`：创建 LFU 缓存。
- **头文件**：`<bfdev/cache/lfu.h>`
- **源文件**：[src/cache/lfu.c](https://github.com/openbfdev/bfdev/blob/master/src/cache/lfu.c)

#### `lru`
- **描述**：最近最少使用缓存淘汰策略。
- **主要函数**：
  - `bfdev_cache_lru_create()`：创建 LRU 缓存。
- **头文件**：`<bfdev/cache/lru.h>`
- **源文件**：[src/cache/lru.c](https://github.com/openbfdev/bfdev/blob/master/src/cache/lru.c)

---

### **Textsearch**

#### `bm`
- **描述**：Boyer-Moore 高效字符串搜索算法。
- **主要函数**：
  - `bfdev_textsearch_bm_prepare()`：准备 BM 模式表。
  - `bfdev_textsearch_bm_search()`：执行 BM 搜索。
- **头文件**：`<bfdev/textsearch/bm.h>`
- **源文件**：[src/textsearch/bm.c](https://github.com/openbfdev/bfdev/blob/master/src/textsearch/bm.c)

#### `kmp`
- **描述**：Knuth-Morris-Pratt 字符串匹配算法。
- **主要函数**：
  - `bfdev_textsearch_kmp_prepare()`：预处理模式串。
  - `bfdev_textsearch_kmp_search()`：执行 KMP 匹配。
- **头文件**：`<bfdev/textsearch/kmp.h>`
- **源文件**：[src/textsearch/kmp.c](https://github.com/openbfdev/bfdev/blob/master/src/textsearch/kmp.c)

#### `sunday`
- **描述**：Sunday 快速字符串查找算法。
- **主要函数**：
  - `bfdev_textsearch_sunday_prepare()`：构建跳转表。
  - `bfdev_textsearch_sunday_search()`：执行 Sunday 查找。
- **头文件**：`<bfdev/textsearch/sunday.h>`
- **源文件**：[src/textsearch/sunday.c](https://github.com/openbfdev/bfdev/blob/master/src/textsearch/sunday.c)

---

### **Miscellaneous**

#### `guards`
- **描述**：变量自动清理机制，在作用域退出时自动释放资源。
- **主要函数**：
  - `bfdev_cleanup()`：注册清理回调。
- **头文件**：`<bfdev/guards.h>`
- **源文件**：[src/guards.c](https://github.com/openbfdev/bfdev/blob/master/src/guards.c)

#### `notifier`
- **描述**：通知链机制，用于事件订阅与广播。
- **主要函数**：
  - [bfdev_notifier_register()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/notifier.h#L85-L86)：注册通知回调。
  - [bfdev_notifier_call()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/notifier.h#L76-L78)：触发所有注册回调。
- **头文件**：`<bfdev/notifier.h>`
- **源文件**：[src/notifier.c](https://github.com/openbfdev/bfdev/blob/master/src/notifier.c)

#### `once`
- **描述**：确保代码段只执行一次，常用于初始化逻辑。
- **主要函数**：
  - [BFDEV_DO_ONCE()](https://github.com/openbfdev/bfdev/blob/master/include/bfdev/once.h#L34-L36)：宏定义确保单次执行。
- **头文件**：`<bfdev/once.h>`
- **源文件**：[src/once.c](https://github.com/openbfdev/bfdev/blob/master/src/once.c)

