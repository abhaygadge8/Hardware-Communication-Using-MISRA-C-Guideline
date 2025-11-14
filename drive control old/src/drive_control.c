#include "drive_control.h"
#include <stdio.h>

ModbusStatus_t Send_Pan_Command(uint16_t cmd)
{
    return MODBUS_UDP_Write(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_PAN_CONTROL, cmd);
}

ModbusStatus_t Send_Tilt_Command(uint16_t cmd)
{
    return MODBUS_UDP_Write(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_TILT_CONTROL, cmd);
}
/*----------------------------------------------------------
 * PARK MOTION FUNCTION
 *----------------------------------------------------------*/
ModbusStatus_t Drive_ParkMotion(void)
{
    ModbusStatus_t status;

    printf(" Starting PARK motion for Pan and Tilt...\n");

    /* 1️⃣ Send PARK command (or target position) */
    status = MODBUS_UDP_Write(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_PAN_PARK_POS, 0x0000U);
    if (status != MODBUS_OK)
    {
        printf(" Failed to set Pan park target.\n");
        return MODBUS_ERROR;
    }

    status = MODBUS_UDP_Write(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_TILT_PARK_POS, 0x0000U);
    if (status != MODBUS_OK)
    {
        printf(" Failed to set Tilt park target.\n");
        return MODBUS_ERROR;
    }

    /* 2️⃣ Send START PARK command to both motors */
    (void)Send_Pan_Command(CMD_START_PARK);
    (void)Send_Tilt_Command(CMD_START_PARK);

    /* 3️⃣ Wait for motion to complete (poll until stopped) */
    uint32_t pan_speed = 0U, tilt_speed = 0U;
    for (int i = 0; i < 50; i++) /* 5 seconds total at 100ms each */
    {
        (void)MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_PAN_SPEED, 1U, &pan_speed);
        (void)MODBUS_UDP_Read(DRIVE_IP_ADDR, DRIVE_PORT_UDP, REG_TILT_SPEED, 1U, &tilt_speed);

        printf(" Pan speed=%lu | Tilt speed=%lu\r", 
               (unsigned long)pan_speed, (unsigned long)tilt_speed);
        Sleep(100); /* 100ms delay */

        if ((pan_speed == 0U) && (tilt_speed == 0U))
        {
            printf("\n Both motors parked successfully!\n");
            return MODBUS_OK;
        }
    }

    printf("\n Timeout: motors did not stop within limit.\n");
    return MODBUS_ERROR;
}