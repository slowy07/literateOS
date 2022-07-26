#ifndef _LIBALLOC_H
#define _LIBALLOC_H

#include <stdint.h>
#include <stddef.h>

/**
  * these are the OS specific which need to
  * be implemented on any platform that the
  * library is expected to work on.
  **/

/** @{ */
#define PREFIX(func) k ## func

#ifdef __cplusplus
extern "C" {
#endif

  /**
    * this function is supposed to lock the memory data structure
    * it could be as simple as disabling interrupts or acquiring a spinlock
    * return 0 if the lock wa acquired successfully. anything else is
    * failure
    */
  extern int liballoc_lock();

  /**
    * this function unlocks what was previously locked by the liballoc_lock
    * function. if it disabled interrupts, it enables interrupts. if it
    * had acquired a spinlock, it release the spinlock, etc
    */
  extern int liballoc_unlock();

  /**
    * this frees previoulsy allocated memory. The void* parameter passed
    * to the function is the exact same value returned from a previous
    * liballoc_alloc call.
    * Then integer value is the number of pages to free.
    * return 0 if the memory was successfully freed
    */
  extern int liballoc_free(void* size_t);
  extern int templiballoc_unlock();

#if defined DEBUG || defined INFO || true
  void liballoc_dump();
#endif

  // the standard function
  extern void *PREFIX(malloc)(size_t);
  // the standard function
  extern void *PREFIX(realloc)(void *, size_t);
  // the standard function
  extern void *PREFIX(calloc)(size_t, size_t);
  // the standard function
  extern void PREFIX(free)(void *);
  extern void *PREFIX(tempmalloc)(size_t req_size);

#ifdef __cplusplus
}
#endif
/** @} */
#endif
