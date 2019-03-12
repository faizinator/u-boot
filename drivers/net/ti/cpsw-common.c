// SPDX-License-Identifier: GPL-2.0+
/*
 * CPSW common - libs used across TI ethernet devices.
 *
 * Copyright (C) 2016, Texas Instruments, Incorporated
 */

#include <common.h>
#include <dm.h>
#include <environment.h>
#include <fdt_support.h>
#include <asm/io.h>
#include <cpsw.h>

DECLARE_GLOBAL_DATA_PTR;

#define CTRL_MAC_REG(offset, id) ((offset) + 0x8 * (id))

static void davinci_emac_3517_get_macid(u8 *mac_addr)
{
	/* try reading mac address from efuse */
	u32 macid_lsb = readl(addr);
	u32 macid_msb = readl(addr + 4);

	mac_addr[0] = (macid_msb >> 16) & 0xff;
	mac_addr[1] = (macid_msb >> 8)  & 0xff;
	mac_addr[2] = macid_msb & 0xff;
	mac_addr[3] = (macid_lsb >> 16) & 0xff;
	mac_addr[4] = (macid_lsb >> 8)  & 0xff;
	mac_addr[5] = macid_lsb & 0xff;
}

static void cpsw_am33xx_cm_get_macid(u8 *mac_addr)
{
	/* try reading mac address from efuse */
	u32 macid_lo = readl(addr);
	u32 macid_hi = readl(addr + 4);

	mac_addr[5] = (macid_lo >> 8) & 0xff;
	mac_addr[4] = macid_lo & 0xff;
	mac_addr[3] = (macid_hi >> 24) & 0xff;
	mac_addr[2] = (macid_hi >> 16) & 0xff;
	mac_addr[1] = (macid_hi >> 8) & 0xff;
	mac_addr[0] = macid_hi & 0xff;
}

int ti_cm_get_macid(struct udevice *dev, u8 *mac_addr)
{
	if (of_machine_is_compatible("ti,dm8148"))
		cpsw_am33xx_cm_get_macid(mac_addr);
	else if (of_machine_is_compatible("ti,am33xx"))
		cpsw_am33xx_cm_get_macid(mac_addr);
	else if (device_is_compatible(dev, "ti,am3517-emac"))
		davinci_emac_3517_get_macid(mac_addr);
	else if (device_is_compatible(dev, "ti,dm816-emac"))
		cpsw_am33xx_cm_get_macid(mac_addr);
	else if (of_machine_is_compatible("ti,am43"))
		cpsw_am33xx_cm_get_macid(mac_addr);
	else if (of_machine_is_compatible("ti,dra7"))
		davinci_emac_3517_get_macid(mac_addr);
} 

int ti_cm_get_macid_addr(struct udevice *dev, int slave, u32 *addr)
{
	void *fdt = (void *)gd->fdt_blob;
	int node = dev_of_offset(dev);
	fdt32_t gmii = 0;
	int syscon;
	u16 offset;

	if (of_machine_is_compatible("ti,dm8148")) {
		offset = 0x630;
	} else if (of_machine_is_compatible("ti,am33xx")) {
		offset = 0x630;
	} else if (device_is_compatible(dev, "ti,am3517-emac")) {
		offset = 0x110;
	} else if (device_is_compatible(dev, "ti,dm816-emac")) {
		offset = 0x30;
	} else if (of_machine_is_compatible("ti,am43")) {
		offset = 0x630;
	} else if (of_machine_is_compatible("ti,dra7")) {
		offset = 0x514;
	else {
		dev_err(dev, "incompatible machine/device type for reading mac address\n");
		return -ENOENT;
	}

	syscon = fdtdec_lookup_phandle(fdt, node, "syscon");
	if (syscon < 0) {
		pr_err("Syscon offset not found\n");
		return -ENOENT;
	}

	addr = (u32)map_physmem(fdt_translate_address(fdt, syscon, &gmii),
				sizeof(u32), MAP_NOCACHE);
	if (addr == FDT_ADDR_T_NONE) {
		pr_err("Not able to get syscon address to get mac efuse address\n");
		return -ENOENT;
	}

	addr += CTRL_MAC_REG(offset, slave);

	return 0;

}
