#ifndef LIMIT_ANGLE_H
#define LIMIT_ANGLE_H

#include <stdint.h>
#include "modbus_udp.h"

/*----------------------------------------------------------
 * Data Structures
 *----------------------------------------------------------*/
typedef struct
{
    uint8_t upper_state;   /* 1 = Active, 0 = Inactive */
    uint8_t lower_state;   /* 1 = Active, 0 = Inactive */
    float   upper_angle_deg;
    float   lower_angle_deg;
} LimitAngle_t;

/*----------------------------------------------------------
 * Function Prototypes
 *----------------------------------------------------------*/

/**
 * @brief Initialize the limit angle module
 * @return MODBUS_OK / MODBUS_ERROR
 */
ModbusStatus_t LimitAngle_Init(void);

/**
 * @brief Read Pan Axis Limit Angles
 * @param limit Pointer to LimitAngle_t
 * @return MODBUS_OK / MODBUS_ERROR
 */
ModbusStatus_t LimitAngle_ReadPan(LimitAngle_t *limit);

/**
 * @brief Read Tilt Axis Limit Angles
 * @param limit Pointer to LimitAngle_t
 * @return MODBUS_OK / MODBUS_ERROR
 */
ModbusStatus_t LimitAngle_ReadTilt(LimitAngle_t *limit);

#endif /* LIMIT_ANGLE_H */
