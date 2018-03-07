#include <stdio.h>
#include "../include/asm/bug.h"
#include <dump_backtrace.h>
#include <linux/bitops.h>
#include <common.h>

DECLARE_GLOBAL_DATA_PTR;

extern const char system_map[] __attribute__((weak));

void dump_backtrace()
{
	unsigned int fp, mode;

	printf("\nBacktrace:\n");
	asm("mov %0, fp" : "=r" (fp) : : "cc");
	mode = 0x10;
	c_backtrace(fp, mode);

}
void dump_backtrace_entry(unsigned long where, unsigned long from, unsigned long frame)
{
	unsigned long base, base2;
	char *str, *str1;

	where -= gd->reloc_off;
	from -= gd->reloc_off;
	str = symbol_lookup(where, &base);
	str1 = symbol_lookup(from, &base2);
	printk("[<%08lx>] (%s) from [<%08lx>] (%s + 0x%x)\n", where, str, from, str1, from - base2);
}

void dump_backtrace_stm(u32 *stack, u32 instruction)
{
	char str[80], *p;
	unsigned int x;
	int reg;

	for (reg = 10, x = 0, p = str; reg >= 0; reg--) {
		if (instruction & BIT(reg)) {
			p += sprintf(p, " r%d:%08x", reg, *stack--);
			if (++x == 6) {
				x = 0;
				p = str;
				printk("%s\n", str);
			}
		}
	}
	if (p != str)
		printk("%s\n", str);
}
