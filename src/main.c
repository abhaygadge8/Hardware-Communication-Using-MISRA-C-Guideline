#include <stdio.h>
#include <stdlib.h>
#include <windows.h>           /* For Sleep() */
#include "config.h"
#include "modbus_udp.h"
#include "drive_feedback.h"
#include "drive_control.h"
#include "limit_angle.h"       /* 🆕 Added for limit switch API */

/*----------------------------------------------------------
 * Display Menu
 *----------------------------------------------------------*/
static void DisplayMenu(void)
{
    printf("\n==============================================\n");
    printf(" Dual-Axis Motor Control Menu\n");
    printf("==============================================\n");
    printf(" [1]  Turn ON Pan Motor\n");
    printf(" [2]  Turn OFF Pan Motor\n");
    printf(" [3]  Turn ON Tilt Motor\n");
    printf(" [4]  Turn OFF Tilt Motor\n");
    printf(" [5]  Read Encoder Feedback (Pan & Tilt)\n");
    printf(" [6]  Execute Park Motion (Both Motors)\n");
    printf(" [7]  Read Limit Switch Angles (Pan & Tilt)\n"); /* 🆕 new option */
    printf(" [0]  Exit Program\n");
    printf("----------------------------------------------\n");
    printf(" Enter your choice: ");
}

/*----------------------------------------------------------
 * Main Program Entry
 *----------------------------------------------------------*/
int main(void)
{
    int choice = -1;
    AxisFeedback_t pan_fb;
    AxisFeedback_t tilt_fb;
    LimitAngle_t pan_limit;
    LimitAngle_t tilt_limit;

    printf("==============================================\n");
    printf(" Dual-Axis Motor Control & Feedback (Modbus UDP)\n");
    printf("==============================================\n");

    /* 1️⃣ Initialize Modbus UDP connection */
    if (MODBUS_UDP_InitConnection() != MODBUS_OK)
    {
        printf(" Failed to initialize Modbus UDP connection.\n");
        return EXIT_FAILURE;
    }

    printf(" Modbus UDP connection established to %s:%u\n",
           DRIVE_IP_ADDR, (unsigned int)DRIVE_PORT_UDP);

    /* 2️⃣ Initialize Limit Switch Module */
    if (LimitAngle_Init() != MODBUS_OK)
    {
        printf("   Limit Switch module initialization failed.\n");
    }

    /*------------------------------------------------------
     * 3️⃣ Interactive Menu Loop
     *------------------------------------------------------*/
    do
    {
        DisplayMenu();
        (void)scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                (void)Send_Pan_Command(CMD_MOTOR_ON);
                break;

            case 2:
                (void)Send_Pan_Command(CMD_MOTOR_OFF);
                break;

            case 3:
                (void)Send_Tilt_Command(CMD_MOTOR_ON);
                break;

            case 4:
                (void)Send_Tilt_Command(CMD_MOTOR_OFF);
                break;

            case 5:
                if ((Read_Pan_Feedback(&pan_fb) == MODBUS_OK) &&
                    (Read_Tilt_Feedback(&tilt_fb) == MODBUS_OK))
                {
                    printf("\n--- PAN AXIS ---\n");
                    printf(" Position: %.2f°\n", pan_fb.position_deg);
                    printf(" Speed   : %.2f RPM\n", pan_fb.speed_rpm);
                    printf(" Count/s : %.2f\n", pan_fb.counts_per_sec);

                    printf("\n--- TILT AXIS ---\n");
                    printf(" Position: %.2f°\n", tilt_fb.position_deg);
                    printf(" Speed   : %.2f RPM\n", tilt_fb.speed_rpm);
                    printf(" Count/s : %.2f\n", tilt_fb.counts_per_sec);
                }
                else
                {
                    printf("  Failed to read feedback from one or both axes.\n");
                }
                break;

            case 6:
                printf("\n Starting Park Motion...\n");
                if (Drive_ParkMotion() == MODBUS_OK)
                {
                    printf(" Both motors parked successfully.\n");
                }
                else
                {
                    printf(" Park Motion failed or timed out.\n");
                }
                break;

            /* 🆕 Case 7: Read Limit Switches & Angles */
            case 7:
                if ((LimitAngle_ReadPan(&pan_limit) == MODBUS_OK) &&
                    (LimitAngle_ReadTilt(&tilt_limit) == MODBUS_OK))
                {
                    printf("\n--- PAN LIMITS ---\n");
                    printf(" Upper: %s (%.2f°)\n",
                           (pan_limit.upper_state != 0U) ? "ACTIVE" : "INACTIVE",
                           pan_limit.upper_angle_deg);
                    printf(" Lower: %s (%.2f°)\n",
                           (pan_limit.lower_state != 0U) ? "ACTIVE" : "INACTIVE",
                           pan_limit.lower_angle_deg);

                    printf("\n--- TILT LIMITS ---\n");
                    printf(" Upper: %s (%.2f°)\n",
                           (tilt_limit.upper_state != 0U) ? "ACTIVE" : "INACTIVE",
                           tilt_limit.upper_angle_deg);
                    printf(" Lower: %s (%.2f°)\n",
                           (tilt_limit.lower_state != 0U) ? "ACTIVE" : "INACTIVE",
                           tilt_limit.lower_angle_deg);
                }
                else
                {
                    printf(" Failed to read one or more limit switches.\n");
                }
                break;

            case 0:
                printf(" Exiting program...\n");
                break;

            default:
                printf(" Invalid choice. Please try again.\n");
                break;
        }

        Sleep(500U); /* Small delay between operations */
    } while (choice != 0);

    /*------------------------------------------------------
     * 4️⃣ Safety Shutdown Before Exit
     *------------------------------------------------------*/
    (void)Send_Pan_Command(CMD_MOTOR_OFF);
    (void)Send_Tilt_Command(CMD_MOTOR_OFF);

    /*------------------------------------------------------
     * 5️⃣ Close Modbus UDP connection
     *------------------------------------------------------*/
    MODBUS_UDP_CloseConnection();
    printf(" Connection closed. Goodbye!\n");

    return EXIT_SUCCESS;
}










/*#include <stdio.h>
#include "drive_control.h"
#include "drive_feedback.h"

int main(void)
{
    AxisFeedback_t pan_fb;
    AxisFeedback_t tilt_fb;

    // Turn ON both motors 
    if (Send_Pan_Command(CMD_MOTOR_ON) == MODBUS_OK)
        printf("Pan Motor: ON Command Sent\n");

    if (Send_Tilt_Command(CMD_MOTOR_ON) == MODBUS_OK)
        printf("Tilt Motor: ON Command Sent\n");

    // Read feedback 
    (void)Read_Pan_Feedback(&pan_fb);
    (void)Read_Tilt_Feedback(&tilt_fb);

    printf("PAN Pos=%lu  Speed=%lu\n", (unsigned long)pan_fb.position, (unsigned long)pan_fb.speed);
    printf("TILT Pos=%lu  Speed=%lu\n", (unsigned long)tilt_fb.position, (unsigned long)tilt_fb.speed);

    // Turn OFF both motors 
    (void)Send_Pan_Command(CMD_MOTOR_OFF);
    (void)Send_Tilt_Command(CMD_MOTOR_OFF);
    printf("Motors turned OFF\n");

    return 0;
}*/



/*#include <stdio.h>
#include "drive_feedback.h"
#include "config.h"

int main(void)
{
    AxisFeedback_t pan_fb = {0U};
    AxisFeedback_t tilt_fb = {0U};

    if (Read_Pan_Feedback(&pan_fb) == MODBUS_OK)
    {
        printf("PAN Axis - Pos: %lu  Speed: %lu  Counts: %lu\n",
               (unsigned long)pan_fb.position,
               (unsigned long)pan_fb.speed,
               (unsigned long)pan_fb.counts);
    }

    if (Read_Tilt_Feedback(&tilt_fb) == MODBUS_OK)
    {
        printf("TILT Axis - Pos: %lu  Speed: %lu  Counts: %lu\n",
               (unsigned long)tilt_fb.position,
               (unsigned long)tilt_fb.speed,
               (unsigned long)tilt_fb.counts);
    }

    return 0;
}*/
