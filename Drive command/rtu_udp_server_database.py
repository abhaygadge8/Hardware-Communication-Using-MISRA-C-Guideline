import socket
import threading
import time


UDP_IP = "127.0.0.1"
UDP_PORT = 502


# =========================================================
# CRC FUNCTION (same as your C code)
# =========================================================
def calc_crc(data):
    crc = 0xFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            if crc & 1:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc >>= 1
    return crc


# =========================================================
# FULL DATABASE OF ALL REGISTERS USED BY C PROGRAM
# =========================================================
HR = {}   # Holding registers (0x03, 0x10)
IR = {}   # Input registers (0x04)


def init_registers():
    # ------------------------------
    # PAN INPUT REGISTERS (0x04)
    # ------------------------------
    IR[412] = 2500   # deg *100
    IR[414] = 120    # vel *10
    IR[416] = 5000   # mm *100
    IR[418] = 1500   # rpm
    IR[420] = 15     # current
    IR[422] = 1      # I/O status
    IR[424] = 0      # system status
    IR[426] = 540    # DC bus
    IR[428] = 45     # Temperature PAN
    IR[430] = 0      # Fault code PAN

    # ------------------------------
    # TILT INPUT REGISTERS (0x04)
    # ------------------------------
    IR[912] = 2800
    IR[914] = 90
    IR[916] = 4500
    IR[918] = 1300
    IR[920] = 12
    IR[922] = 1
    IR[924] = 0
    IR[926] = 520
    IR[928] = 48
    IR[930] = 0

    # ------------------------------
    # PAN HOLDING REGISTERS
    # ------------------------------
    HR[282] = 1000   # position
    HR[284] = 100
    HR[286] = 50
    HR[288] = 50
    HR[310] = 0
    HR[312] = 0
    HR[314] = 0

    # ------------------------------
    # TILT HOLDING REGISTERS
    # ------------------------------
    HR[782] = 2000
    HR[784] = 80
    HR[786] = 40
    HR[788] = 40
    HR[810] = 0
    HR[812] = 0
    HR[814] = 0

    # -------------------------------
    # FAULT REGISTERS
    # -------------------------------
    HR[384] = 0
    HR[884] = 0

    # -------------------------------
    # COMMAND REGISTERS (write only)
    # -------------------------------
    HR[445] = 0     # halt
    HR[446] = 0     # estop
    HR[448] = 0     # enable
    HR[449] = 0     # reset
    HR[451] = 0     # pos move
    HR[452] = 0     # home move
    HR[453] = 0     # vel fwd
    HR[454] = 0     # vel rev
    HR[455] = 0     # pos move deg


# =========================================================
# RTU-UDP SERVER (WORKS WITH YOUR C PROGRAM)
# =========================================================
def process_request(data):
    if len(data) < 4:
        return None

    slave = data[0]
    func = data[1]
    addr = (data[2] << 8) | data[3]

    # ------------- READ HOLDING (0x03) -------------
    if func == 0x03:
        count = (data[4] << 8) | data[5]
        values = []
        for i in range(count):
            v = HR.get(addr + i, 0)
            values.append((v >> 8) & 0xFF)
            values.append(v & 0xFF)
        response = bytes([slave, func, len(values)]) + bytes(values)

    # ------------- READ INPUT (0x04) -------------
    elif func == 0x04:
        count = (data[4] << 8) | data[5]
        values = []
        for i in range(count):
            v = IR.get(addr + i, 0)
            values.append((v >> 8) & 0xFF)
            values.append(v & 0xFF)
        response = bytes([slave, func, len(values)]) + bytes(values)

    # ------------- WRITE SINGLE REGISTER (0x06) -------------
    elif func == 0x06:
        value = (data[4] << 8) | data[5]
        HR[addr] = value
        response = data[:6]  # echo back

    # ------------- WRITE MULTIPLE REGISTERS (0x10) -------------
    elif func == 0x10:
        count = (data[4] << 8) | data[5]
        byte_count = data[6]
        base = 7
        for i in range(count):
            hi = data[base + i * 2]
            lo = data[base + i * 2 + 1]
            HR[addr + i] = (hi << 8) | lo

        # Reply contains start addr + count
        response = bytes([slave, func, data[2], data[3], data[4], data[5]])

    else:
        return None

    # Add CRC
    crc = calc_crc(response)
    response += bytes([crc & 0xFF, crc >> 8])
    return response


def start_rtu_udp_server():
    init_registers()

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((UDP_IP, UDP_PORT))

    print("🔥 FULL RTU-UDP Simulator running at 127.0.0.1:502")
    print("   ✓ All registers loaded automatically")
    print("   ✓ Works with your C program (CRC, RTU frame)")
    print("-------------------------------------------------\n")

    while True:
        data, addr = sock.recvfrom(256)
        print("RX:", data.hex())

        response = process_request(data)
        if response:
            print("TX:", response.hex(), "\n")
            sock.sendto(response, addr)


# =========================================================
# MAIN
# =========================================================
if __name__ == "__main__":
    start_rtu_udp_server()
