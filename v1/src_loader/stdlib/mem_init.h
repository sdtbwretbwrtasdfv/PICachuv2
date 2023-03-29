#pragma once

union chunk;
struct info {
  union chunk* prev;
  union chunk* next;
  size_t size;
};
struct node {
  struct info info;
  size_t used;
};
union chunk {
  unsigned char data[sizeof (struct node)];
  struct node node;
};

// static memory
enum {CHUNK_MAX = 1024};
static size_t initialized = 0;
static const size_t chunk_max = CHUNK_MAX;
static union chunk pool[CHUNK_MAX];
static union chunk head = {.node = {{NULL, NULL, 0}, 0}};
static union chunk tail = {.node = {{NULL, NULL, 0}, 0}};

static void _iinit() {
  head.node.info.next = pool;
  union chunk* prev = &head;
  union chunk* curr = pool;
  for (size_t i = 0; i < chunk_max; i++) {
    curr->node.info.prev = prev;
    curr->node.info.next = curr + 1;
    curr->node.info.size = 0;
    curr->node.used = 0;
    prev = curr++;
  }
  prev->node.info.next = &tail;
  tail.node.info.prev = prev;
  initialized = 1;
}

static size_t _cchunk_count(size_t size) {
  size_t chunks = 0;
  size_t chunks_size = 0;
  while (chunks_size < size + sizeof (struct info)) {
    chunks_size += sizeof (union chunk);
    chunks++;
  }
  return chunks;
}

static void* _uunlink(union chunk* top, size_t count) {
  union chunk* prev = top->node.info.prev;
  union chunk* next = top[count - 1].node.info.next;
  prev->node.info.next = next;
  next->node.info.prev = prev;
  top->node.info.size = count;
  return top->data + sizeof (struct info);
}

static void _rrelink(union chunk* top) {
  if (top < pool || pool + chunk_max - 1 < top) return;
  union chunk* prev = &head;
  while (prev->node.info.next != &tail && prev->node.info.next < top) {
    prev = prev->node.info.next;
  }
  if (prev == top) return; //dual free
  union chunk* next = prev->node.info.next;
  union chunk* curr = top;
  prev->node.info.next = curr;
  size_t count = top->node.info.size;
  for (size_t i = 0; i < count; i++) {
    curr->node.info.prev = prev;
    curr->node.info.next = curr + 1;
    curr->node.info.size = 0;
    curr->node.used = 0;
    prev = curr++;
  }
  prev->node.info.next = next;
  next->node.info.prev = prev;
}
