#ifndef DRIVE_FEEDBACK_H
#define DRIVE_FEEDBACK_H

#include <stdint.h>
#include "modbus_udp.h"

/*----------------------------------------------------------
 * Axis Feedback Data with physical units
 *----------------------------------------------------------*/
typedef struct
{
    uint32_t raw_position;   /* Raw encoder position (counts) */
    uint32_t raw_speed;      /* Raw encoder speed (counts/sec) */
    uint32_t raw_counts;     /* Raw total count feedback */

    float position_deg;      /* Position converted to degrees */
    float speed_rpm;         /* Speed converted to RPM */
    float counts_per_sec;    /* Encoder speed in counts/sec */
} AxisFeedback_t;

/*----------------------------------------------------------
 * Function Prototypes
 *----------------------------------------------------------*/

/**
 * @brief Reads Pan (Axis 1) encoder feedback and computes derived units
 * @param fb Pointer to AxisFeedback_t structure
 * @return MODBUS_OK / MODBUS_ERROR
 */
ModbusStatus_t Read_Pan_Feedback(AxisFeedback_t *fb);

/**
 * @brief Reads Tilt (Axis 2) encoder feedback and computes derived units
 * @param fb Pointer to AxisFeedback_t structure
 * @return MODBUS_OK / MODBUS_ERROR
 */
ModbusStatus_t Read_Tilt_Feedback(AxisFeedback_t *fb);

#endif /* DRIVE_FEEDBACK_H */
