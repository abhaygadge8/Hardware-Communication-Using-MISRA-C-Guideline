# Hardware-Communication-Using-MISRA-C-Guideline
Hardware communication using the MODBUS UDP protocol ensures reliable, real-time data exchange between controllers and devices over Ethernet. Implemented in MISRA C for safety-critical systems, it enforces robust coding standards, minimizing errors and enhancing portability, maintainability, and deterministic performance.

# 🌀 Dual-Axis Motor Control System (Modbus UDP)

This project implements a **dual-axis (Pan & Tilt) motor control and feedback system** using the **Modbus UDP protocol**.  
It provides a command-line interface (CLI) to control both motors, read feedback, perform park motions, and check limit switch angles safely.

---

## ⚙️ Features

- ✅ Modbus UDP-based communication  
- ✅ Independent **Pan** and **Tilt** control  
- ✅ Real-time feedback (position, speed, encoder counts)  
- ✅ ON/OFF and Park Motion commands  
- ✅ Limit switch state and angle monitoring  
- ✅ Safe motor shutdown before exit  

---

## 🧩 Project Structure
```
Hardware Communication/
│
├── include/
│ ├── config.h
│ ├── drive_control.h
│ ├── drive_feedback.h
│ ├── limit_angle.h
│ └── modbus_udp.h
│
├── src/
│ ├── drive_control.c
│ ├── drive_feedback.c
│ ├── limit_angle.c
│ ├── main.c
│ └── modbus_udp.c
│
├── test/
│ ├── test_drive_control.c
│ ├── test_drive_control.exe
│ ├── test_drive_feedback.c
│ ├── test_drive_feedback.exe
│ ├── test_limit_angle.c
│ ├── test_limit_angle.exe
│ ├── test_modbus_udp.exe
│ ├── unity.c
│ ├── unity.h
│ └── unity_internals.h
│
├── drive_control.c.gcov
├── dual_axis_app.exe
├── makefile
└── wcs_cmd_eth.exe
└── README.md
```
---

##  Requirements

| Component | Description |
|------------|--------------|
| **OS** | Windows (tested with MinGW64) |
| **Compiler** | GCC (MinGW) |
| **Dependencies** | Winsock2 (Windows Sockets API) |
| **Tools** | `make` (for automated build) |

---
##  Menu Options
| Option | Description                           |
| ------ | ------------------------------------- |
| 1      | Turn ON Pan Motor                     |
| 2      | Turn OFF Pan Motor                    |
| 3      | Turn ON Tilt Motor                    |
| 4      | Turn OFF Tilt Motor                   |
| 5      | Read Encoder Feedback (Pan & Tilt)    |
| 6      | Execute Park Motion (Both Motors)     |
| 7      | Read Limit Switch Angles (Pan & Tilt) |
| 0      | Exit Program                          |

 ##  Source File Descriptions
| File                      | Description                                        |
| ------------------------- | -------------------------------------------------- |
| `main.c`                  | Main program entry with menu-based control         |
| `modbus_udp.c` / `.h`     | Handles UDP socket communication                   |
| `drive_control.c` / `.h`  | Sends ON/OFF and Park commands                     |
| `drive_feedback.c` / `.h` | Reads position, speed, encoder count feedback      |
| `limit_angle.c` / `.h`    | Reads limit switch states and corresponding angles |
| `config.h`                | Defines IP, port, and system constants             |
| `Makefile`                | Automates build and clean operations               |

## Developer Notes
- Ensure the drive controllers and PC are on the same subnet.
- Adjust encoder scaling or gearbox ratio in feedback logic if required.
- Limit switch readings are safety-critical — test carefully before operation.
- For continuous control, integrate a real-time task or event loop.

