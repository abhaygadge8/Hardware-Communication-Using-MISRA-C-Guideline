#ifndef DRIVE_FAULT_H
#define DRIVE_FAULT_H

#include <stdint.h>
#include "drive_feedback.h"  /* for Axis_t type */

/**
 * @brief Structure representing drive fault and health status
 */
typedef struct
{
    uint16_t raw_code;      /**< Raw fault register value */
    uint8_t short_circuit;
    uint8_t system_ok;
    uint8_t over_temp;
    uint8_t over_volt;
    uint8_t under_volt;
    uint8_t lock_rotor;
    uint8_t motion_complete;
} FaultStatus_t;

/**
 * @brief Read drive fault register and decode fault bits
 * @param axis Axis to read (AXIS_PAN or AXIS_TILT)
 * @param status Pointer to FaultStatus_t structure to populate
 */
void Read_FaultStatus(Axis_t axis, FaultStatus_t *status);

/**
 * @brief Read drive temperature (°C)
 * @param axis Axis to read (AXIS_PAN or AXIS_TILT)
 * @return Temperature in °C
 */
float Read_Temperature(Axis_t axis);

/**
 * @brief Read fault code (numeric code from drive)
 * @param axis Axis to read (AXIS_PAN or AXIS_TILT)
 * @return Raw fault code
 */
uint16_t Read_FaultCode(Axis_t axis);

#endif /* DRIVE_FAULT_H */
