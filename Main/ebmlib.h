#ifndef _EBMLIB_H_
#define _EBMLIB_H_

int ebmlib_init();
void ebmlib_exit();
int ebmlib_rd(int);
signed char ebmlib_rd8(int);
short ebmlib_rd16(int);
void ebmlib_wr(int, int);
void ebmlib_wr8(int, char);
void ebmlib_wr16(int, short);

#endif /* _EBMLIB_H_ */
