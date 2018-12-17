/*
 * XAG XLINKHS board support
 *
 * Copyright (c) 2011 Qualcomm Atheros
 * Copyright (c) 2011-2012 Gabor Juhos <juhosg@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <linux/irq.h>
#include <linux/pci.h>
#include <linux/phy.h>
#include <linux/platform_device.h>
#include <linux/platform_data/phy-at803x.h>
#include <linux/ath9k_platform.h>
#include <linux/ar8216_platform.h>
#include <linux/export.h>

#include <asm/mach-ath79/ar71xx_regs.h>

#include "pci.h"
#include "common.h"
#include "dev-ap9x-pci.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-nfc.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"


#define XAG9342_GPIO_LED_USB		11
#define XAG9342_GPIO_LED_WLAN		12
#define XAG9342_GPIO_LED_STS		14

#define XAG9342_GPIO_BTN_RESET		16

#define XAG9342_KEYS_POLL_INTERVAL	20 /* msecs */
#define XAG9342_KEYS_DEBOUNCE_INTERVAL	(3 * XAG9342_KEYS_POLL_INTERVAL)

#define XAG9342_MAC0_OFFSET		0x0

#define XAG9342_WMAC_CALDATA_OFFSET	0x1000
#define XAG9342_PCIE_CALDATA_OFFSET	0x5000

#define XAG9342_ART_SIZE		0x8000


static struct gpio_led xag9342_leds_gpio[] __initdata = {
	{
		.name		= "xag9342:usb",
		.gpio		= XAG9342_GPIO_LED_USB,
		.active_low	= 1,
	},
	{
		.name		= "xag9342:wlan",
		.gpio		= XAG9342_GPIO_LED_WLAN,
		.active_low	= 1,
	},
	{
		.name		= "xag9342:sts",
		.gpio		= XAG9342_GPIO_LED_STS,
		.active_low	= 1,
	},
};

static struct gpio_keys_button xag9342_gpio_keys[] __initdata = {
	{
		.desc		= "reset",
		.type		= EV_KEY,
		.code		= KEY_RESTART,
		.debounce_interval = XAG9342_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= XAG9342_GPIO_BTN_RESET,
		.active_low	= 1,
	},
};


static struct at803x_platform_data mi124_ar8035_data = {
		.disable_smarteee = 0,
		.enable_rgmii_rx_delay = 1,
		.enable_rgmii_tx_delay = 0,
		.fixup_rgmii_tx_delay = 1,
};

static struct mdio_board_info mi124_mdio0_info[] = {
        {
                .bus_id = "ar71xx-mdio.0",
                .phy_addr = 0,
                .platform_data = &mi124_ar8035_data,
        },
};

static void __init xag9342_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);
	ath79_register_leds_gpio(-1, ARRAY_SIZE(xag9342_leds_gpio),
				xag9342_leds_gpio);

	ath79_register_gpio_keys_polled(-1, XAG9342_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(xag9342_gpio_keys),
					xag9342_gpio_keys);

	ath79_register_usb();

	ath79_register_wmac(art + XAG9342_WMAC_CALDATA_OFFSET, NULL);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + XAG9342_MAC0_OFFSET, 0);

	ath79_setup_ar934x_eth_cfg(AR934X_ETH_CFG_RGMII_GMAC0 |
			   AR934X_ETH_CFG_RXD_DELAY | AR934X_ETH_CFG_RDV_DELAY);

	//ath79_register_pci();
	//ath79_register_mdio(1, 0x0);
	ath79_register_mdio(0, 0x0);

	mdiobus_register_board_info(mi124_mdio0_info, ARRAY_SIZE(mi124_mdio0_info));

	

	/* GMAC0 is connected to an AR8035 Gigabit PHY */
	ath79_eth0_data.mii_bus_dev = &ath79_mdio0_device.dev;
	ath79_eth0_data.phy_if_mode = PHY_INTERFACE_MODE_RGMII;
	ath79_eth0_data.phy_mask = BIT(0);	
	ath79_eth0_pll_data.pll_1000 = 0x03000000;
	ath79_eth0_pll_data.pll_100 = 0x0101;
	ath79_eth0_pll_data.pll_10 = 0x1313;
	ath79_register_eth(0);

}

MIPS_MACHINE(ATH79_MACH_XAG9342, "XAG9342", "XAG XLinkHS", xag9342_setup);
