/*
 * Muuselabs ML-J2000 board support
 *
 * Copyright (c) 2020 Muuselabs SA
 * Author: Pieter Palmers <pieter@muuselabs.com>
 *
 * based upon halley2 board support 
 * (c) 2013 Ingenic Semiconductor
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <nand.h>
#include <net.h>
#include <netdev.h>
#include <asm/gpio.h>
#include <asm/arch/cpm.h>
#include <asm/arch/clk.h>
#include <asm/arch/mmc.h>

#define DEBUG
#include <i2c.h>

#define CONFIG_SYS_I2C_HT_ADDR 0xA0

/* the following must match the values in the regmap json */
#define EEPROM_ADDR_OFFSET 0x00000025 /* address offset of the 'EEPROM_ADDR' register */
#define EEPROM_DATA_OFFSET 0x00000026 /* address offset of the 'EEPROM_DATA' register */

extern int jz_net_initialize(bd_t *bis);

int board_early_init_f(void)
{
	return 0;
}

int board_early_init_r(void)
{
	return 0;

}

#ifdef CONFIG_USB_GADGET
int jz_udc_probe(void);
void board_usb_init(void)
{
	printf("USB_udc_probe\n");
	jz_udc_probe();
}
#endif /* CONFIG_USB_GADGET */

#ifdef CONFIG_USB_DWC2
int board_usb_init_host(void /**/)
{
	/*Drive VBUS always ON in host mode.*/
	gpio_direction_output(CONFIG_USB_DRV_VBUS, 1);

	otg_phy_init(OTG_MODE, CONFIG_SYS_EXTAL);
	return 0;
}
#endif	/* CONFIG_USB_DWC2 */

int misc_init_r(void)
{
#if 0 /* TO DO */
	uint8_t mac[6] = { 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc };

	/* set MAC address */
	eth_setenv_enetaddr("ethaddr", mac);
#endif

	/* get the sysmode from the HT MCU */
	u8 sysmode = 0xFF;
    char buf[4];
	i2c_init (CONFIG_SYS_I2C_SPEED, 0xFE);

	i2c_reg_write(CONFIG_SYS_I2C_HT_ADDR, EEPROM_ADDR_OFFSET, 0);
	sysmode = i2c_reg_read(CONFIG_SYS_I2C_HT_ADDR, EEPROM_DATA_OFFSET);
    sprintf(buf, "%d", sysmode);
	setenv ("ht_sysmode", buf);

	return 0;
}

#ifdef CONFIG_MMC
int board_mmc_init(bd_t *bd)
{
	jz_mmc_init();
	return 0;
}
#endif

/* U-Boot common routines */
int checkboard(void)
{
	puts("Board: ML-J2000\n");
	return 0;
}

#ifdef CONFIG_SPL_BUILD

void spl_board_init(void)
{
}

#endif /* CONFIG_SPL_BUILD */
