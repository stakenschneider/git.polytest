/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_gen2_0' in SOPC Builder design 'max10_qsys'
 * SOPC Builder design path: ../../max10_qsys.sopcinfo
 *
 * Generated: Mon Dec 19 16:20:59 MSK 2016
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x11000820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1d
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x08000020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1d
#define ALT_CPU_NAME "nios2_gen2_0"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x08000000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x11000820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1d
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x08000020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1d
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x08000000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SGDMA
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_MEM_IF_DDR3_EMIF
#define __ALTERA_NIOS2_GEN2
#define __ALTPLL


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "MAX 10"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x78
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x78
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x78
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "max10_qsys"


/*
 * ddr3_status configuration
 *
 */

#define ALT_MODULE_CLASS_ddr3_status altera_avalon_pio
#define DDR3_STATUS_BASE 0x10000050
#define DDR3_STATUS_BIT_CLEARING_EDGE_REGISTER 0
#define DDR3_STATUS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DDR3_STATUS_CAPTURE 0
#define DDR3_STATUS_DATA_WIDTH 3
#define DDR3_STATUS_DO_TEST_BENCH_WIRING 0
#define DDR3_STATUS_DRIVEN_SIM_VALUE 0
#define DDR3_STATUS_EDGE_TYPE "NONE"
#define DDR3_STATUS_FREQ 40000000
#define DDR3_STATUS_HAS_IN 1
#define DDR3_STATUS_HAS_OUT 0
#define DDR3_STATUS_HAS_TRI 0
#define DDR3_STATUS_IRQ -1
#define DDR3_STATUS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DDR3_STATUS_IRQ_TYPE "NONE"
#define DDR3_STATUS_NAME "/dev/ddr3_status"
#define DDR3_STATUS_RESET_VALUE 0
#define DDR3_STATUS_SPAN 32
#define DDR3_STATUS_TYPE "altera_avalon_pio"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x78
#define JTAG_UART_IRQ 2
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 16
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * key configuration
 *
 */

#define ALT_MODULE_CLASS_key altera_avalon_pio
#define KEY_BASE 0x10000060
#define KEY_BIT_CLEARING_EDGE_REGISTER 0
#define KEY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEY_CAPTURE 1
#define KEY_DATA_WIDTH 5
#define KEY_DO_TEST_BENCH_WIRING 0
#define KEY_DRIVEN_SIM_VALUE 0
#define KEY_EDGE_TYPE "FALLING"
#define KEY_FREQ 40000000
#define KEY_HAS_IN 1
#define KEY_HAS_OUT 0
#define KEY_HAS_TRI 0
#define KEY_IRQ 3
#define KEY_IRQ_INTERRUPT_CONTROLLER_ID 0
#define KEY_IRQ_TYPE "EDGE"
#define KEY_NAME "/dev/key"
#define KEY_RESET_VALUE 0
#define KEY_SPAN 32
#define KEY_TYPE "altera_avalon_pio"


/*
 * lcd_sgdma configuration
 *
 */

#define ALT_MODULE_CLASS_lcd_sgdma altera_avalon_sgdma
#define LCD_SGDMA_ADDRESS_WIDTH 32
#define LCD_SGDMA_ALWAYS_DO_MAX_BURST 1
#define LCD_SGDMA_ATLANTIC_CHANNEL_DATA_WIDTH 4
#define LCD_SGDMA_AVALON_MM_BYTE_REORDER_MODE 0
#define LCD_SGDMA_BASE 0x11001000
#define LCD_SGDMA_BURST_DATA_WIDTH 8
#define LCD_SGDMA_BURST_TRANSFER 1
#define LCD_SGDMA_BYTES_TO_TRANSFER_DATA_WIDTH 16
#define LCD_SGDMA_CHAIN_WRITEBACK_DATA_WIDTH 32
#define LCD_SGDMA_COMMAND_FIFO_DATA_WIDTH 104
#define LCD_SGDMA_CONTROL_DATA_WIDTH 8
#define LCD_SGDMA_CONTROL_SLAVE_ADDRESS_WIDTH 0x4
#define LCD_SGDMA_CONTROL_SLAVE_DATA_WIDTH 32
#define LCD_SGDMA_DESCRIPTOR_READ_BURST 0
#define LCD_SGDMA_DESC_DATA_WIDTH 32
#define LCD_SGDMA_HAS_READ_BLOCK 1
#define LCD_SGDMA_HAS_WRITE_BLOCK 0
#define LCD_SGDMA_IN_ERROR_WIDTH 0
#define LCD_SGDMA_IRQ 0
#define LCD_SGDMA_IRQ_INTERRUPT_CONTROLLER_ID 0
#define LCD_SGDMA_NAME "/dev/lcd_sgdma"
#define LCD_SGDMA_OUT_ERROR_WIDTH 0
#define LCD_SGDMA_READ_BLOCK_DATA_WIDTH 64
#define LCD_SGDMA_READ_BURSTCOUNT_WIDTH 8
#define LCD_SGDMA_SPAN 64
#define LCD_SGDMA_STATUS_TOKEN_DATA_WIDTH 24
#define LCD_SGDMA_STREAM_DATA_WIDTH 64
#define LCD_SGDMA_SYMBOLS_PER_BEAT 8
#define LCD_SGDMA_TYPE "altera_avalon_sgdma"
#define LCD_SGDMA_UNALIGNED_TRANSFER 0
#define LCD_SGDMA_WRITE_BLOCK_DATA_WIDTH 64
#define LCD_SGDMA_WRITE_BURSTCOUNT_WIDTH 4


/*
 * mem_if_ddr3_emif configuration
 *
 */

#define ALT_MODULE_CLASS_mem_if_ddr3_emif altera_mem_if_ddr3_emif
#define MEM_IF_DDR3_EMIF_BASE 0x8000000
#define MEM_IF_DDR3_EMIF_IRQ -1
#define MEM_IF_DDR3_EMIF_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MEM_IF_DDR3_EMIF_NAME "/dev/mem_if_ddr3_emif"
#define MEM_IF_DDR3_EMIF_SPAN 134217728
#define MEM_IF_DDR3_EMIF_TYPE "altera_mem_if_ddr3_emif"


/*
 * mem_if_ddr3_emif configuration as viewed by lcd_sgdma_m_read
 *
 */

#define LCD_SGDMA_M_READ_MEM_IF_DDR3_EMIF_BASE 0x8000000
#define LCD_SGDMA_M_READ_MEM_IF_DDR3_EMIF_IRQ -1
#define LCD_SGDMA_M_READ_MEM_IF_DDR3_EMIF_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LCD_SGDMA_M_READ_MEM_IF_DDR3_EMIF_NAME "/dev/mem_if_ddr3_emif"
#define LCD_SGDMA_M_READ_MEM_IF_DDR3_EMIF_SPAN 134217728
#define LCD_SGDMA_M_READ_MEM_IF_DDR3_EMIF_TYPE "altera_mem_if_ddr3_emif"


/*
 * pll configuration
 *
 */

#define ALT_MODULE_CLASS_pll altpll
#define PLL_BASE 0x11001040
#define PLL_IRQ -1
#define PLL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PLL_NAME "/dev/pll"
#define PLL_SPAN 16
#define PLL_TYPE "altpll"


/*
 * rh_temp_drdy_n configuration
 *
 */

#define ALT_MODULE_CLASS_rh_temp_drdy_n altera_avalon_pio
#define RH_TEMP_DRDY_N_BASE 0x10000020
#define RH_TEMP_DRDY_N_BIT_CLEARING_EDGE_REGISTER 0
#define RH_TEMP_DRDY_N_BIT_MODIFYING_OUTPUT_REGISTER 0
#define RH_TEMP_DRDY_N_CAPTURE 0
#define RH_TEMP_DRDY_N_DATA_WIDTH 1
#define RH_TEMP_DRDY_N_DO_TEST_BENCH_WIRING 0
#define RH_TEMP_DRDY_N_DRIVEN_SIM_VALUE 0
#define RH_TEMP_DRDY_N_EDGE_TYPE "NONE"
#define RH_TEMP_DRDY_N_FREQ 40000000
#define RH_TEMP_DRDY_N_HAS_IN 1
#define RH_TEMP_DRDY_N_HAS_OUT 0
#define RH_TEMP_DRDY_N_HAS_TRI 0
#define RH_TEMP_DRDY_N_IRQ -1
#define RH_TEMP_DRDY_N_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RH_TEMP_DRDY_N_IRQ_TYPE "NONE"
#define RH_TEMP_DRDY_N_NAME "/dev/rh_temp_drdy_n"
#define RH_TEMP_DRDY_N_RESET_VALUE 0
#define RH_TEMP_DRDY_N_SPAN 32
#define RH_TEMP_DRDY_N_TYPE "altera_avalon_pio"


/*
 * rh_temp_i2c_scl configuration
 *
 */

#define ALT_MODULE_CLASS_rh_temp_i2c_scl altera_avalon_pio
#define RH_TEMP_I2C_SCL_BASE 0x10000040
#define RH_TEMP_I2C_SCL_BIT_CLEARING_EDGE_REGISTER 0
#define RH_TEMP_I2C_SCL_BIT_MODIFYING_OUTPUT_REGISTER 0
#define RH_TEMP_I2C_SCL_CAPTURE 0
#define RH_TEMP_I2C_SCL_DATA_WIDTH 1
#define RH_TEMP_I2C_SCL_DO_TEST_BENCH_WIRING 0
#define RH_TEMP_I2C_SCL_DRIVEN_SIM_VALUE 0
#define RH_TEMP_I2C_SCL_EDGE_TYPE "NONE"
#define RH_TEMP_I2C_SCL_FREQ 40000000
#define RH_TEMP_I2C_SCL_HAS_IN 0
#define RH_TEMP_I2C_SCL_HAS_OUT 1
#define RH_TEMP_I2C_SCL_HAS_TRI 0
#define RH_TEMP_I2C_SCL_IRQ -1
#define RH_TEMP_I2C_SCL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RH_TEMP_I2C_SCL_IRQ_TYPE "NONE"
#define RH_TEMP_I2C_SCL_NAME "/dev/rh_temp_i2c_scl"
#define RH_TEMP_I2C_SCL_RESET_VALUE 0
#define RH_TEMP_I2C_SCL_SPAN 32
#define RH_TEMP_I2C_SCL_TYPE "altera_avalon_pio"


/*
 * rh_temp_i2c_sda configuration
 *
 */

#define ALT_MODULE_CLASS_rh_temp_i2c_sda altera_avalon_pio
#define RH_TEMP_I2C_SDA_BASE 0x10000030
#define RH_TEMP_I2C_SDA_BIT_CLEARING_EDGE_REGISTER 0
#define RH_TEMP_I2C_SDA_BIT_MODIFYING_OUTPUT_REGISTER 0
#define RH_TEMP_I2C_SDA_CAPTURE 0
#define RH_TEMP_I2C_SDA_DATA_WIDTH 1
#define RH_TEMP_I2C_SDA_DO_TEST_BENCH_WIRING 0
#define RH_TEMP_I2C_SDA_DRIVEN_SIM_VALUE 0
#define RH_TEMP_I2C_SDA_EDGE_TYPE "NONE"
#define RH_TEMP_I2C_SDA_FREQ 40000000
#define RH_TEMP_I2C_SDA_HAS_IN 0
#define RH_TEMP_I2C_SDA_HAS_OUT 0
#define RH_TEMP_I2C_SDA_HAS_TRI 1
#define RH_TEMP_I2C_SDA_IRQ -1
#define RH_TEMP_I2C_SDA_IRQ_INTERRUPT_CONTROLLER_ID -1
#define RH_TEMP_I2C_SDA_IRQ_TYPE "NONE"
#define RH_TEMP_I2C_SDA_NAME "/dev/rh_temp_i2c_sda"
#define RH_TEMP_I2C_SDA_RESET_VALUE 0
#define RH_TEMP_I2C_SDA_SPAN 32
#define RH_TEMP_I2C_SDA_TYPE "altera_avalon_pio"


/*
 * sysid_qsys configuration
 *
 */

#define ALT_MODULE_CLASS_sysid_qsys altera_avalon_sysid_qsys
#define SYSID_QSYS_BASE 0x10000070
#define SYSID_QSYS_ID 0
#define SYSID_QSYS_IRQ -1
#define SYSID_QSYS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSID_QSYS_NAME "/dev/sysid_qsys"
#define SYSID_QSYS_SPAN 8
#define SYSID_QSYS_TIMESTAMP 1435901586
#define SYSID_QSYS_TYPE "altera_avalon_sysid_qsys"


/*
 * timer configuration
 *
 */

#define ALT_MODULE_CLASS_timer altera_avalon_timer
#define TIMER_ALWAYS_RUN 0
#define TIMER_BASE 0x10000000
#define TIMER_COUNTER_SIZE 32
#define TIMER_FIXED_PERIOD 0
#define TIMER_FREQ 40000000
#define TIMER_IRQ 1
#define TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_LOAD_VALUE 39999
#define TIMER_MULT 0.001
#define TIMER_NAME "/dev/timer"
#define TIMER_PERIOD 1
#define TIMER_PERIOD_UNITS "ms"
#define TIMER_RESET_OUTPUT 0
#define TIMER_SNAPSHOT 1
#define TIMER_SPAN 64
#define TIMER_TICKS_PER_SEC 1000
#define TIMER_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
