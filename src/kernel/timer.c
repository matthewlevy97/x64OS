#include <amd64/pic.h>
#include <amd64/timer/pit_8254.h>
#include <kernel/atomic.h>
#include <kernel/timer.h>
#include <assert.h>

static void (*handler_stage2)();

/**
 * Generaic timer wrapper
 */
void timer_init()
{
	pit_init();
}

void timer_register_interrupt_callback(void (*handler)())
{
	handler_stage2 = handler;

	register_interrupt_handler(IRQ_TO_ISR(0), timer_interrupt_handler);
	pic_write_mask(pic_get_mask() | PIC_INTERNAL_TIMER);
}

registers_t *timer_interrupt_handler(registers_t *regs)
{
	atomic_set_counter(1);

	pic_acknowledge(regs->int_no);

	handler_stage2();

	ASSERT(atomic_counter() == 1);
	atomic_set_counter(0);

	return regs;
}