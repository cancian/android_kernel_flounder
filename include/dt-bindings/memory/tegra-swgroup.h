/*
 * This header provides constants for binding nvidia,swgroup ID
 *
 * Copyright (c) 2014 NVIDIA CORPORATION, All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#ifndef _DT_BINDINGS_MEMORY_TEGRA_SWGROUP_H
#define _DT_BINDINGS_MEMORY_TEGRA_SWGROUP_H

#define TEGRA_SWGROUP_AFI	0	/* 0x238 */
#define TEGRA_SWGROUP_AVPC	1	/* 0x23c */
#define TEGRA_SWGROUP_DC	2	/* 0x240 */
#define TEGRA_SWGROUP_DCB	3	/* 0x244 */
#define TEGRA_SWGROUP_EPP	4	/* 0x248 */
#define TEGRA_SWGROUP_G2	5	/* 0x24c */
#define TEGRA_SWGROUP_HC	6	/* 0x250 */
#define TEGRA_SWGROUP_HDA	7	/* 0x254 */
#define TEGRA_SWGROUP_ISP	8	/* 0x258 */
#define TEGRA_SWGROUP_ISP2	8
#define TEGRA_SWGROUP_DC14	9	/* 0x490 *//* Exceptional non-linear */
#define TEGRA_SWGROUP_DC12	10	/* 0xa88 *//* Exceptional non-linear */
#define TEGRA_SWGROUP_MPE	11	/* 0x264 */
#define TEGRA_SWGROUP_MSENC	11
#define TEGRA_SWGROUP_NV	12	/* 0x268 */
#define TEGRA_SWGROUP_NV2	13	/* 0x26c */
#define TEGRA_SWGROUP_PPCS	14	/* 0x270 */
#define TEGRA_SWGROUP_SATA2	15	/* 0x274 */
#define TEGRA_SWGROUP_SATA	16	/* 0x278 */
#define TEGRA_SWGROUP_VDE	17	/* 0x27c */
#define TEGRA_SWGROUP_VI	18	/* 0x280 */
#define TEGRA_SWGROUP_VIC	19	/* 0x284 */
#define TEGRA_SWGROUP_XUSB_HOST	20	/* 0x288 */
#define TEGRA_SWGROUP_XUSB_DEV	21	/* 0x28c */
#define TEGRA_SWGROUP_A9AVP	22	/* 0x290 */
#define TEGRA_SWGROUP_TSEC	23	/* 0x294 */
#define TEGRA_SWGROUP_PPCS1	24	/* 0x298 */
#define TEGRA_SWGROUP_SDMMC1A	25	/* 0xa94 *//* Linear shift again */
#define TEGRA_SWGROUP_SDMMC2A	26	/* 0xa98 */
#define TEGRA_SWGROUP_SDMMC3A	27	/* 0xa9c */
#define TEGRA_SWGROUP_SDMMC4A	28	/* 0xaa0 */
#define TEGRA_SWGROUP_ISP2B	29	/* 0xaa4 */
#define TEGRA_SWGROUP_GPU	30	/* 0xaa8 */
#define TEGRA_SWGROUP_GPUB	31	/* 0xaac */
#define TEGRA_SWGROUP_PPCS2	32	/* 0xab0 */
#define TEGRA_SWGID_APE		34	/* 0xab8 */
#define TEGRA_SWGID_SE		35	/* 0xabc */
#define TEGRA_SWGID_NVJPG	36	/* 0xac0 */
#define TEGRA_SWGID_HC1		37	/* 0xac4 */
#define TEGRA_SWGID_SE1		38	/* 0xac8 */
#define TEGRA_SWGID_AXIAP	39	/* 0xacc */
#define TEGRA_SWGID_ETR		40	/* 0xad0 */
#define TEGRA_SWGID_TSECB	41	/* 0xad4 */
#define TEGRA_SWGID_TSEC1	42	/* 0xad8 */
#define TEGRA_SWGID_TSECB1	43	/* 0xadc */

#define TWO_U32_OF_U64(x)	((x) & 0xffffffff) ((x) >> 32)
#define TEGRA_SWGROUP_BIT(x)	(1ULL << TEGRA_SWGROUP_##x)
#define TEGRA_SWGROUP_CELLS(x)	TWO_U32_OF_U64(TEGRA_SWGROUP_BIT(x))

#define TEGRA_SWGROUP_CELLS2(x1, x2)	 \
				TWO_U32_OF_U64( TEGRA_SWGROUP_BIT(x1) | \
						TEGRA_SWGROUP_BIT(x2))
#define TEGRA_SWGROUP_CELLS3(x1, x2, x3)	 \
				TWO_U32_OF_U64( TEGRA_SWGROUP_BIT(x1) | \
						TEGRA_SWGROUP_BIT(x2) | \
						TEGRA_SWGROUP_BIT(x3))
#define TEGRA_SWGROUP_CELLS4(x1, x2, x3, x4)	 \
				TWO_U32_OF_U64( TEGRA_SWGROUP_BIT(x1) | \
						TEGRA_SWGROUP_BIT(x2) | \
						TEGRA_SWGROUP_BIT(x3) | \
						TEGRA_SWGROUP_BIT(x4))

#define TEGRA_SWGROUP_MAX	64

#endif /* _DT_BINDINGS_MEMORY_TEGRA_SWGROUP_H */
