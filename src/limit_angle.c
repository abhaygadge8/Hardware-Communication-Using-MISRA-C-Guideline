#include "limit_angle.h"
#include "config.h"
#include <stdio.h>

/*----------------------------------------------------------
 * Local helper function
 *----------------------------------------------------------*/
static ModbusStatus_t ReadLimitRegister(uint16_t reg_addr, uint8_t *state)
{
    uint32_t raw_val = 0U;

    if (state == NULL)
    {
        return MODBUS_ERROR;
    }

    if (MODBUS_UDP_Read(LCU_IP_ADDR, LCU_PORT_UDP, reg_addr, 1U, &raw_val) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }

    *state = (uint8_t)(raw_val & 0x01U);
    return MODBUS_OK;
}

/*----------------------------------------------------------
 * Initialize Modbus UDP connection
 *----------------------------------------------------------*/
ModbusStatus_t LimitAngle_Init(void)
{
    return MODBUS_UDP_InitConnection();
}

/*----------------------------------------------------------
 * Read Pan Axis Limit Angles
 *----------------------------------------------------------*/
ModbusStatus_t LimitAngle_ReadPan(LimitAngle_t *limit)
{
    if (limit == NULL)
    {
        return MODBUS_ERROR;
    }

    if (ReadLimitRegister(REG_PAN_LIMIT_UPPER, &limit->upper_state) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }
    if (ReadLimitRegister(REG_PAN_LIMIT_LOWER, &limit->lower_state) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }

    /* Assign corresponding angle limits */
    limit->upper_angle_deg = PAN_LIMIT_UPPER_DEG;
    limit->lower_angle_deg = PAN_LIMIT_LOWER_DEG;

    return MODBUS_OK;
}

/*----------------------------------------------------------
 * Read Tilt Axis Limit Angles
 *----------------------------------------------------------*/
ModbusStatus_t LimitAngle_ReadTilt(LimitAngle_t *limit)
{
    if (limit == NULL)
    {
        return MODBUS_ERROR;
    }

    if (ReadLimitRegister(REG_TILT_LIMIT_UPPER, &limit->upper_state) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }
    if (ReadLimitRegister(REG_TILT_LIMIT_LOWER, &limit->lower_state) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }

    /* Assign corresponding angle limits */
    limit->upper_angle_deg = TILT_LIMIT_UPPER_DEG;
    limit->lower_angle_deg = TILT_LIMIT_LOWER_DEG;

    return MODBUS_OK;
}
