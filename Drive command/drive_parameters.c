#include "config.h"
#include "drive_parameters.h"
#include "modbus_functions.h"
#include <stdio.h>
#include <stdint.h>

/*----------------------------------------------------------
 * Helper: Select the correct Modbus register address
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
 * Helper: Convert float to scaled uint16_t value
 *----------------------------------------------------------*/
static uint16_t FloatToReg(float value, float scale)
{
    int16_t temp = (int16_t)(value * scale);
    return (uint16_t)temp;
}

/*----------------------------------------------------------
 * Helper: Read back and verify write (optional)
 *----------------------------------------------------------*/
static void VerifyParameterWrite(uint16_t addr)
{
    uint8_t rx_buf[8U];
    (void)MODBUS_ReadHolding(MODBUS_UNIT_ID, addr, 1U, rx_buf);

    uint16_t raw = (uint16_t)(((uint16_t)rx_buf[3U] << 8U) | rx_buf[4U]);
    printf("  ↳ Verified Write: [0x%X] = %u\n", addr, raw);
}

/*----------------------------------------------------------
 * Set Position (degrees)
 *----------------------------------------------------------*/
void Set_Position(Axis_t axis, float deg)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_POSITION, REG_TILT_POSITION);
    uint16_t val = FloatToReg(deg, 100.0F);  /* ×100 scaling */

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set Position = %.2f° (Reg 0x%X)\n", axis, deg, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Set Velocity
 *----------------------------------------------------------*/
void Set_Velocity(Axis_t axis, float vel)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_VELOCITY, REG_TILT_VELOCITY);
    uint16_t val = FloatToReg(vel, 10.0F);

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set Velocity = %.2f (Reg 0x%X)\n", axis, vel, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Set Acceleration
 *----------------------------------------------------------*/
void Set_Acceleration(Axis_t axis, float accel)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_ACCEL, REG_TILT_ACCEL);
    uint16_t val = FloatToReg(accel, 10.0F);

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set Accel = %.2f (Reg 0x%X)\n", axis, accel, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Set Deceleration
 *----------------------------------------------------------*/
void Set_Deceleration(Axis_t axis, float decel)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_DECEL, REG_TILT_DECEL);
    uint16_t val = FloatToReg(decel, 10.0F);

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set Decel = %.2f (Reg 0x%X)\n", axis, decel, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Set Home Offset
 *----------------------------------------------------------*/
void Set_HomeOffset(Axis_t axis, float offset)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_HOME_OFFSET, REG_TILT_HOME_OFFSET);
    uint16_t val = FloatToReg(offset, 100.0F);

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set HomeOffset = %.2f (Reg 0x%X)\n", axis, offset, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Set Degree Correction
 *----------------------------------------------------------*/
void Set_DegCorrection(Axis_t axis, float deg_corr)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_DEG_CORRECTION, REG_TILT_DEG_CORRECTION);
    uint16_t val = FloatToReg(deg_corr, 100.0F);

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set DegCorrection = %.2f (Reg 0x%X)\n", axis, deg_corr, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Set Degree Position
 *----------------------------------------------------------*/
void Set_DegPosition(Axis_t axis, float deg_pos)
{
    uint16_t addr = GetRegisterAddress(axis, REG_PAN_DEG_POS, REG_TILT_DEG_POS);
    uint16_t val = FloatToReg(deg_pos, 100.0F);

    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, addr, val);
    printf("Axis %u: Set DegPosition = %.2f (Reg 0x%X)\n", axis, deg_pos, addr);

    VerifyParameterWrite(addr);
}

/*----------------------------------------------------------
 * Write Multiple Registers (0x10)
 *   position, velocity, acceleration, deceleration
 *----------------------------------------------------------*/
void Set_MotionParameters(Axis_t axis, float pos, float vel, float accel, float decel)
{
    uint16_t start_addr;
    uint16_t reg_data[4U]; /* 4 registers */

    if (axis == AXIS_PAN)
    {
        start_addr = REG_PAN_POSITION;
    }
    else
    {
        start_addr = REG_TILT_POSITION;
    }

    reg_data[0] = FloatToReg(pos,   100.0F);
    reg_data[1] = FloatToReg(vel,   10.0F);
    reg_data[2] = FloatToReg(accel, 10.0F);
    reg_data[3] = FloatToReg(decel, 10.0F);

    (void)MODBUS_WriteMultiple(MODBUS_UNIT_ID, start_addr, 4U, reg_data);

    printf("Axis %u: Multi-param write @0x%X → Pos=%.2f Vel=%.2f Acc=%.2f Dec=%.2f\n",
           axis, start_addr, pos, vel, accel, decel);

    VerifyParameterWrite(start_addr);
}
