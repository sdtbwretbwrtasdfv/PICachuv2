#pragma once

extern void _free(void* ptr) {
  if (ptr == NULL) return;
  char* bptr = ptr;
  ptr = bptr - sizeof (struct info);
  _rrelink(ptr);
}
