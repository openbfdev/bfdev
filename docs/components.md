# Supporting Components

## Data Container

- array: Dynamic array, also with stack APIs
- bloom: Bloom filter
- btree: B+ tree
- circle: Circular queue
- fifo: First in first out (single read/write needn't lock)
- hashmap: Hash map with burst rehash
- hashtbl: Hash table tools
- heap: Binary heap tree
- hlist: Hash linked list
- ilist: Index linked list
- list: Double linked list
- llist: Lock free linked list
- radix: Radix tree
- rbtree: Red black tree
- ringbuf: Ring buffer
- segtree: Segment tree
- skiplist: Skip list
- slist: Single linked list

## Algorithms

- arc4: RC4 stream cipher
- ascii85: Ascii85 binary-to-text encoding
- base32: Base32 binary-to-text encoding
- base64: Base64 binary-to-text encoding
- bsearch: Array binary search
- crc: Cyclic redundancy check
- hash: Golden ratio Hash
- prandom: Pseudo random generator
- stringhash: String hash functions

## Bit Operation

- bcd: Binary-coded-decimal encoding
- bitfield: Bit padding functions
- bitmap: Bitmap operations
- bitops: Bit operations
- bitrev: Bitwise reversal
- bitwalk: Bitmap iterator
- popcount: Bitwise statistics

## Architecture

- atomic: Atomic operation functions
- byteorder: Byte order exchange
- cmpxchg: Atomic compare and exchange
- overflow: Saturation operations
- swab: Byte exchange functions
- unaligned: Non-aligned access functions

## Memory Allocator

- allocator: Allocation compatibility layer
- allocpool: Mempool optimized for allocation performance
- minpool: Simple memory allocator

## String Process

- argv: Argv segmentation
- fsm: Finite state machine
- levenshtein: Levenshtein edit distance
- scnprintf: Safe snprintf in buffer

## Mathematics

- align: Alignment functions
- dword: Double word calculate
- log2: Log2 calculate
- math: Mathematical methods
- minmax: Maximum and minimum value
- uplower: High and low bytes
- mpi: Multi precision integer

## Cache

- lfu: Least-frequently-used cache
- lru: Least-recently-used cache

## Textsearch

- bm: Boyer–Moore string-search algorithm
- kmp: Knuth–Morris–Pratt string-search algorithm
- sunday：Sunday string-search algorithm

## Miscellaneous

- action: Callback function framework
- callback: Dummy callbacks
- guards: Clear variable when goes out of scope
- log: Log framework
- notifier: Notifier chain
- once: Do once functions
