#include "config.h"
#include "drive_command.h"
#include "modbus_functions.h"
#include <stdio.h>
#include <stdint.h>

/*----------------------------------------------------------
 * Internal helper to issue a single register write command
 *----------------------------------------------------------*/
static void WriteCommand(uint16_t reg_addr, Axis_t axis)
{
    /* Example: Writing 1U for command execution */
    (void)MODBUS_WriteSingle(MODBUS_UNIT_ID, reg_addr, (uint16_t)axis);
    printf("Command 0x%X executed for Axis %u\n", reg_addr, axis);
}

/*----------------------------------------------------------
 * CMD_Enable - Enable Drive
 *----------------------------------------------------------*/
void CMD_Enable(Axis_t axis)
{
    WriteCommand(REG_CMD_ENABLE, axis);
}

/*----------------------------------------------------------
 * CMD_Reset - Clear Drive Faults
 *----------------------------------------------------------*/
void CMD_Reset(Axis_t axis)
{
    WriteCommand(REG_CMD_RESET, axis);
}

/*----------------------------------------------------------
 * CMD_Halt - Stop Motion Smoothly
 *----------------------------------------------------------*/
void CMD_Halt(Axis_t axis)
{
    WriteCommand(REG_CMD_HALT, axis);
}

/*----------------------------------------------------------
 * CMD_EStop - Immediate Emergency Stop
 *----------------------------------------------------------*/
void CMD_EStop(Axis_t axis)
{
    WriteCommand(REG_CMD_EMG_STOP, axis);
}

/*----------------------------------------------------------
 * CMD_PositionMove - Move to preset target position
 *----------------------------------------------------------*/
void CMD_PositionMove(Axis_t axis)
{
    WriteCommand(REG_CMD_POS_MOVE, axis);
}

/*----------------------------------------------------------
 * CMD_PositionMove_Deg - Move to position in degrees
 *----------------------------------------------------------*/
void CMD_PositionMove_Deg(Axis_t axis)
{
    WriteCommand(REG_CMD_POS_MOVE_DEG, axis);
}

/*----------------------------------------------------------
 * CMD_HomeMove - Move to home position
 *----------------------------------------------------------*/
void CMD_HomeMove(Axis_t axis)
{
    WriteCommand(REG_CMD_HOME_MOVE_DEG, axis);
}

/*----------------------------------------------------------
 * CMD_VelocityFwd - Continuous motion forward
 *----------------------------------------------------------*/
void CMD_VelocityFwd(Axis_t axis)
{
    WriteCommand(REG_CMD_VEL_FWD, axis);
}

/*----------------------------------------------------------
 * CMD_VelocityRev - Continuous motion reverse
 *----------------------------------------------------------*/
void CMD_VelocityRev(Axis_t axis)
{
    WriteCommand(REG_CMD_VEL_REV, axis);
}
