#include "printers.h"

void PrintRegisters(const Registers *regs)
{
    printf("ds=%d\nedi=%d esi=%d ebp=%d ebx=%d edx=%d ecx=%d eax=%d\n",
           regs->ds, regs->edi, regs->esi, regs->ebp, regs->ebx, regs->edx, regs->ecx, regs->eax);
    printf("interrupt=%d error=%d\n", regs->interrupt, regs->error);
    printf("eip=%d cs=%d eflags=%d esp=%d ss=%d\n",
           regs->eip, regs->cs, regs->eflags, regs->esp, regs->ss);
}