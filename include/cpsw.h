/*
 * CPSW Ethernet Switch Driver
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _CPSW_H_
#define _CPSW_H_

/* reg offset */
#define CPSW_HOST_PORT_OFFSET	0x108
#define CPSW_SLAVE0_OFFSET	0x208
#define CPSW_SLAVE1_OFFSET	0x308
#define CPSW_SLAVE_SIZE		0x100
#define CPSW_CPDMA_OFFSET	0x800
#define CPSW_HW_STATS		0x900
#define CPSW_STATERAM_OFFSET	0xa00
#define CPSW_CPTS_OFFSET	0xc00
#define CPSW_ALE_OFFSET		0xd00
#define CPSW_SLIVER0_OFFSET	0xd80
#define CPSW_SLIVER1_OFFSET	0xdc0
#define CPSW_BD_OFFSET		0x2000
#define CPSW_MDIO_DIV		0xff

#define AM335X_GMII_SEL_OFFSET	0x630

/* DMA Registers */
#define CPDMA_TXCONTROL		0x004
#define CPDMA_RXCONTROL		0x014
#define CPDMA_SOFTRESET		0x01c
#define CPDMA_RXFREE		0x0e0
#define CPDMA_TXHDP_VER1	0x100
#define CPDMA_TXHDP_VER2	0x200
#define CPDMA_RXHDP_VER1	0x120
#define CPDMA_RXHDP_VER2	0x220
#define CPDMA_TXCP_VER1		0x140
#define CPDMA_TXCP_VER2		0x240
#define CPDMA_RXCP_VER1		0x160
#define CPDMA_RXCP_VER2		0x260

/* Descriptor mode bits */
#define CPDMA_DESC_SOP		BIT(31)
#define CPDMA_DESC_EOP		BIT(30)
#define CPDMA_DESC_OWNER	BIT(29)
#define CPDMA_DESC_EOQ		BIT(28)

struct cpsw_slave_data {
	u32		slave_reg_ofs;
	u32		sliver_reg_ofs;
	int		phy_addr;
	int		phy_if;
	int		phy_of_handle;
};

enum {
	CPSW_CTRL_VERSION_1 = 0,
	CPSW_CTRL_VERSION_2	/* am33xx like devices */
};

struct cpsw_platform_data {
	u32	mdio_base;
	u32	cpsw_base;
	u32	gmii_sel;
	int	mdio_div;
	int	channels;	/* number of cpdma channels (symmetric)	*/
	u32	cpdma_reg_ofs;	/* cpdma register offset		*/
	int	slaves;		/* number of slave cpgmac ports		*/
	u32	ale_reg_ofs;	/* address lookup engine reg offset	*/
	int	ale_entries;	/* ale table size			*/
	u32	host_port_reg_ofs;	/* cpdma host port registers	*/
	u32	hw_stats_reg_ofs;	/* cpsw hw stats counters	*/
	u32	bd_ram_ofs;		/* Buffer Descriptor RAM offset */
	u32	mac_control;
	struct cpsw_slave_data	*slave_data;
	void	(*control)(int enabled);
	u32	host_port_num;
	u32	active_slave;
	bool	rmii_clock_external;
	u8	version;
	const char *phy_sel_compat;
	u32 *addr;
};

int cpsw_register(struct cpsw_platform_data *data);
int ti_cm_get_macid_addr(struct udevice *dev, int slave, u32 *addr)
int ti_cm_get_macid(struct udevice *dev, u8 *mac_addr);
int cpsw_get_slave_phy_addr(struct udevice *dev, int slave);

#endif /* _CPSW_H_  */
