#include "config.h"
#include "modbus_functions.h"
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdint.h>

/*----------------------------------------------------------
 * CRC16 Calculation (Modbus RTU)
 *----------------------------------------------------------*/
static uint16_t MODBUS_CRC16(const uint8_t *buffer, uint16_t length)
{
    uint16_t crc = 0xFFFFU;
    uint16_t i, j;

    for (i = 0U; i < length; i++)
    {
        crc ^= buffer[i];
        for (j = 0U; j < 8U; j++)
        {
            if ((crc & 0x0001U) != 0U)
            {
                crc = (crc >> 1U) ^ 0xA001U;
            }
            else
            {
                crc >>= 1U;
            }
        }
    }
    return crc;
}

/*----------------------------------------------------------
 * UDP Globals
 *----------------------------------------------------------*/
static SOCKET modbus_socket = INVALID_SOCKET;
static struct sockaddr_in modbus_target;
static int modbus_target_len = sizeof(modbus_target);

/*----------------------------------------------------------
 * Initialize UDP connection
 *----------------------------------------------------------*/
void MODBUS_Init(void)
{
    WSADATA wsaData;
    (void)WSAStartup(MAKEWORD(2, 2), &wsaData);

    modbus_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    (void)memset(&modbus_target, 0, sizeof(modbus_target));
    modbus_target.sin_family = AF_INET;
    modbus_target.sin_port = htons(DRIVE_PORT_UDP);
    modbus_target.sin_addr.s_addr = inet_addr(DRIVE_IP_ADDR);
}

/*----------------------------------------------------------
 * 1) Read Holding Registers (0x03)
 *----------------------------------------------------------*/
int32_t MODBUS_ReadHolding(uint8_t slave_id, uint16_t start_addr,
                           uint16_t num_regs, uint8_t *rx_buf)
{
    uint8_t tx_buf[8U];
    uint16_t crc;

    tx_buf[0] = slave_id;
    tx_buf[1] = MODBUS_FUNC_READ_HOLDING;
    tx_buf[2] = (uint8_t)(start_addr >> 8U);
    tx_buf[3] = (uint8_t)(start_addr & 0xFFU);
    tx_buf[4] = (uint8_t)(num_regs >> 8U);
    tx_buf[5] = (uint8_t)(num_regs & 0xFFU);

    crc = MODBUS_CRC16(tx_buf, 6U);
    tx_buf[6] = (uint8_t)(crc & 0xFFU);
    tx_buf[7] = (uint8_t)(crc >> 8U);

    (void)sendto(modbus_socket, (const char *)tx_buf, 8, 0,
                 (struct sockaddr *)&modbus_target, modbus_target_len);

    return recvfrom(modbus_socket, (char *)rx_buf, 256, 0, NULL, NULL);
}

/*----------------------------------------------------------
 * 2) Read Input Registers (0x04)
 *----------------------------------------------------------*/
int32_t MODBUS_ReadInput(uint8_t slave_id, uint16_t start_addr,
                         uint16_t num_regs, uint8_t *rx_buf)
{
    uint8_t tx_buf[8U];
    uint16_t crc;

    tx_buf[0] = slave_id;
    tx_buf[1] = MODBUS_FUNC_READ_INPUT;
    tx_buf[2] = (uint8_t)(start_addr >> 8U);
    tx_buf[3] = (uint8_t)(start_addr & 0xFFU);
    tx_buf[4] = (uint8_t)(num_regs >> 8U);
    tx_buf[5] = (uint8_t)(num_regs & 0xFFU);

    crc = MODBUS_CRC16(tx_buf, 6U);
    tx_buf[6] = (uint8_t)(crc & 0xFFU);
    tx_buf[7] = (uint8_t)(crc >> 8U);

    (void)sendto(modbus_socket, (const char *)tx_buf, 8, 0,
                 (struct sockaddr *)&modbus_target, modbus_target_len);
    printf("[MODBUS] Sending to %s:%d | Function 0x%02X | Addr: %u | Count: %u\n",
       DRIVE_IP_ADDR, DRIVE_PORT_UDP, tx_buf[1], start_addr, num_regs);

    //return recvfrom(modbus_socket, (char *)rx_buf, 256, 0, NULL, NULL);
    int len = recvfrom(modbus_socket, (char *)rx_buf, 256, 0, NULL, NULL);
    printf("[MODBUS] Received %d bytes\n", len);
    for (int i = 0; i < len; i++) { printf("%02X ", rx_buf[i]); }
    printf("\n");
    return len;

    
}

/*----------------------------------------------------------
 * 3) Write Single Register (0x06)
 *----------------------------------------------------------*/
int32_t MODBUS_WriteSingle(uint8_t slave_id, uint16_t reg_addr,
                           uint16_t value)
{
    uint8_t tx_buf[8U];
    uint16_t crc;

    tx_buf[0] = slave_id;
    tx_buf[1] = MODBUS_FUNC_WRITE_SINGLE;
    tx_buf[2] = (uint8_t)(reg_addr >> 8U);
    tx_buf[3] = (uint8_t)(reg_addr & 0xFFU);
    tx_buf[4] = (uint8_t)(value >> 8U);
    tx_buf[5] = (uint8_t)(value & 0xFFU);

    crc = MODBUS_CRC16(tx_buf, 6U);
    tx_buf[6] = (uint8_t)(crc & 0xFFU);
    tx_buf[7] = (uint8_t)(crc >> 8U);

    (void)sendto(modbus_socket, (const char *)tx_buf, 8, 0,
                 (struct sockaddr *)&modbus_target, modbus_target_len);

    return recvfrom(modbus_socket, (char *)tx_buf, 256, 0, NULL, NULL);
}

/*----------------------------------------------------------
 * 4) Write Multiple Registers (0x10)
 *----------------------------------------------------------*/
int32_t MODBUS_WriteMultiple(uint8_t slave_id, uint16_t start_addr,
                             uint16_t num_regs, const uint16_t *data)
{
    uint8_t tx_buf[260U];
    uint16_t i;
    uint16_t crc;
    uint16_t idx = 7U;

    tx_buf[0] = slave_id;
    tx_buf[1] = MODBUS_FUNC_WRITE_MULTIPLE;
    tx_buf[2] = (uint8_t)(start_addr >> 8U);
    tx_buf[3] = (uint8_t)(start_addr & 0xFFU);
    tx_buf[4] = (uint8_t)(num_regs >> 8U);
    tx_buf[5] = (uint8_t)(num_regs & 0xFFU);
    tx_buf[6] = (uint8_t)(num_regs * 2U);

    for (i = 0U; i < num_regs; i++)
    {
        tx_buf[idx++] = (uint8_t)(data[i] >> 8U);
        tx_buf[idx++] = (uint8_t)(data[i] & 0xFFU);
    }

    crc = MODBUS_CRC16(tx_buf, idx);
    tx_buf[idx++] = (uint8_t)(crc & 0xFFU);
    tx_buf[idx++] = (uint8_t)(crc >> 8U);

    (void)sendto(modbus_socket, (const char *)tx_buf, idx, 0,
                 (struct sockaddr *)&modbus_target, modbus_target_len);

    return recvfrom(modbus_socket, (char *)tx_buf, 256, 0, NULL, NULL);
}

/*----------------------------------------------------------
 * Close UDP connection
 *----------------------------------------------------------*/
void MODBUS_Close(void)
{
    if (modbus_socket != INVALID_SOCKET)
    {
        (void)closesocket(modbus_socket);
        modbus_socket = INVALID_SOCKET;
        (void)WSACleanup();
    }
}
