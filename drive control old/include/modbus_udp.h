#ifndef MODBUS_UDP_H
#define MODBUS_UDP_H

#include <stdint.h>

/* ==========================================================
 * Platform-specific includes
 * ========================================================== */
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <unistd.h>
    #include <netinet/in.h>
    #include <sys/time.h>
#endif

typedef enum {
    MODBUS_OK = 0,
    MODBUS_ERROR = -1
} ModbusStatus_t;

/* Connection control */
ModbusStatus_t MODBUS_UDP_InitConnection(void);
void MODBUS_UDP_CloseConnection(void);

/* CRC calculation */
uint16_t MODBUS_CRC16(const uint8_t *buffer, uint16_t length);

/* Read/Write operations */
ModbusStatus_t MODBUS_UDP_Read(const char *ip, uint16_t port,
                               uint16_t start_addr, uint16_t num_regs,
                               uint32_t *value);

ModbusStatus_t MODBUS_UDP_Write(const char *ip, uint16_t port,
                                uint16_t reg_addr, uint16_t reg_value);

#endif /* MODBUS_UDP_H */