#include "config.h"
#include "drive_feedback.h"
#include "modbus_functions.h"
#include <stdio.h>
#include <stdint.h>

/*----------------------------------------------------------
 * Internal helper to get register address based on axis and type
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
 * Read Position (Degrees)
 *----------------------------------------------------------*/
float Read_Position_Deg(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_POS_DEG, REG_TILT_POS_DEG);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);

    /* Combine bytes (Big Endian) */
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float position = ((float)raw) / 100.0F; /* scaled by *100 per documentation */
    return position;
}

/*----------------------------------------------------------
 * Read Position (Millimeters)
 *----------------------------------------------------------*/
float Read_Position_MM(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_POS_MM, REG_TILT_POS_MM);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float pos_mm = ((float)raw) / 100.0F;
    return pos_mm;
}

/*----------------------------------------------------------
 * Read Velocity
 *----------------------------------------------------------*/
float Read_Velocity(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_VEL_SPD, REG_TILT_VEL_SPD);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float velocity = (float)raw;
    return velocity;
}

/*----------------------------------------------------------
 * Read RPM
 *----------------------------------------------------------*/
float Read_RPM(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_RPM, REG_TILT_RPM);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float rpm = (float)raw;
    return rpm;
}

/*----------------------------------------------------------
 * Read Actual Current
 *----------------------------------------------------------*/
float Read_Current(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_ACTUAL_CURRENT, REG_TILT_ACTUAL_CURRENT);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float current = ((float)raw) / 10.0F; /* Example scale factor */
    return current;
}

/*----------------------------------------------------------
 * Read DC Bus Voltage
 *----------------------------------------------------------*/
float Read_DCBusVoltage(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_DCBUS_VOLT, REG_TILT_DCBUS_VOLT);

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    float voltage = ((float)raw);
    return voltage;
}

/*----------------------------------------------------------
 * Read I/O Status (Input Register 0x04)
 *----------------------------------------------------------*/
float Read_IOStatus(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = (axis == AXIS_PAN) ? REG_PAN_IO_STATUS : REG_TILT_IO_STATUS;

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = ((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U];

    /* Return as-is; user can decode bits externally */
    return (float)raw;
}

/*----------------------------------------------------------
 * Read System Status (Input Register 0x04)
 *----------------------------------------------------------*/
float Read_SystemStatus(Axis_t axis)
{
    uint8_t rx_buf[8U];
    uint16_t addr = (axis == AXIS_PAN) ? REG_PAN_SYSTEM_STATUS : REG_TILT_SYSTEM_STATUS;

    (void)MODBUS_ReadInput(MODBUS_UNIT_ID, addr, 1U, rx_buf);
    uint16_t raw = ((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U];

    /* Return as-is; user can decode system bits later */
    return (float)raw;
}

