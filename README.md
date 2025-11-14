# Hardware-Communication-Using-MISRA-C-Guideline
Hardware communication using the MODBUS UDP protocol ensures reliable, real-time data exchange between controllers and devices over Ethernet. Implemented in MISRA C for safety-critical systems, it enforces robust coding standards, minimizing errors and enhancing portability, maintainability, and deterministic performance.

# 🌀 Dual-Axis Motor Control System (Modbus UDP)

This project implements a **dual-axis (Pan & Tilt) motor control and feedback system** using the **Modbus UDP protocol**.  
It provides a command-line interface (CLI) to control both motors, read feedback, perform park motions, and check limit switch angles safely.

# Dual-Axis Drive Control (PAN / TILT)  
### Modbus RTU-over-UDP Communication • C Language • Full Simulator Included  

---

## 📌 Overview  

This project provides a complete **Dual-Axis Motor Drive Control System** using:

- **Modbus RTU over UDP** communication  
- **C language drive controller**
- **Full Modbus RTU-UDP simulator (Python)** for offline testing  
- Supports **PAN (Axis-1)** and **TILT (Axis-2)** motors  

This allows you to test and validate drive communication **without actual hardware**.

---

## 🧩 Project Structure
```
Hardware Communication/
│
├── main.c # Main control menu (user interface)
├── config.h # All register addresses & Modbus constants
│
├── modbus_functions.c # UDP send/recv + RTU CRC + Modbus frame builder
├── modbus_functions.h
│
├── drive_feedback.c # Read position, velocity, current, temp, faults
├── drive_feedback.h
│
├── drive_parameters.c # Write parameters (pos, vel, accel, decel)
├── drive_parameters.h
│
├── drive_command.c # Drive control commands (enable, reset, stop)
├── drive_command.h
│
├── rtu_udp_server.py # Python Modbus RTU-over-UDP full simulator
│
└── README.md # Documentation
```
## 🚀 Features  

### ✔ Supported Modbus Function Codes  
| Function | Description |
|---------|-------------|
| **0x03** | Read Holding Registers |
| **0x04** | Read Input Registers |
| **0x06** | Write Single Register |
| **0x10** | Write Multiple Registers |

---
## 🎯 Drive Feedback Supported (0x04)  
- Position (deg, mm)  
- Velocity  
- RPM  
- Actual Current  
- IO Status  
- System Status  
- DC Bus Voltage  
- Temperature  
- Fault Code  

---

## ⚙️ Drive Parameter Write (0x03 / 0x10)  
- Set Position  
- Set Velocity  
- Set Acceleration  
- Set Deceleration  
- Set Home Offset  
- Set Degree Correction  
- Set Degree Position  
- Write Multiple Motion Params in a single command  

---

## 🔧 Drive Control Commands (0x06)  
- Enable  
- Disable  
- Reset  
- Halt / Stop  
- Emergency Stop  
- Position Move  
- Home Move  
- Velocity Forward / Reverse  

---

# 🖥 How to Build the Project (Windows / MinGW)

Use GCC:

```sh
gcc main.c modbus_functions.c drive_feedback.c drive_parameters.c drive_command.c -lws2_32 -o drive_control.exe
