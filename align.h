/* Align a pointer to width bits */
#define ALIGN(ptr,width) (void *)((char *)ptr+(((int)ptr+width-1)&(width-1)))
