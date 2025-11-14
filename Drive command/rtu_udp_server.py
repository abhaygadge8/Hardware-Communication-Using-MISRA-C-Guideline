import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 502

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

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

print("🔥 RTU-UDP Simulator running on 127.0.0.1:502 (NO MBAP HEADER)")
print("   Accepting RTU frames exactly like your C program.\n")

while True:
    data, addr = sock.recvfrom(256)
    print("RX:", data.hex())

    if len(data) < 6:
        print("Invalid RTU frame\n")
        continue

    slave = data[0]
    func  = data[1]
    addr_hi = data[2]
    addr_lo = data[3]

    reg_addr = (addr_hi << 8) | addr_lo

    # Example: return 0x1234 for ANY register
    response = bytes([slave, func, 2, 0x12, 0x34])

    # add CRC
    crc = calc_crc(response)
    response += bytes([crc & 0xFF, crc >> 8])

    print("TX:", response.hex(), "\n")
    sock.sendto(response, addr)
