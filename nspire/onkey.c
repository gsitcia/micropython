#include <stdint.h>
#include <nucleus.h>
#include <syscall-decls.h>

#include "onkey.h"

extern void __crt0_exit();
extern void nsp_texture_deinit();
extern void __cpp_fini();
extern char *heap;

static uint32_t handler, mask;

static void __attribute((used)) emergencyExit()
{
	unregisterOnkey();

	//Tidy up at least a slight bit before exiting.
	free(heap);
	nsp_texture_deinit();
	__cpp_fini();

	exit(0);
}

static void __attribute__((interrupt("IRQ"))) irqHandler()
{
	//Acknowledge interrupt
        *(volatile uint32_t*)(0x900B0010) = 0;
        *(volatile uint32_t*)(0x900B0014) = 1;

	//Jump to emergencyExit after IRQ handled
	asm volatile(	"ldr lr, =emergencyExit\n"
			"add lr, lr, #4\n"
			"subs pc, lr, #4\n");

	__builtin_unreachable();
}

void registerOnkey()
{
	volatile uint32_t *irq = (volatile uint32_t*) 0x38;
	handler = *irq;
	*irq = (volatile uint32_t) irqHandler;

	*(volatile uint32_t*)(0x900B0010) = 1;
	*(volatile uint32_t*)(0x900B0014) = 1;

	// Mask everything except IRQ 15
	*(volatile uint32_t*) IO(0xDC00000C, 0xDC000014) = ~0;
	*(volatile uint32_t*) IO(0xDC000008, 0xDC000010) = 1 << 15;
	mask = *(volatile uint32_t*) IO(0xDC000008, 0xDC000010); 

	asm volatile("mrs r0, cpsr;"
                        "bic r0, r0, #0x80;"
                        "msr cpsr_c, r0;" ::: "r0");
}

void unregisterOnkey()
{
	asm volatile("mrs r0, cpsr;"
                        "orr r0, r0, #0x80;"
                        "msr cpsr_c, r0;" ::: "r0");

	*(volatile uint32_t*)(0x900B0010) = 0;
	*(volatile uint32_t*)(0x900B0014) = 1;

	// Apply old mask again
	*(volatile uint32_t*) IO(0xDC00000C, 0xDC000014) = ~0;
	*(volatile uint32_t*) IO(0xDC000008, 0xDC000010) = mask;

	volatile uint32_t *irq = (volatile uint32_t*) 0x38;
	*irq = handler;
}
