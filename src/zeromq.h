#if !defined(__ZEROMQ_H__)
#define __ZEROMQ_H

int zeromq_init();
int zeromq_newblock(int height);
void zeromq_term(); 

#endif
