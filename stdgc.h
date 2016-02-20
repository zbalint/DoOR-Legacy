#ifndef STDGC_STDGC_H
#define STDGC_STDGC_H

#include <stdlib.h>

#ifndef DEBUGMALLOC_H
#define malloc(X) mallocgc(X)
#define free(P) freegc(P)
#endif //DEBUGMALLOC_H

void *mallocgc(size_t size);

void freegc(void *ptr);

#endif //STDGC_STDGC_H
