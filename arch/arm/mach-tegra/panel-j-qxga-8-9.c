/*
 * arch/arm/mach-tegra/panel-s-wqxga-10-1.c
 *
 * Copyright (c) 2012-2014, NVIDIA CORPORATION. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <mach/dc.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/regulator/consumer.h>

#include "board.h"
#include "board-panel.h"
#include "devices.h"
#include "gpio-names.h"

#define DSI_PANEL_RESET		0

static bool reg_requested;
static struct regulator *avdd_lcd_3v3;
static struct regulator *vdd_lcd_bl;
static struct regulator *vdd_lcd_bl_en;
static struct regulator *dvdd_lcd_1v8;
static u16 en_panel_rst;

static int dalmore_dsi_regulator_get(struct device *dev)
{
	int err = 0;

	if (reg_requested)
		return 0;
	dvdd_lcd_1v8 = regulator_get(dev, "dvdd_lcd2");
	if (IS_ERR(dvdd_lcd_1v8)) {
		pr_info("dvdd_lcd regulator get failed\n");
		err = PTR_ERR(dvdd_lcd_1v8);
		dvdd_lcd_1v8 = NULL;
		goto fail;
	}
	avdd_lcd_3v3 = regulator_get(dev, "avdd_lcd2");
	if (IS_ERR(avdd_lcd_3v3)) {
		pr_info("avdd_lcd regulator get failed\n");
		err = PTR_ERR(avdd_lcd_3v3);
		avdd_lcd_3v3 = NULL;
		goto fail;
	}

	vdd_lcd_bl = regulator_get(dev, "vdd_lcd_bl2");
	if (IS_ERR(vdd_lcd_bl)) {
		pr_info("vdd_lcd_bl regulator get failed\n");
		err = PTR_ERR(vdd_lcd_bl);
		vdd_lcd_bl = NULL;
		goto fail;
	}

	vdd_lcd_bl_en = regulator_get(dev, "vdd_lcd_bl_en2");
	if (IS_ERR(vdd_lcd_bl_en)) {
		pr_info("vdd_lcd_bl_en regulator get failed\n");
		err = PTR_ERR(vdd_lcd_bl_en);
		vdd_lcd_bl_en = NULL;
		goto fail;
	}
	reg_requested = true;
	return 0;
fail:
	return err;
}

static int dsi_j_qxga_8_9_postpoweron(struct device *dev)
{
	int err = 0;

	err = dalmore_dsi_regulator_get(dev);
	if (err < 0) {
		pr_info("dsi regulator get failed\n");
		goto fail;
	}

	err = tegra_panel_gpio_get_dt("j,qxga-8-9", &panel_of);
	if (err < 0) {
		pr_info("dsi gpio request failed\n");
		goto fail;
	}

	/* If panel rst gpio is specified in device tree,
	 * use that.
	 */
	if (gpio_is_valid(panel_of.panel_gpio[TEGRA_GPIO_RESET]))
		en_panel_rst = panel_of.panel_gpio[TEGRA_GPIO_RESET];

	if (dvdd_lcd_1v8) {
		err = regulator_enable(dvdd_lcd_1v8);
		if (err < 0) {
			pr_info("dvdd_lcd regulator enable failed\n");
			goto fail;
		}
	}

	if (avdd_lcd_3v3) {
		err = regulator_enable(avdd_lcd_3v3);
		if (err < 0) {
			pr_info("avdd_lcd regulator enable failed\n");
			goto fail;
		}
	}

	/* panel ic requirement after vcc enable */
	msleep(260);

	if (vdd_lcd_bl) {
		err = regulator_enable(vdd_lcd_bl);
		if (err < 0) {
			pr_info("vdd_lcd_bl regulator enable failed\n");
			goto fail;
		}
	}

	if (vdd_lcd_bl_en) {
		err = regulator_enable(vdd_lcd_bl_en);
		if (err < 0) {
			pr_info("vdd_lcd_bl_en regulator enable failed\n");
			goto fail;
		}
	}

	msleep(20);

/*	gpio_set_value(avdd_4v, 1);
	usleep_range(1 * 1000, 1 * 1000 + 500);
	gpio_set_value(dcdc_en, 1);
	usleep_range(15 * 1000, 15 * 1000 + 500);*/
	gpio_direction_output(en_panel_rst, 1);
	usleep_range(15 * 1000, 15 * 1000 + 500);

	return 0;
fail:
	return err;
}

static int dsi_j_qxga_8_9_enable(struct device *dev)
{
	int err = 0;
/*	gpio_set_value(iovdd_1v8, 1);
	usleep_range(15 * 1000, 15 * 1000 + 500);*/
	if (dvdd_lcd_1v8) {
		err = regulator_enable(dvdd_lcd_1v8);
		if (err < 0) {
			pr_info("dvdd_lcd regulator enable failed\n");
		}
	}

	return 0;
}

static int dsi_j_qxga_8_9_disable(struct device *dev)
{
	if (vdd_lcd_bl)
		regulator_disable(vdd_lcd_bl);

	if (vdd_lcd_bl_en)
		regulator_disable(vdd_lcd_bl_en);

	if (avdd_lcd_3v3)
		regulator_disable(avdd_lcd_3v3);

	if (dvdd_lcd_1v8)
		regulator_disable(dvdd_lcd_1v8);

/*	gpio_set_value(lcm_rst, 0);
	msleep(1);
	gpio_set_value(dcdc_en, 0);
	msleep(15);
	gpio_set_value(avdd_4v, 0);
	gpio_set_value(iovdd_1v8, 0);
	msleep(10);*/

	return 0;
}

static int dsi_j_qxga_8_9_postsuspend(void)
{
	return 0;
}

static int dsi_j_qxga_8_9_bl_notify(struct device *dev, int brightness)
{
	struct backlight_device *bl = NULL;
	struct pwm_bl_data *pb = NULL;
	int cur_sd_brightness = atomic_read(&sd_brightness);
	bl = (struct backlight_device *)dev_get_drvdata(dev);
	pb = (struct pwm_bl_data *)dev_get_drvdata(&bl->dev);

	/* SD brightness is a percentage */
	brightness = (brightness * cur_sd_brightness) / 255;

	/* Apply any backlight response curve */
	if (brightness > 255)
		pr_info("Error: Brightness > 255!\n");
	else if (pb->bl_measured)
		brightness = pb->bl_measured[brightness];

	return brightness;
}

static int dsi_j_qxga_8_9_check_fb(struct device *dev, struct fb_info *info)
{
	struct platform_device *pdev = NULL;
	pdev = to_platform_device(bus_find_device_by_name(
		&platform_bus_type, NULL, "tegradc.0"));
	return info->device == &pdev->dev;
}

static struct pwm_bl_data_dt_ops  dsi_j_qxga_8_9_pwm_bl_ops = {
	.notify = dsi_j_qxga_8_9_bl_notify,
	.check_fb = dsi_j_qxga_8_9_check_fb,
	.blnode_compatible = "j,qxga-8-9-bl",
};

struct tegra_panel_ops dsi_j_qxga_8_9_ops = {
	.enable = dsi_j_qxga_8_9_enable,
	.disable = dsi_j_qxga_8_9_disable,
	.postpoweron = dsi_j_qxga_8_9_postpoweron,
	.postsuspend = dsi_j_qxga_8_9_postsuspend,
	.pwm_bl_ops = &dsi_j_qxga_8_9_pwm_bl_ops,
};
