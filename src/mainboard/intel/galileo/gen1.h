/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Intel Corp.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/* Jumper J2 determines the slave address of Cypress I/O GPIO expander */
#define GALILEO_DETERMINE_IOEXP_SLA_RESUMEWELL_GPIO     5

static const struct reg_script gen1_gpio_init[] = {
	/* Initialize the legacy GPIO controller */
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGEN_CORE_WELL, 0x03),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGIO_CORE_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGLVL_CORE_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGTPE_CORE_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGTNE_CORE_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGGPE_CORE_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGSMI_CORE_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CGTS_CORE_WELL, 0x03),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_CNMIEN_CORE_WELL, 0x00),

	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGEN_RESUME_WELL, 0x3f),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGIO_RESUME_WELL, 0x21),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGLVL_RESUME_WELL, 0x14),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGTPE_RESUME_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGTNE_RESUME_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGGPE_RESUME_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGSMI_RESUME_WELL, 0x00),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RGTS_RESUME_WELL, 0x3f),
	REG_LEG_GPIO_WRITE(R_QNC_GPIO_RNMIEN_RESUME_WELL, 0x00),

	/* Initialize the GPIO controller */
	REG_GPIO_WRITE(GPIO_INTEN, 0),
	REG_GPIO_WRITE(GPIO_INTSTATUS, 0),
	REG_GPIO_WRITE(GPIO_SWPORTA_DR, 5),
	REG_GPIO_WRITE(GPIO_SWPORTA_DDR, 0x15),
	REG_GPIO_WRITE(GPIO_INTMASK, 0),
	REG_GPIO_WRITE(GPIO_INTTYPE_LEVEL, 0),
	REG_GPIO_WRITE(GPIO_INT_POLARITY, 0),
	REG_GPIO_WRITE(GPIO_DEBOUNCE, 0),
	REG_GPIO_WRITE(GPIO_LS_SYNC, 0),

	/* Toggle the Cypress reset line */
	REG_GPIO_OR(GPIO_SWPORTA_DR, BIT4),
	REG_GPIO_AND(GPIO_SWPORTA_DR, ~BIT4),

	REG_SCRIPT_END
};

static const struct reg_script gen1_i2c_0x20_init[] = {
	/* Route I2C pins to Arduino header:
	 * Clear I2C_MUX (GPORT1_BIT5) to route I2C to Arduino Shield connector
	 *
	 *	I2C_SDA -> ANALOG_A4
	 *	I2C_SCL -> ANALOG_A5
	 */
	REG_I2C_WRITE(GEN1_I2C_GPIO_EXP_0x20, GEN1_GPIO_EXP_PORT_SELECT, 1),
	REG_I2C_AND(GEN1_I2C_GPIO_EXP_0x20, GEN1_GPIO_EXP_PORT_DIR, ~BIT5),
	REG_I2C_WRITE(GEN1_I2C_GPIO_EXP_0x20, GEN1_GPIO_EXP_OUTPUT1, ~BIT5),

	/* Set all GPIO expander pins connected to the Reset Button as inputs
	 * Configure RESET_N_SHLD (GPORT5_BIT0) and SW_RESET_N_SHLD
	 * (GPORT5_BIT1) as inputs
	 */
	REG_I2C_WRITE(GEN1_I2C_GPIO_EXP_0x20, GEN1_GPIO_EXP_PORT_SELECT, 5),
	REG_I2C_OR(GEN1_I2C_GPIO_EXP_0x20, GEN1_GPIO_EXP_PORT_DIR, BIT1 | BIT0),

	REG_SCRIPT_END
};

static const struct reg_script gen1_i2c_0x21_init[] = {
	/* Route I2C pins to Arduino header:
	 * Clear I2C_MUX (GPORT1_BIT5) to route I2C to Arduino Shield connector
	 *
	 *	I2C_SDA -> ANALOG_A4
	 *	I2C_SCL -> ANALOG_A5
	 */
	REG_I2C_WRITE(GEN1_I2C_GPIO_EXP_0x21, GEN1_GPIO_EXP_PORT_SELECT, 1),
	REG_I2C_AND(GEN1_I2C_GPIO_EXP_0x21, GEN1_GPIO_EXP_PORT_DIR, ~BIT5),
	REG_I2C_AND(GEN1_I2C_GPIO_EXP_0x21, GEN1_GPIO_EXP_OUTPUT1, ~BIT5),

	/* Set all GPIO expander pins connected to the Reset Button as inputs
	 * Configure RESET_N_SHLD (GPORT5_BIT0) and SW_RESET_N_SHLD
	 * (GPORT5_BIT1) as inputs
	 */
	REG_I2C_WRITE(GEN1_I2C_GPIO_EXP_0x21, GEN1_GPIO_EXP_PORT_SELECT, 5),
	REG_I2C_OR(GEN1_I2C_GPIO_EXP_0x21, GEN1_GPIO_EXP_PORT_DIR, BIT1 | BIT0),

	REG_SCRIPT_END
};
