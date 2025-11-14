#ifndef CONFIG_H
#define CONFIG_H

/*===========================================================
 * Network Configuration
 *===========================================================*/
#define DRIVE_IP_ADDR        "192.168.0.10"
#define DRIVE_PORT_UDP       (502U)

#define LCU_IP_ADDR          "192.168.0.20"
#define LCU_PORT_UDP         (502U)

#define ENCODER_RESOLUTION        (4096U)   /* counts per revolution */
#define GEAR_RATIO                (1.0F)


/*===========================================================
 * Modbus Common Settings
 *===========================================================*/
#define MODBUS_UNIT_ID       (0x01U)
#define MODBUS_READ_FUNC     (0x03U)
#define MODBUS_WRITE_FUNC    (0x06U)
#define MODBUS_MAX_RESP      (260U)
#define MODBUS_TIMEOUT_SEC   (1U)

/*===========================================================
 * Dual-Axis Drive Register Map
 * Axis 1 = PAN, Axis 2 = TILT
 *===========================================================*/

/* ---------- PAN Axis (Axis 1) ---------- */
#define REG_PAN_POSITION     (0x1000U)
#define REG_PAN_SPEED        (0x1002U)
#define REG_PAN_COUNTS       (0x1004U)
#define REG_PAN_CONTROL      (0x2000U)
#define REG_PAN_PARK_POS     (0x3000U)   /* Park target position */

/* ---------- TILT Axis (Axis 2) ---------- */
#define REG_TILT_POSITION    (0x1100U)
#define REG_TILT_SPEED       (0x1102U)
#define REG_TILT_COUNTS      (0x1104U)
#define REG_TILT_CONTROL     (0x2100U)
#define REG_TILT_PARK_POS    (0x3100U)   /* Park target position */

/*===========================================================
 * Control Commands (sent via Write Register)
 *===========================================================*/
#define CMD_MOTOR_OFF        (0x0000U)
#define CMD_MOTOR_ON         (0x0001U)
#define CMD_START_PARK       (0x0002U)  /* Command to initiate park motion */

/*===========================================================
 * Park Motion Configuration
 *===========================================================*/
#define PAN_PARK_TARGET_POS  (0x0000U)  /* Example park coordinate */
#define TILT_PARK_TARGET_POS (0x0000U)

/* ---------------- Limit Switch Register Map -------------- */
/* Each switch gives binary 1/0 signal (pressed/released) */
#define REG_PAN_LIMIT_UPPER   (0x4000U)
#define REG_PAN_LIMIT_LOWER   (0x4001U)
#define REG_TILT_LIMIT_UPPER  (0x4010U)
#define REG_TILT_LIMIT_LOWER  (0x4011U)

/* ---------------- Angular Limit Definitions -------------- */
#define PAN_LIMIT_UPPER_DEG   (180.0F)
#define PAN_LIMIT_LOWER_DEG   (0.0F)
#define TILT_LIMIT_UPPER_DEG  (90.0F)
#define TILT_LIMIT_LOWER_DEG  (0.0F)

#endif /* CONFIG_H */
