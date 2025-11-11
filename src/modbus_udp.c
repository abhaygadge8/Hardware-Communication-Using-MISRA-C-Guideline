#include "modbus_udp.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

/* Global Modbus UDP socket and server info */
static SOCKET modbus_sock = INVALID_SOCKET;
static struct sockaddr_in modbus_server;
static int modbus_addr_len = sizeof(modbus_server);
static uint16_t modbus_transaction_id = 1U;

/*----------------------------------------------------------
 * Initialize Modbus UDP connection (only once)
 *----------------------------------------------------------*/
ModbusStatus_t MODBUS_UDP_InitConnection(void)
{
    WSADATA wsaData;
    DWORD timeout = (MODBUS_TIMEOUT_SEC * 1000U);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed.\n");
        return MODBUS_ERROR;
    }

    modbus_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (modbus_sock == INVALID_SOCKET)
    {
        printf("Socket creation failed: %d\n", WSAGetLastError());
        WSACleanup();
        return MODBUS_ERROR;
    }

    if (setsockopt(modbus_sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout)) == SOCKET_ERROR)
    {
        printf("Set timeout failed: %d\n", WSAGetLastError());
        closesocket(modbus_sock);
        WSACleanup();
        return MODBUS_ERROR;
    }

    memset(&modbus_server, 0, sizeof(modbus_server));
    modbus_server.sin_family = AF_INET;
    modbus_server.sin_port = htons(DRIVE_PORT_UDP);
    modbus_server.sin_addr.s_addr = inet_addr(DRIVE_IP_ADDR);

    printf("Modbus UDP connection established with %s:%d\n", DRIVE_IP_ADDR, DRIVE_PORT_UDP);
    return MODBUS_OK;
}

/*----------------------------------------------------------
 * Close Modbus UDP connection
 *----------------------------------------------------------*/
void MODBUS_UDP_CloseConnection(void)
{
    if (modbus_sock != INVALID_SOCKET)
    {
        closesocket(modbus_sock);
        modbus_sock = INVALID_SOCKET;
        printf(" Modbus UDP connection closed.\n");
    }
    WSACleanup();
}

/*----------------------------------------------------------
 * Compute Modbus CRC16
 *----------------------------------------------------------*/
uint16_t MODBUS_CRC16(const uint8_t *buffer, uint16_t length)
{
    uint16_t crc = 0xFFFFU;
    for (uint16_t i = 0U; i < length; i++)
    {
        crc ^= buffer[i];
        for (uint16_t j = 0U; j < 8U; j++)
        {
            if (crc & 1U)
                crc = (crc >> 1U) ^ 0xA001U;
            else
                crc >>= 1U;
        }
    }
    return crc;
}

/*----------------------------------------------------------
 * Modbus UDP Read Function
 *----------------------------------------------------------*/
ModbusStatus_t MODBUS_UDP_Read(const char *ip, uint16_t port,
                               uint16_t start_addr, uint16_t num_regs,
                               uint32_t *value)
{
    if (modbus_sock == INVALID_SOCKET)
    {
        printf("Connection not initialized.\n");
        return MODBUS_ERROR;
    }

    uint8_t request[12];
    uint8_t response[MODBUS_MAX_RESP];
    int bytes_received;

    /* Build Modbus UDP request */
    request[0] = (uint8_t)(modbus_transaction_id >> 8);
    request[1] = (uint8_t)(modbus_transaction_id & 0xFFU);
    request[2] = 0x00; request[3] = 0x00;
    request[4] = 0x00; request[5] = 0x06;
    request[6] = MODBUS_UNIT_ID;
    request[7] = MODBUS_READ_FUNC;
    request[8] = (uint8_t)(start_addr >> 8);
    request[9] = (uint8_t)(start_addr & 0xFFU);
    request[10] = (uint8_t)(num_regs >> 8);
    request[11] = (uint8_t)(num_regs & 0xFFU);

    /* Send Modbus request */
    if (sendto(modbus_sock, (const char *)request, 12, 0,
               (struct sockaddr *)&modbus_server, modbus_addr_len) == SOCKET_ERROR)
    {
        printf("Send failed: %d\n", WSAGetLastError());
        return MODBUS_ERROR;
    }

    /* Receive response */
    bytes_received = recvfrom(modbus_sock, (char *)response, sizeof(response), 0, NULL, NULL);
    if (bytes_received == SOCKET_ERROR)
    {
        printf("Receive failed: %d\n", WSAGetLastError());
        return MODBUS_ERROR;
    }

    /* Parse Modbus response */
    if (num_regs == 2U)
    {
        uint16_t reg_hi = (uint16_t)((response[9] << 8U) | response[10]);
        uint16_t reg_lo = (uint16_t)((response[11] << 8U) | response[12]);
        *value = ((uint32_t)reg_hi << 16U) | reg_lo;
    }
    else
    {
        *value = (uint32_t)((response[9] << 8U) | response[10]);
    }

    modbus_transaction_id++;
    return MODBUS_OK;
}

/*----------------------------------------------------------
 * Modbus UDP Write Function
 *----------------------------------------------------------*/
ModbusStatus_t MODBUS_UDP_Write(const char *ip, uint16_t port,
                                uint16_t reg_addr, uint16_t reg_value)
{
    if (modbus_sock == INVALID_SOCKET)
    {
        printf("Connection not initialized.\n");
        return MODBUS_ERROR;
    }

    uint8_t request[12];
    uint8_t response[MODBUS_MAX_RESP];
    int bytes_received;

    request[0] = (uint8_t)(modbus_transaction_id >> 8);
    request[1] = (uint8_t)(modbus_transaction_id & 0xFFU);
    request[2] = 0x00; request[3] = 0x00;
    request[4] = 0x00; request[5] = 0x06;
    request[6] = MODBUS_UNIT_ID;
    request[7] = 0x06;
    request[8] = (uint8_t)(reg_addr >> 8);
    request[9] = (uint8_t)(reg_addr & 0xFFU);
    request[10] = (uint8_t)(reg_value >> 8);
    request[11] = (uint8_t)(reg_value & 0xFFU);

    if (sendto(modbus_sock, (const char *)request, 12, 0,
               (struct sockaddr *)&modbus_server, modbus_addr_len) == SOCKET_ERROR)
    {
        printf("Write send failed: %d\n", WSAGetLastError());
        return MODBUS_ERROR;
    }

    bytes_received = recvfrom(modbus_sock, (char *)response, sizeof(response), 0, NULL, NULL);
    if (bytes_received == SOCKET_ERROR)
    {
        printf("Write receive failed: %d\n", WSAGetLastError());
        return MODBUS_ERROR;
    }

    modbus_transaction_id++;
    return MODBUS_OK;
}
