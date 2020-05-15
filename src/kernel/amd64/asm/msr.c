#include <amd64/asm/msr.h>

static inline void __wrmsr(uint32_t msr, uint32_t low, uint32_t high)
{
	__asm__ volatile ("wrmsr"
		:
		: "c" (msr), "a" (low), "d" (high)
		: "memory"
	);
}

static inline uint64_t __rdmsr(uint32_t msr)
{
	uint64_t low, high;

	__asm__ volatile ("rdmsr"
		: "=a" (low), "=d" (high)
		: "c" (msr)
	);

	return (low | (high << 32));
}

void wrmsr(uint32_t msr, uint32_t low, uint32_t high)
{
	__wrmsr(msr, low, high);
}

void wrmsrl(uint32_t msr, uint64_t value)
{
	__wrmsr(msr, (uint32_t)(value & 0xFFFFFFFFULL), (uint32_t)(value >> 32));
}

uint64_t rdmsrl(uint32_t msr)
{
	return __rdmsr(msr);
}