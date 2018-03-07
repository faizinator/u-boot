#ifndef __DUMP_BACKTRACE_H__
#define __DUMP_BACKTRACE_H__

#if CONFIG_IS_ENABLED(BACKTRACE)
void dump_backtrace(void);
#else
void dump_backtrace()
{
	return;
}
#endif
extern void dump_backtrace_stm(u32 *stack, u32 instruction);
extern void dump_backtrace_entry(unsigned long where, unsigned long from, unsigned long frame);

#endif
