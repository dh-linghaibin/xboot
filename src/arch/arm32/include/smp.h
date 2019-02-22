#ifndef __ARM32_SMP_H__
#define __ARM32_SMP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <arm32.h>

#if __ARM32_ARCH__ == 5
static inline int smp_processor_id(void)
{
	return 0;
}
#else
static inline int smp_processor_id(void)
{
	// int tmp;

	// __asm__ __volatile__(
	// 	"mrc p15, 0, %0, c0, c0, 5\n"
	// 	"and %0, %0, #0xf\n"
	// 	: "=r" (tmp)
	// 	:
	// 	: "memory");
	// return tmp;
	return 0;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* __ARM32_SMP_H__ */
