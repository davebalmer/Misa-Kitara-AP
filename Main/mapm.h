#ifndef _MAPM_SASI
#define _MAPM_SASI


#define	MAPM_ERR_MAP	((void *)-1)
#define	MAPM_ERR_FILE	((void *)-2)


#ifdef __cplusplus
extern "C"{
#endif


void	*mapm(unsigned int addr, unsigned int len);

void	*fmapm(int fd, unsigned int addr, unsigned int len);

int		unmapm(void *addr, unsigned int len);


#ifdef __cplusplus
}
#endif

#endif

