#ifndef DRIVE_FEEDBACK_H
#define DRIVE_FEEDBACK_H

#include <stdint.h>

/* Axis enumeration */
typedef enum
{
    AXIS_PAN = 1U,
    AXIS_TILT = 2U
} Axis_t;

/**
 * @brief Read position in degrees from the drive (Input Reg 0x04)
 */
float Read_Position_Deg(Axis_t axis);

/**
 * @brief Read position in millimeters from the drive (Input Reg 0x04)
 */
float Read_Position_MM(Axis_t axis);

/**
 * @brief Read velocity feedback (Input Reg 0x04)
 */
float Read_Velocity(Axis_t axis);

/**
 * @brief Read RPM feedback (Input Reg 0x04)
 */
float Read_RPM(Axis_t axis);

/**
 * @brief Read actual current feedback (Input Reg 0x04)
 */
float Read_Current(Axis_t axis);

/**
 * @brief Read DC Bus voltage feedback (Input Reg 0x04)
 */
float Read_DCBusVoltage(Axis_t axis);

float Read_IOStatus(Axis_t axis);
float Read_SystemStatus(Axis_t axis);

#endif /* DRIVE_FEEDBACK_H */
