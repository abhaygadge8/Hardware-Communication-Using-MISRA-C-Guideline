#include "config.h"
#include "drive_fault.h"
#include "modbus_functions.h"
#include <stdio.h>
#include <stdint.h>

/*----------------------------------------------------------
 * Internal helper
 *----------------------------------------------------------*/
static uint16_t GetRegisterAddress(Axis_t axis, uint16_t pan_addr, uint16_t tilt_addr)
{
    if (axis == AXIS_PAN)
    {
        return pan_addr;
    }
    else
    {
        return tilt_addr;
    }
}

/*----------------------------------------------------------
 * Read and decode fault status bits
 *----------------------------------------------------------*/
void Read_FaultStatus(Axis_t axis, FaultStatus_t *status)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_FAULT_STATUS_PAN, REG_FAULT_STATUS_TILT);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);

    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    status->raw_code = raw;

    /* Decode bits */
    status->short_circuit   = (uint8_t)((raw & FAULT_SHORT_CKT) != 0U);
    status->system_ok       = (uint8_t)((raw & FAULT_SYSTEM_HEALTHY) != 0U);
    status->over_temp       = (uint8_t)((raw & FAULT_OVER_TEMP) != 0U);
    status->over_volt       = (uint8_t)((raw & FAULT_OVER_VOLT) != 0U);
    status->under_volt      = (uint8_t)((raw & FAULT_UNDER_VOLT) != 0U);
    status->lock_rotor      = (uint8_t)((raw & FAULT_LOCK_ROTOR) != 0U);
    status->motion_complete = (uint8_t)((raw & FAULT_MOTION_COMPLETE) != 0U);

    printf("Axis %u Fault Reg: 0x%04X [Temp=%u]\n", axis, raw, status->over_temp);
}

/*----------------------------------------------------------
 * Read temperature from drive (Input Register)
 *----------------------------------------------------------*/
float Read_Temperature(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_TEMP, REG_TILT_TEMP);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float temp = ((float)raw) / 10.0F; /* e.g., scaled ×10 in register */

    return temp;
}

/*----------------------------------------------------------
 * Read numeric fault code from drive
 *----------------------------------------------------------*/
uint16_t Read_FaultCode(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_FAULT_CODE, REG_TILT_FAULT_CODE);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t code = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);

    return code;
}
