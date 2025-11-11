#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H

#include "modbus_udp.h"
#include "config.h"

/**
 * @brief Turn ON or OFF Pan motor
 */
ModbusStatus_t Send_Pan_Command(uint16_t cmd);

/**
 * @brief Turn ON or OFF Tilt motor
 */
ModbusStatus_t Send_Tilt_Command(uint16_t cmd);

/**
 * @brief Park both Pan and Tilt motors (move to default position)
 */
ModbusStatus_t Drive_ParkMotion(void);

#endif /* DRIVE_CONTROL_H */
