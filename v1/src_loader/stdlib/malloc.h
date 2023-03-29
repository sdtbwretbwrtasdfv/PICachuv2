#pragma once


extern void* _malloc(size_t size) {
  if (!initialized) _iinit();
  size_t chunks = _cchunk_count(size);
  size_t keep = 0;
  union chunk* top = head.node.info.next;
  for (union chunk* curr = top; curr != &tail; curr = curr->node.info.next) {
    keep++;
    if (keep == chunks) return _uunlink(top, chunks);
    if (curr->node.info.next == curr + 1) continue;
    keep = 0;
    top = curr->node.info.next;
  }
  return NULL;
}
