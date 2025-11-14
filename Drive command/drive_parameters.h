#ifndef DRIVE_PARAMETERS_H
#define DRIVE_PARAMETERS_H

#include <stdint.h>
#include "drive_feedback.h"  /* For Axis_t type */

/**
 * @brief Set target position (degrees)
 */
void Set_Position(Axis_t axis, float deg);

/**
 * @brief Set velocity (speed units)
 */
void Set_Velocity(Axis_t axis, float vel);

/**
 * @brief Set acceleration
 */
void Set_Acceleration(Axis_t axis, float accel);

/**
 * @brief Set deceleration
 */
void Set_Deceleration(Axis_t axis, float decel);

/**
 * @brief Set home offset (position calibration)
 */
void Set_HomeOffset(Axis_t axis, float offset);

/**
 * @brief Set degree correction (error compensation)
 */
void Set_DegCorrection(Axis_t axis, float deg_corr);

/**
 * @brief Set degree position (fine tuning)
 */
void Set_DegPosition(Axis_t axis, float deg_pos);

/**
 * @brief Set motion parameters (position, velocity, acceleration, deceleration)
 *        in one Modbus multi-register command (0x10)
 *
 * @param axis   Axis to command (AXIS_PAN or AXIS_TILT)
 * @param pos    Target position in degrees
 * @param vel    Velocity
 * @param accel  Acceleration
 * @param decel  Deceleration
 */
void Set_MotionParameters(Axis_t axis, float pos, float vel, float accel, float decel);


#endif /* DRIVE_PARAMETERS_H */
