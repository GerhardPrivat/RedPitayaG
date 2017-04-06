/**
 * $Id: $
 *
 * @brief Red Pitaya library oscilloscope module interface
 *
 * @Author Red Pitaya
 *
 * (c) Red Pitaya  http://www.redpitaya.com
 *
 * This part of code is written in C programming language.
 * Please visit http://en.wikipedia.org/wiki/C_(programming_language)
 * for more details on the language used herein.
 */


#ifndef SRC_ACW_H_
#define SRC_ACW_H_

#include <stdint.h>
#include <stdbool.h>

// Base Oscilloscope address
static const int OSC_BASE_ADDR = 0x00100000;
static const int OSC_BASE_SIZE = 0x30000;

// Oscilloscope Channel A input signal buffer offset
#define OSC_CHA_OFFSET 0x10000

// Oscilloscope Channel B input signal buffer offset
#define OSC_CHB_OFFSET 0x20000

// @brief Channel A/B Equalization filter
typedef struct {
    uint32_t aa;  // bits [31:18] - reserved, bits [17:0] - AA coefficient (pole)
    uint32_t bb;  // bits [31:25] - reserved, bits [24:0] - BB coefficient (zero)
    uint32_t kk;  // bits [31:25] - reserved, bits [24:0] - KK coefficient (gain)
    uint32_t pp;  // bits [31:25] - reserved, bits [24:0] - PP coefficient (pole)
} osc_filter;

// Oscilloscope structure declaration
typedef struct osc_control_s {

    /** @brief Offset 0x00 - configuration register
     *
     * Configuration register (offset 0x00):
     * bit [0] - arm_trigger
     * bit [1] - rst_wr_state_machine
     * bit [2] - trigger_status
     * bit [3] - arm_keep
     * bits [31:4] - reserved
     */
    uint32_t conf;

    /** @brief Offset 0x04 - trigger source register
     *
     * Trigger source register (offset 0x04):
     * bits [ 2 : 0] - trigger source:
     * 1 - trig immediately
     * 2 - ChA positive edge
     * 3 - ChA negative edge
     * 4 - ChB positive edge
     * 5 - ChB negative edge
     * 6 - External trigger 0
     * 7 - External trigger 1
     * bits [31 : 3] -reserved
     */
    uint32_t trig_source;

    /** @brief Offset 0x08 - Channel A/B threshold register
     *
     * Channel A/B threshold register (offset 0x08):
     * bits [13: 0] - ChA threshold
     * bits [31:14] - sign extension
     */
    int32_t thr[2];

    /** @brief Offset 0x10 - After trigger delay register
     *
     * After trigger delay register (offset 0x10)
     * bits [31: 0] - trigger delay
     * 32 bit number - how many decimated samples should be stored into a buffer.
     * (max 16k samples)
     */
    uint32_t dly_pst;

    /** @brief Offset 0x14 - Data decimation register
     *
     * Data decimation register (offset 0x14):
     * bits [16: 0] - decimation factor, legal values:
     * 1, 8, 64, 1024, 8192 65536
     * If other values are written data is undefined
     * bits [31:17] - reserved
     */
    uint32_t data_dec;

    /** @brief Offset 0x18 - Current write pointer register
     *
     * Current write pointer register (offset 0x18), read only:
     * bits [13: 0] - current write pointer
     * bits [31:14] - reserved
     */
    uint32_t wr_ptr_cur;

    /** @brief Offset 0x1C - Trigger write pointer register
     *
     * Trigger write pointer register (offset 0x1C), read only:
     * bits [13: 0] - trigger pointer (pointer where trigger was detected)
     * bits [31:14] - reserved
     */
    uint32_t wr_ptr_trigger;

    /** @brief ChA & ChB hysteresis - both of the format:
     * bits [13: 0] - hysteresis threshold
     * bits [31:14] - sign extension
     */
    int32_t hystersis[2];

    /** @brief
     * bits [0] - enable signal average at decimation
     * bits [31:1] - reserved
     */
    uint32_t other;

    /** @brief - Pre Trigger counter
     *
     * Pre Trigger counter (offset 0x2C)
     * bits [31: 0] - Pre Trigger counter
     * 32 bit number - how many decimated samples have been stored into a buffer
     * before trigger arrived.
     */
    uint32_t pre_trigger_counter;

    // @brief Channel A/B Equalization filter
    osc_filter filter [2];

    /* Reserved 0x88 & 0x8C */
    uint32_t reserved_4[16];

    /**@brief Trigger debuncer time
    * bits [19:0] Number of ADC clock periods
    * trigger is disabled after activation
    * reset value is decimal 62500
    * or equivalent to 0.5ms
    */
    uint32_t trig_dbc_t;

    uint32_t dly_pre;

    /* ChA & ChB data - 14 LSB bits valid starts from 0x10000 and
     * 0x20000 and are each 16k samples long */
} osc_control_t;

static const uint32_t DATA_DEC_MASK         = 0x1FFFF;      // (17 bits)
static const uint32_t DATA_AVG_MASK         = 0x1;          // (1 bit)
static const uint32_t TRIG_SRC_MASK         = 0xF;          // (4 bits)
static const uint32_t START_DATA_WRITE_MASK = 0x1;          // (1 bit)
static const uint32_t TRIG_DELAY_MASK       = 0xFFFFFFFF;   // (32 bits)
static const uint32_t WRITE_POINTER_MASK    = 0x3FFF;       // (14 bits)
static const uint32_t RST_WR_ST_MCH_MASK    = 0x2;          // (1st bit)
static const uint32_t TRIG_ST_MCH_MASK      = 0x4;          // (2st bit)
static const uint32_t PRE_TRIGGER_COUNTER   = 0xFFFFFFFF;   // (32 bit)
static const uint32_t ARM_KEEP_MASK         = 0xF;          // (4 bit)

#endif /* SRC_ACQ_H_ */
