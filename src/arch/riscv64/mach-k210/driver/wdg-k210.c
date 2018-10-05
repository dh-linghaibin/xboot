/*
 * driver/wdg-k210.c
 *
 * Copyright(c) 2007-2018 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>
#include <clk/clk.h>
#include <reset/reset.h>
#include <watchdog/watchdog.h>

enum {
	WDG_CR				= 0x00,
	WDG_TORR			= 0x04,
	WDG_CCVR			= 0x08,
	WDG_CRR				= 0x0c,
	WDG_STAT			= 0x10,
	WDG_EOI				= 0x14,
	WDG_PROT_LEVEL		= 0x1c,
	WDG_COMP_PARAM5		= 0xe4,
	WDG_COMP_PARAM4		= 0xe8,
	WDG_COMP_PARAM3		= 0xec,
	WDG_COMP_PARAM2		= 0xf0,
	WDG_COMP_PARAM1		= 0xf4,
	WDG_COMP_VERSION	= 0xf8,
	WDG_COMP_TYPE		= 0xfc,
};

struct wdg_k210_pdata_t {
	virtual_addr_t virt;
	char * clk;
	int reset;
};

static void wdg_k210_set(struct watchdog_t * watchdog, int timeout)
{
	struct wdg_k210_pdata_t * pdat = (struct wdg_k210_pdata_t *)watchdog->priv;
	u64_t top;
	u8_t v;

	if(timeout < 0)
		timeout = 0;

	if(timeout > 0)
	{
		top = (timeout * clk_get_rate(pdat->clk)) >> 16;
		if(top)
			top = (uint32_t)ilog2(top);
		if(top > 0xf)
			top = 0xf;
		v = (u8_t)top;
		write32(pdat->virt + WDG_TORR, (v << 4 | v << 0) & 0xff);
		write32(pdat->virt + WDG_CRR, 0x76);
		write32(pdat->virt + WDG_CR, read32(pdat->virt + WDG_CR) | (1 << 0));
	}
	else
	{
		write32(pdat->virt + WDG_CRR, 0x76);
		write32(pdat->virt + WDG_CR, read32(pdat->virt + WDG_CR) & ~(1 << 0));
	}
}

static int wdg_k210_get(struct watchdog_t * watchdog)
{
	return 0;
}

static struct device_t * wdg_k210_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct wdg_k210_pdata_t * pdat;
	struct watchdog_t * wdg;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * clk = dt_read_string(n, "clock-name", NULL);

	if(!search_clk(clk))
		return NULL;

	pdat = malloc(sizeof(struct wdg_k210_pdata_t));
	if(!pdat)
		return NULL;

	wdg = malloc(sizeof(struct watchdog_t));
	if(!wdg)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt = virt;
	pdat->clk = strdup(clk);
	pdat->reset = dt_read_int(n, "reset", -1);

	wdg->name = alloc_device_name(dt_read_name(n), -1);
	wdg->set = wdg_k210_set;
	wdg->get = wdg_k210_get;
	wdg->priv = pdat;

	if(pdat->reset >= 0)
		reset_deassert(pdat->reset);
	clk_enable(pdat->clk);
	write32(pdat->virt + WDG_CRR, 0x76);
	write32(pdat->virt + WDG_CR, read32(pdat->virt + WDG_CR) & ~(1 << 0));
	write32(pdat->virt + WDG_EOI, read32(pdat->virt + WDG_EOI));
	write32(pdat->virt + WDG_CR, read32(pdat->virt + WDG_CR) & ~(1 << 1));

	if(!register_watchdog(&dev, wdg))
	{
		write32(pdat->virt + WDG_CRR, 0x76);
		write32(pdat->virt + WDG_CR, read32(pdat->virt + WDG_CR) & ~(1 << 0));
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(wdg->name);
		free(wdg->priv);
		free(wdg);
		return NULL;
	}
	dev->driver = drv;

	return dev;
}

static void wdg_k210_remove(struct device_t * dev)
{
	struct watchdog_t * wdg = (struct watchdog_t *)dev->priv;
	struct wdg_k210_pdata_t * pdat = (struct wdg_k210_pdata_t *)wdg->priv;

	if(wdg && unregister_watchdog(wdg))
	{
		write32(pdat->virt + WDG_CRR, 0x76);
		write32(pdat->virt + WDG_CR, read32(pdat->virt + WDG_CR) & ~(1 << 0));
		clk_disable(pdat->clk);
		free(pdat->clk);

		free_device_name(wdg->name);
		free(wdg->priv);
		free(wdg);
	}
}

static void wdg_k210_suspend(struct device_t * dev)
{
}

static void wdg_k210_resume(struct device_t * dev)
{
}

static struct driver_t wdg_k210 = {
	.name		= "wdg-k210",
	.probe		= wdg_k210_probe,
	.remove		= wdg_k210_remove,
	.suspend	= wdg_k210_suspend,
	.resume		= wdg_k210_resume,
};

static __init void wdg_k210_driver_init(void)
{
	register_driver(&wdg_k210);
}

static __exit void wdg_k210_driver_exit(void)
{
	unregister_driver(&wdg_k210);
}

driver_initcall(wdg_k210_driver_init);
driver_exitcall(wdg_k210_driver_exit);
