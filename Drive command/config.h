#ifndef CONFIG_H
#define CONFIG_H

/*===========================================================
 * Network Configuration
 *===========================================================*/
#define DRIVE_IP_ADDR        "127.0.0.1"
#define DRIVE_PORT_UDP       (1502U)

/*===========================================================
 * Modbus Protocol Constants
 *===========================================================*/
#define MODBUS_UNIT_ID             (0x01U)
#define MODBUS_FUNC_READ_HOLDING   (0x03U)
#define MODBUS_FUNC_READ_INPUT     (0x04U)
#define MODBUS_FUNC_WRITE_SINGLE   (0x06U)
#define MODBUS_FUNC_WRITE_MULTIPLE (0x10U)
#define MODBUS_MAX_RESP            (260U)
#define MODBUS_TIMEOUT_SEC         (1U)

/*===========================================================
 * Axis Definitions
 *===========================================================*/
#define AXIS1_NAME   "PAN"
#define AXIS2_NAME   "TILT"

/*===========================================================
 * Holding Registers (0x03 / 0x10)
 *===========================================================*/
/* ---- PAN Axis (Axis 1) ---- */
#define REG_PAN_POSITION          (282U)
#define REG_PAN_VELOCITY          (284U)
#define REG_PAN_ACCEL             (286U)
#define REG_PAN_DECEL             (288U)
#define REG_PAN_HOME_OFFSET       (310U)
#define REG_PAN_DEG_CORRECTION    (312U)
#define REG_PAN_DEG_POS           (314U)

/* ---- TILT Axis (Axis 2) ---- */
#define REG_TILT_POSITION         (782U)
#define REG_TILT_VELOCITY         (784U)
#define REG_TILT_ACCEL            (786U)
#define REG_TILT_DECEL            (788U)
#define REG_TILT_HOME_OFFSET      (810U)
#define REG_TILT_DEG_CORRECTION   (812U)
#define REG_TILT_DEG_POS          (814U)

/*===========================================================
 * Input Registers (0x04) – ApplicationReadPara (from image)
 *===========================================================*/
/* ---- PAN Axis ---- */
#define REG_PAN_POS_DEG           (412U)
#define REG_PAN_VEL_SPD           (414U)
#define REG_PAN_POS_MM            (416U)
#define REG_PAN_RPM               (418U)
#define REG_PAN_ACTUAL_CURRENT    (420U)
#define REG_PAN_IO_STATUS         (422U)
#define REG_PAN_SYSTEM_STATUS     (424U)
#define REG_PAN_DCBUS_VOLT        (426U)

/* ---- TILT Axis ---- */
#define REG_TILT_POS_DEG          (912U)
#define REG_TILT_VEL_SPD          (914U)
#define REG_TILT_POS_MM           (916U)
#define REG_TILT_RPM              (918U)
#define REG_TILT_ACTUAL_CURRENT   (920U)
#define REG_TILT_IO_STATUS        (922U)
#define REG_TILT_SYSTEM_STATUS    (924U)
#define REG_TILT_DCBUS_VOLT       (926U)

/*===========================================================
 * Command Registers (0x06)
 *===========================================================*/
#define REG_CMD_HALT              (445U)
#define REG_CMD_EMG_STOP          (446U)
#define REG_CMD_ENABLE            (448U)
#define REG_CMD_RESET             (449U)
#define REG_CMD_POS_MOVE          (451U)
#define REG_CMD_HOME_MOVE_DEG     (452U)
#define REG_CMD_VEL_FWD           (453U)
#define REG_CMD_VEL_REV           (454U)
#define REG_CMD_POS_MOVE_DEG      (455U)

/*===========================================================
 * Fault Register
 *===========================================================*/
#define REG_FAULT_STATUS_PAN      (384U)
#define REG_FAULT_STATUS_TILT     (884U)

/* Fault Bits */
#define FAULT_SHORT_CKT           (0x0001U)
#define FAULT_SYSTEM_HEALTHY      (0x0002U)
#define FAULT_OVER_TEMP           (0x0008U)
#define FAULT_OVER_VOLT           (0x0010U)
#define FAULT_UNDER_VOLT          (0x0020U)
#define FAULT_LOCK_ROTOR          (0x0400U)
#define FAULT_MOTION_COMPLETE     (0x8000U)

/*===========================================================
 * Extended Diagnostic Registers
 *===========================================================*/
/* ---- PAN Axis ---- */
#define REG_PAN_TEMP        (428U)
#define REG_PAN_FAULT_CODE  (430U)

/* ---- TILT Axis ---- */
#define REG_TILT_TEMP       (928U)
#define REG_TILT_FAULT_CODE (930U)

/*===========================================================
 * Software Limit Settings (User Editable)
 *===========================================================*/
#define PAN_LIMIT_LEFT_DEG      (-90.0F)
#define PAN_LIMIT_RIGHT_DEG     (90.0F)

#define TILT_LIMIT_DOWN_DEG     (-30.0F)
#define TILT_LIMIT_UP_DEG       (60.0F)

/* Motor & Mechanical Specs */
#define MAX_RPM        (3000.0F)
#define DPMR_MM        (5.0F)      /* mm per one motor revolution */

#define ACCEL_FACTOR   (1.5F)      /* or 2.0F depending on load */

// Pan tilt Limit switch Constant in MM
#define PAN_LIMIT_LEFT_MM     (-100.0F)
#define PAN_LIMIT_RIGHT_MM     (100.0F)

#define TILT_LIMIT_DOWN_MM     (-50.0F)
#define TILT_LIMIT_UP_MM        (50.0F)

#endif /* CONFIG_H */

