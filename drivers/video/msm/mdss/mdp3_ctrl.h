/* Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef MDP3_CTRL_H
#define MDP3_CTRL_H

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/completion.h>
#include <linux/timer.h>

#include "mdp3.h"
#include "mdp3_dma.h"
#include "mdss_fb.h"
#include "mdss_panel.h"

#define MDP3_MAX_BUF_QUEUE 8

struct mdp3_buffer_queue {
	struct mdp3_img_data img_data[MDP3_MAX_BUF_QUEUE];
	int count;
	int push_idx;
	int pop_idx;
};

#ifdef CONFIG_FB_MSM_MDSS_MDP3_KCAL_CTRL
#define R_MASK  0xff0000
#define G_MASK  0xff00
#define B_MASK  0xff
#define R_SHIFT 16
#define G_SHIFT 8
#define B_SHIFT 0

#define NUM_QLUT 256
#define MAX_KCAL_V (NUM_QLUT - 1)

#define lut2rgb(lut,mask,shift) ((lut & mask) >> shift)
#define scaled_by_kcal(rgb, kcal) \
    (((((unsigned int)(rgb) * (unsigned int)(kcal)) << 16) / \
        (unsigned int)MAX_KCAL_V) >> 16)

struct kcal_lut_data {
	int red;
	int green;
	int blue;
	int minimum;
};
#endif

struct mdp3_session_data {
	struct mutex lock;
	struct mutex offlock;
	int status;
	struct mdp3_dma *dma;
	struct mdss_panel_data *panel;
	struct mdp3_intf *intf;
	struct msm_fb_data_type *mfd;
	ktime_t vsync_time;
	struct timer_list vsync_timer;
	int vsync_period;
	struct sysfs_dirent *vsync_event_sd;
	struct mdp_overlay overlay;
	struct mdp_overlay req_overlay;
	struct mdp3_buffer_queue bufq_in;
	struct mdp3_buffer_queue bufq_out;
	struct work_struct clk_off_work;
	struct work_struct dma_done_work;
	int histo_status;
	struct mutex histo_lock;
	int lut_sel;
	int cc_vect_sel;
	bool vsync_before_commit;
	bool first_commit;
	int clk_on;
	struct blocking_notifier_head notifier_head;

	int vsync_enabled;
	atomic_t vsync_countdown; /* Used to count down  */

#ifdef CONFIG_FB_MSM_MDSS_MDP3_KCAL_CTRL
	struct kcal_lut_data lut_data;
#endif
};

int mdp3_ctrl_init(struct msm_fb_data_type *mfd);

#endif /* MDP3_CTRL_H */
