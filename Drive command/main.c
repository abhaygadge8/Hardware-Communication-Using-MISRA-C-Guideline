#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "config.h"
#include "modbus_functions.h"
#include "drive_feedback.h"
#include "drive_parameters.h"
#include "drive_command.h"
#include "drive_fault.h"

/*----------------------------------------------------------
 * Menu Helper Functions
 *----------------------------------------------------------*/

static void PrintMainMenu(void)
{
    printf("\n=============== DRIVE CONTROL MENU ===============\n");
    printf("1. Read Drive Feedback(0x04)\n");
    printf("2. Write Single Parameter (0x06)\n");
    printf("3. Write Multiple Parameters (0x10)\n");
    printf("4. Send Drive Command (0x06)\n");
    printf("5. Read Fault / Temperature / Fault Code\n");
    printf("6. Exit\n");
    printf("==================================================\n");
    printf("Enter choice: ");
}

static Axis_t SelectAxis(void)
{
    int input = 0;
    printf("Select Axis (1=PAN, 2=TILT): ");
    (void)scanf("%d", &input);
    if (input == 2)
    {
        return AXIS_TILT;
    }
    return AXIS_PAN;
}

/*----------------------------------------------------------
 * Menu 1: Read Feedback
 *----------------------------------------------------------*/
static void Menu_ReadFeedback(void)
{
    Axis_t axis = SelectAxis();

    float pos_deg = Read_Position_Deg(axis);
    float pos_mm  = Read_Position_MM(axis);
    float vel     = Read_Velocity(axis);
    float rpm     = Read_RPM(axis);
    float curr    = Read_Current(axis);
    float vdc     = Read_DCBusVoltage(axis);
    float io_stat = Read_IOStatus(axis);
    float sys_stat = Read_SystemStatus(axis);

    printf("\n--- Feedback (Axis %u) ---\n", axis);
    printf("Position: %.2f deg (%.2f mm)\n", pos_deg, pos_mm);
    printf("Velocity: %.2f | RPM: %.2f\n", vel, rpm);
    printf("Current: %.2f A | DC Bus: %.2f V\n", curr, vdc);
    printf("IO Status: %.0f | System Status: %.0f\n", io_stat, sys_stat);
}

/*----------------------------------------------------------
 * Menu 2: Write Single Parameter (0x06)
 *----------------------------------------------------------*/
static void Menu_SetSingleParameter(void)
{
    Axis_t axis = SelectAxis();
    int choice = 0;
    float value = 0.0F;

    printf("\n--- Write Single Parameter ---\n");
    printf("1. Position\n2. Velocity\n3. Acceleration\n4. Deceleration\n");
    printf("5. Home Offset\n6. Degree Correction\n7. Degree Position\n");
    printf("Select parameter: ");
    (void)scanf("%d", &choice);

    printf("Enter value: ");
    (void)scanf("%f", &value);

    switch (choice)
    {
        case 1: Set_Position(axis, value); break;
        case 2: Set_Velocity(axis, value); break;
        case 3: Set_Acceleration(axis, value); break;
        case 4: Set_Deceleration(axis, value); break;
        case 5: Set_HomeOffset(axis, value); break;
        case 6: Set_DegCorrection(axis, value); break;
        case 7: Set_DegPosition(axis, value); break;
        default: printf("Invalid selection.\n"); break;
    }
}

/*----------------------------------------------------------
 * Menu 3: Write Multiple Parameters (0x10)
 *----------------------------------------------------------*/
static void Menu_SetMultiParameter(void)
{
    Axis_t axis = SelectAxis();
    float pos = 0.0F, vel = 0.0F, accel = 0.0F, decel = 0.0F;

    printf("\n--- Write Multiple Parameters ---\n");
    printf("Enter Position (deg): ");
    (void)scanf("%f", &pos);
    printf("Enter Velocity: ");
    (void)scanf("%f", &vel);
    printf("Enter Acceleration: ");
    (void)scanf("%f", &accel);
    printf("Enter Deceleration: ");
    (void)scanf("%f", &decel);

    Set_MotionParameters(axis, pos, vel, accel, decel);
}

/*----------------------------------------------------------
 * Menu 4: Send Drive Command
 *----------------------------------------------------------*/
static void Menu_SendCommand(void)
{
    Axis_t axis = SelectAxis();
    int choice = 0;

    printf("\n--- Drive Commands ---\n");
    printf("1. Enable\n2. Reset\n3. Halt\n4. Emergency Stop\n");
    printf("5. Position Move\n6. Position Move (Degree)\n");
    printf("7. Home Move\n8. Velocity Forward\n9. Velocity Reverse\n");
    printf("Select command: ");
    (void)scanf("%d", &choice);

    switch (choice)
    {
        case 1: CMD_Enable(axis); break;
        case 2: CMD_Reset(axis); break;
        case 3: CMD_Halt(axis); break;
        case 4: CMD_EStop(axis); break;
        case 5: CMD_PositionMove(axis); break;
        case 6: CMD_PositionMove_Deg(axis); break;
        case 7: CMD_HomeMove(axis); break;
        case 8: CMD_VelocityFwd(axis); break;
        case 9: CMD_VelocityRev(axis); break;
        default: printf("Invalid selection.\n"); break;
    }
}

/*----------------------------------------------------------
 * Menu 5: Read Fault / Temperature / Fault Code
 *----------------------------------------------------------*/
static void Menu_ReadFault(void)
{
    Axis_t axis = SelectAxis();
    FaultStatus_t status;
    Read_FaultStatus(axis, &status);

    float temp = Read_Temperature(axis);
    uint16_t code = Read_FaultCode(axis);

    printf("\n--- Fault & Diagnostic Data (Axis %u) ---\n", axis);
    printf("Temperature: %.1f °C | Fault Code: %u\n", temp, code);
    printf("ShortCkt=%u | OverTemp=%u | OverVolt=%u | UnderVolt=%u | LockRotor=%u | MotionDone=%u\n",
           status.short_circuit, status.over_temp, status.over_volt,
           status.under_volt, status.lock_rotor, status.motion_complete);
}

/*----------------------------------------------------------
 * Main Function
 *----------------------------------------------------------*/
int main(void)
{
    int choice = 0;

    MODBUS_Init();
    printf("==================================================\n");
    printf("   Dual Axis Drive Control via UDP Modbus (C)    \n");
    printf("==================================================\n");

    do
    {
        PrintMainMenu();
        (void)scanf("%d", &choice);

        switch (choice)
        {
            case 1: Menu_ReadFeedback(); break;
            case 2: Menu_SetSingleParameter(); break;
            case 3: Menu_SetMultiParameter(); break;
            case 4: Menu_SendCommand(); break;
            case 5: Menu_ReadFault(); break;
            case 6: printf("Closing connection...\n"); break;
            default: printf("Invalid selection.\n"); break;
        }
    } while (choice != 6);

    MODBUS_Close();
    printf("Drive Control Program Terminated.\n");
    return 0;
}
