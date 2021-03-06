/* Copyright (c) 2008-2010, Code Aurora Forum. All rights reserved.
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

#ifndef MDDIHOSTI_H
#define MDDIHOSTI_H

#include "msm_fb.h"
#include "mddihost.h"
#include <linux/clk.h>

#define MDDI_CMD   		0x0000
#define MDDI_VERSION   		0x0004
#define MDDI_PRI_PTR		0x0008
#define MDDI_BPS		0x0010
#define MDDI_SPM		0x0014
#define MDDI_INT		0x0018
#define MDDI_INTEN		0x001c
#define MDDI_REV_PTR		0x0020
#define MDDI_REV_SIZE		0x0024
#define MDDI_STAT		0x0028
#define MDDI_REV_RATE_DIV	0x002c
#define MDDI_REV_CRC_ERR	0x0030
#define MDDI_TA1_LEN		0x0034
#define MDDI_TA2_LEN		0x0038
#define MDDI_TEST		0x0040
#define MDDI_REV_PKT_CNT	0x0044
#define MDDI_DRIVE_HI		0x0048
#define MDDI_DRIVE_LO		0x004c
#define MDDI_DISP_WAKE		0x0050
#define MDDI_REV_ENCAP_SZ	0x0054
#define MDDI_RTD_VAL		0x0058
#define MDDI_PAD_CTL		0x0068
#define MDDI_DRIVER_START_CNT	0x006c
#define MDDI_CORE_VER		0x008c
#define MDDI_FIFO_ALLOC         0x0090
#define MDDI_PAD_IO_CTL         0x00a0
#define MDDI_PAD_CAL            0x00a4

#ifdef ENABLE_MDDI_MULTI_READ_WRITE
#define MDDI_HOST_MAX_CLIENT_REG_IN_SAME_ADDR 128
#else
#define MDDI_HOST_MAX_CLIENT_REG_IN_SAME_ADDR 1
#endif

extern int32 mddi_client_type;
extern u32 mddi_msg_level;

#define xxxx_mddi_host_reg_outm(reg, mask, val)  \
do { \
	if (host_idx == MDDI_HOST_PRIM) \
		mddi_host_reg_outm_pmdh(reg, mask, val); \
	else \
		mddi_host_reg_outm_emdh(reg, mask, val); \
} while (0)

#define mddi_host_reg_outm(reg, mask, val) \
do { \
	unsigned long __addr; \
	if (host_idx == MDDI_HOST_PRIM) \
		__addr = (u32)msm_pmdh_base + MDDI_##reg; \
	else \
		__addr = (u32)msm_emdh_base + MDDI_##reg; \
	writel((readl(__addr) & ~(mask)) | ((val) & (mask)), __addr); \
} while (0)

#define xxxx_mddi_host_reg_out(reg, val) \
do { \
	if (host_idx == MDDI_HOST_PRIM)  \
		mddi_host_reg_out_pmdh(reg, val); \
	else \
		mddi_host_reg_out_emdh(reg, val); \
	} while (0)

#define mddi_host_reg_out(reg, val) \
do { \
	if (host_idx == MDDI_HOST_PRIM) \
		writel(val, (u32)msm_pmdh_base + MDDI_##reg); \
	else \
		writel(val, (u32)msm_emdh_base + MDDI_##reg); \
} while (0)

#define xxxx_mddi_host_reg_in(reg)  \
  ((host_idx) ? \
     mddi_host_reg_in_emdh(reg) : mddi_host_reg_in_pmdh(reg));

#define mddi_host_reg_in(reg) \
((host_idx) ? \
	readl((u32)msm_emdh_base + MDDI_##reg) : \
	readl((u32)msm_pmdh_base + MDDI_##reg)) \

#define xxxx_mddi_host_reg_inm(reg, mask)  \
  ((host_idx) ? \
    mddi_host_reg_inm_emdh(reg, mask) : \
    mddi_host_reg_inm_pmdh(reg, mask);)

#define mddi_host_reg_inm(reg, mask) \
((host_idx) ? \
	readl((u32)msm_emdh_base + MDDI_##reg) & (mask) : \
	readl((u32)msm_pmdh_base + MDDI_##reg) & (mask)) \

#define mddi_invalidate_cache_lines(addr_start, num_bytes)
#define mddi_flush_cache_lines(addr_start, num_bytes)  \
    (void) addr_start; \
    (void) num_bytes;  \
    memory_barrier()

#define MDDI_HOST_IS_IO_CLOCK_ON mddi_host_io_clock_on
#define MDDI_HOST_ENABLE_IO_CLOCK
#define MDDI_HOST_DISABLE_IO_CLOCK
#define MDDI_HOST_IS_HCLK_ON mddi_host_hclk_on
#define MDDI_HOST_ENABLE_HCLK
#define MDDI_HOST_DISABLE_HCLK
#define FEATURE_MDDI_HOST_IO_CLOCK_CONTROL_DISABLE
#define FEATURE_MDDI_HOST_HCLK_CONTROL_DISABLE

#define TRAMP_MDDI_HOST_ISR TRAMP_MDDI_PRI_ISR
#define TRAMP_MDDI_HOST_EXT_ISR TRAMP_MDDI_EXT_ISR
#define MDP_LINE_COUNT_BMSK  0x3ff
#define MDP_SYNC_STATUS  0x000c
#define MDP_LINE_COUNT      \
(readl(msm_mdp_base + MDP_SYNC_STATUS) & MDP_LINE_COUNT_BMSK)

#define MDDI_HOST_BYTES_PER_SUBFRAME  0x3C00

#if defined(CONFIG_FB_MSM_MDP31) || defined(CONFIG_FB_MSM_MDP40)
#define MDDI_HOST_TA2_LEN       0x001a
#define MDDI_HOST_REV_RATE_DIV  0x0004
#else
#define MDDI_HOST_TA2_LEN       0x000c
#define MDDI_HOST_REV_RATE_DIV  0x0002
#endif

#define MDDI_MSG_EMERG(msg, ...)    \
	if (mddi_msg_level > 0)  \
		printk(KERN_EMERG msg, ## __VA_ARGS__);
#define MDDI_MSG_ALERT(msg, ...)    \
	if (mddi_msg_level > 1)  \
		printk(KERN_ALERT msg, ## __VA_ARGS__);
#define MDDI_MSG_CRIT(msg, ...)    \
	if (mddi_msg_level > 2)  \
		printk(KERN_CRIT msg, ## __VA_ARGS__);
#define MDDI_MSG_ERR(msg, ...)    \
	if (mddi_msg_level > 3)  \
		printk(KERN_ERR msg, ## __VA_ARGS__);
#define MDDI_MSG_WARNING(msg, ...)    \
	if (mddi_msg_level > 4)  \
		printk(KERN_WARNING msg, ## __VA_ARGS__);
#define MDDI_MSG_NOTICE(msg, ...)    \
	if (mddi_msg_level > 5)  \
		printk(KERN_NOTICE msg, ## __VA_ARGS__);
#define MDDI_MSG_INFO(msg, ...)    \
	if (mddi_msg_level > 6)  \
		printk(KERN_INFO msg, ## __VA_ARGS__);
#define MDDI_MSG_DEBUG(msg, ...)    \
	if (mddi_msg_level > 7)  \
		printk(KERN_DEBUG msg, ## __VA_ARGS__);

#define GCC_PACKED __attribute__((packed))
typedef struct GCC_PACKED {
	uint16 packet_length;

	uint16 packet_type;

	uint16 bClient_ID;

} mddi_rev_packet_type;

typedef struct GCC_PACKED {
	uint16 packet_length;

	uint16 packet_type;

	uint16 bClient_ID;

	uint16 reverse_link_request;

	uint8 crc_error_count;
	uint8 capability_change;
	uint16 graphics_busy_flags;

	uint16 parameter_CRC;

} mddi_client_status_type;

typedef struct GCC_PACKED {
	uint16 packet_length;

	uint16 packet_type;

	uint16 bClient_ID;

	uint16 Protocol_Version;
	uint16 Minimum_Protocol_Version;
	uint16 Data_Rate_Capability;
	uint8 Interface_Type_Capability;
	uint8 Number_of_Alt_Displays;
	uint16 PostCal_Data_Rate;
	uint16 Bitmap_Width;
	uint16 Bitmap_Height;
	uint16 Display_Window_Width;
	uint16 Display_Window_Height;
	uint32 Color_Map_Size;
	uint16 Color_Map_RGB_Width;
	uint16 RGB_Capability;
	uint8 Monochrome_Capability;
	uint8 Reserved_1;
	uint16 Y_Cb_Cr_Capability;
	uint16 Bayer_Capability;
	uint16 Alpha_Cursor_Image_Planes;
	uint32 Client_Feature_Capability_Indicators;
	uint8 Maximum_Video_Frame_Rate_Capability;
	uint8 Minimum_Video_Frame_Rate_Capability;
	uint16 Minimum_Sub_frame_Rate;
	uint16 Audio_Buffer_Depth;
	uint16 Audio_Channel_Capability;
	uint16 Audio_Sample_Rate_Capability;
	uint8 Audio_Sample_Resolution;
	uint8 Mic_Audio_Sample_Resolution;
	uint16 Mic_Sample_Rate_Capability;
	uint8 Keyboard_Data_Format;
	uint8 pointing_device_data_format;
	uint16 content_protection_type;
	uint16 Mfr_Name;
	uint16 Product_Code;
	uint16 Reserved_3;
	uint32 Serial_Number;
	uint8 Week_of_Manufacture;
	uint8 Year_of_Manufacture;

	uint16 parameter_CRC;
	

} mddi_client_capability_type;

typedef struct GCC_PACKED {
	uint16 packet_length;
	

	uint16 packet_type;
	

	uint16 bClient_ID;
	

	uint16 video_data_format_descriptor;

	uint16 pixel_data_attributes;

	uint16 x_left_edge;
	uint16 y_top_edge;
	

	uint16 x_right_edge;
	uint16 y_bottom_edge;
	

	uint16 x_start;
	uint16 y_start;
	

	uint16 pixel_count;
	

	uint16 parameter_CRC;
	

	uint16 reserved;
	

} mddi_video_stream_packet_type;

typedef struct GCC_PACKED {
	uint16 packet_length;
	

	uint16 packet_type;
	

	uint16 bClient_ID;
	

	uint16 read_write_info;

	uint32 register_address;
	

	uint16 parameter_CRC;
	

	uint32 register_data_list[MDDI_HOST_MAX_CLIENT_REG_IN_SAME_ADDR];
	
	

} mddi_register_access_packet_type;

typedef union GCC_PACKED {
	mddi_video_stream_packet_type video_pkt;
	mddi_register_access_packet_type register_pkt;
#ifdef ENABLE_MDDI_MULTI_READ_WRITE
	uint32 alignment_pad[252];	
#else
	uint32 alignment_pad[12];	
#endif
} mddi_packet_header_type;

typedef struct GCC_PACKED mddi_host_llist_struct {
	uint16 link_controller_flags;
	uint16 packet_header_count;
	uint16 packet_data_count;
	void *packet_data_pointer;
	struct mddi_host_llist_struct *next_packet_pointer;
	uint16 reserved;
	mddi_packet_header_type packet_header;
} mddi_linked_list_type;

typedef struct {
	struct completion done_comp;
	mddi_llist_done_cb_type done_cb;
	uint16 next_idx;
	boolean waiting;
	boolean in_use;
} mddi_linked_list_notify_type;

#ifdef ENABLE_MDDI_MULTI_READ_WRITE
#define MDDI_LLIST_POOL_SIZE 0x10000
#else
#define MDDI_LLIST_POOL_SIZE 0x1000
#endif
#define MDDI_MAX_NUM_LLIST_ITEMS (MDDI_LLIST_POOL_SIZE / \
		 sizeof(mddi_linked_list_type))
#define UNASSIGNED_INDEX MDDI_MAX_NUM_LLIST_ITEMS
#define MDDI_FIRST_DYNAMIC_LLIST_IDX 0

#define MDDI_NUM_STATIC_ABL_ITEMS 1
#define MDDI_NUM_STATIC_WINDOW_ITEMS 5
#define MDDI_NUM_STATIC_LLIST_ITEMS (MDDI_NUM_STATIC_ABL_ITEMS + \
				MDDI_NUM_STATIC_WINDOW_ITEMS)
#define MDDI_NUM_DYNAMIC_LLIST_ITEMS (MDDI_MAX_NUM_LLIST_ITEMS - \
				MDDI_NUM_STATIC_LLIST_ITEMS)

#define MDDI_FIRST_STATIC_LLIST_IDX  MDDI_NUM_DYNAMIC_LLIST_ITEMS
#define MDDI_FIRST_STATIC_ABL_IDX  MDDI_FIRST_STATIC_LLIST_IDX
#define MDDI_FIRST_STATIC_WINDOW_IDX  (MDDI_FIRST_STATIC_LLIST_IDX + \
				MDDI_NUM_STATIC_ABL_ITEMS)

#define VSYNC_WAKEUP_REG          0x80
#define GPIO_REG                  0x81
#define GPIO_OUTPUT_REG           0x82
#define GPIO_INTERRUPT_REG        0x83
#define GPIO_INTERRUPT_ENABLE_REG 0x84
#define GPIO_POLARITY_REG         0x85

#define MDDI_INT_PRI_PTR_READ       0x0001
#define MDDI_INT_SEC_PTR_READ       0x0002
#define MDDI_INT_REV_DATA_AVAIL     0x0004
#define MDDI_INT_DISP_REQ           0x0008
#define MDDI_INT_PRI_UNDERFLOW      0x0010
#define MDDI_INT_SEC_UNDERFLOW      0x0020
#define MDDI_INT_REV_OVERFLOW       0x0040
#define MDDI_INT_CRC_ERROR          0x0080
#define MDDI_INT_MDDI_IN            0x0100
#define MDDI_INT_PRI_OVERWRITE      0x0200
#define MDDI_INT_SEC_OVERWRITE      0x0400
#define MDDI_INT_REV_OVERWRITE      0x0800
#define MDDI_INT_DMA_FAILURE        0x1000
#define MDDI_INT_LINK_ACTIVE        0x2000
#define MDDI_INT_IN_HIBERNATION     0x4000
#define MDDI_INT_PRI_LINK_LIST_DONE 0x8000
#define MDDI_INT_SEC_LINK_LIST_DONE 0x10000
#define MDDI_INT_NO_CMD_PKTS_PEND   0x20000
#define MDDI_INT_RTD_FAILURE        0x40000

#define MDDI_INT_ERROR_CONDITIONS ( \
	MDDI_INT_PRI_UNDERFLOW | MDDI_INT_SEC_UNDERFLOW | \
	MDDI_INT_REV_OVERFLOW | MDDI_INT_CRC_ERROR | \
	MDDI_INT_PRI_OVERWRITE | MDDI_INT_SEC_OVERWRITE | \
	MDDI_INT_RTD_FAILURE | \
	MDDI_INT_REV_OVERWRITE | MDDI_INT_DMA_FAILURE)

#define MDDI_INT_LINK_STATE_CHANGES ( \
	MDDI_INT_LINK_ACTIVE | MDDI_INT_IN_HIBERNATION)

#define MDDI_STAT_LINK_ACTIVE        0x0001
#define MDDI_STAT_NEW_REV_PTR        0x0002
#define MDDI_STAT_NEW_PRI_PTR        0x0004
#define MDDI_STAT_NEW_SEC_PTR        0x0008
#define MDDI_STAT_IN_HIBERNATION     0x0010
#define MDDI_STAT_PRI_LINK_LIST_DONE 0x0020
#define MDDI_STAT_SEC_LINK_LIST_DONE 0x0040
#define MDDI_STAT_PENDING_TIMING_PKT 0x0080
#define MDDI_STAT_PENDING_REV_ENCAP  0x0100
#define MDDI_STAT_PENDING_POWERDOWN  0x0200
#define MDDI_STAT_RTD_MEAS_FAIL      0x0800
#define MDDI_STAT_CLIENT_WAKEUP_REQ  0x1000

#define MDDI_CMD_POWERDOWN           0x0100
#define MDDI_CMD_POWERUP             0x0200
#define MDDI_CMD_HIBERNATE           0x0300
#define MDDI_CMD_RESET               0x0400
#define MDDI_CMD_DISP_IGNORE         0x0501
#define MDDI_CMD_DISP_LISTEN         0x0500
#define MDDI_CMD_SEND_REV_ENCAP      0x0600
#define MDDI_CMD_GET_CLIENT_CAP      0x0601
#define MDDI_CMD_GET_CLIENT_STATUS   0x0602
#define MDDI_CMD_SEND_RTD            0x0700
#define MDDI_CMD_LINK_ACTIVE         0x0900
#define MDDI_CMD_PERIODIC_REV_ENCAP  0x0A00
#define MDDI_CMD_FW_LINK_SKEW_CAL    0x0D00

extern void mddi_host_init(mddi_host_type host);
extern void mddi_host_powerdown(mddi_host_type host);
extern uint16 mddi_get_next_free_llist_item(mddi_host_type host, boolean wait);
extern uint16 mddi_get_reg_read_llist_item(mddi_host_type host, boolean wait);
extern void mddi_queue_forward_packets(uint16 first_llist_idx,
				       uint16 last_llist_idx,
				       boolean wait,
				       mddi_llist_done_cb_type llist_done_cb,
				       mddi_host_type host);

extern void mddi_host_write_pix_attr_reg(uint32 value);
extern void mddi_client_lcd_gpio_poll(uint32 poll_reg_val);
extern void mddi_client_lcd_vsync_detected(boolean detected);
extern void mddi_host_disable_hibernation(boolean disable);

extern mddi_linked_list_type *llist_extern[];
extern mddi_linked_list_type *llist_dma_extern[];
extern mddi_linked_list_notify_type *llist_extern_notify[];
extern struct timer_list mddi_host_timer;

typedef struct {
	uint16 transmitting_start_idx;
	uint16 transmitting_end_idx;
	uint16 waiting_start_idx;
	uint16 waiting_end_idx;
	uint16 reg_read_idx;
	uint16 next_free_idx;
	boolean reg_read_waiting;
} mddi_llist_info_type;

extern mddi_llist_info_type mddi_llist;

#define MDDI_GPIO_DEFAULT_POLLING_INTERVAL 200
typedef struct {
	uint32 polling_reg;
	uint32 polling_val;
	uint32 polling_interval;
	boolean polling_enabled;
} mddi_gpio_info_type;

uint32 mddi_get_client_id(void);
void mddi_mhctl_remove(mddi_host_type host_idx);
void mddi_host_timer_service(unsigned long data);
void mddi_host_client_cnt_reset(void);
#endif 
