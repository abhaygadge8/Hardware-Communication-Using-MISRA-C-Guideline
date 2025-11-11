#include "drive_feedback.h"
#include "config.h"
#include <string.h>
#include <stdio.h>

/*----------------------------------------------------------
 * Local conversion helpers
 *----------------------------------------------------------*/
static float Convert_Position_To_Degrees(uint32_t counts)
{
    /* position (deg) = (counts * 360) / encoder resolution */
    return ((float)counts * 360.0F) / (float)ENCODER_RESOLUTION;
}

static float Convert_Speed_To_RPM(uint32_t count_per_sec)
{
    /* speed (rpm) = (count/sec * 60) / encoder resolution */
    return ((float)count_per_sec * 60.0F) / (float)ENCODER_RESOLUTION;
}

/*----------------------------------------------------------
 * Read Feedback for PAN Axis
 *----------------------------------------------------------*/
ModbusStatus_t Read_Pan_Feedback(AxisFeedback_t *fb)
{
    if (fb == NULL)
    {
        return MODBUS_ERROR;
    }

    /* Read raw data from Modbus registers */
    if (MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_PAN_POSITION, 2U, &fb->raw_position) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }
    if (MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_PAN_SPEED, 2U, &fb->raw_speed) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }
    if (MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_PAN_COUNTS, 2U, &fb->raw_counts) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }

    /* Compute physical units */
    fb->counts_per_sec = (float)fb->raw_speed;
    fb->speed_rpm      = Convert_Speed_To_RPM(fb->raw_speed);
    fb->position_deg   = Convert_Position_To_Degrees(fb->raw_position);

    return MODBUS_OK;
}

/*----------------------------------------------------------
 * Read Feedback for TILT Axis
 *----------------------------------------------------------*/
ModbusStatus_t Read_Tilt_Feedback(AxisFeedback_t *fb)
{
    if (fb == NULL)
    {
        return MODBUS_ERROR;
    }

    if (MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_TILT_POSITION, 2U, &fb->raw_position) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }
    if (MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_TILT_SPEED, 2U, &fb->raw_speed) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }
    if (MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_TILT_COUNTS, 2U, &fb->raw_counts) != MODBUS_OK)
    {
        return MODBUS_ERROR;
    }

    fb->counts_per_sec = (float)fb->raw_speed;
    fb->speed_rpm      = Convert_Speed_To_RPM(fb->raw_speed);
    fb->position_deg   = Convert_Position_To_Degrees(fb->raw_position);

    return MODBUS_OK;
}
