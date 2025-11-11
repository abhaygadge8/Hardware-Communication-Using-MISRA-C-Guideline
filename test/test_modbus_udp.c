#include "unity.h"
#include "modbus_udp.h"
#include <stdio.h>
#include <stdint.h>

#ifdef _WIN32
#include <winsock2.h>
SOCKET mock_socket = 1; // dummy socket
#endif

// --- MOCKS FOR UDP FUNCTIONS ---
// Use the same calling convention macro as winsock (WSAAPI)
int WSAAPI sendto(SOCKET s, const char *buf, int len, int flags,
                  const struct sockaddr *to, int tolen)
{
    (void)s; (void)flags; (void)to; (void)tolen;
    printf("[Mock sendto] len=%d\n", len);
    return len; // success
}

int WSAAPI recvfrom(SOCKET s, char *buf, int len, int flags,
                    struct sockaddr *from, int *fromlen)
{
    (void)s; (void)flags; (void)from; (void)fromlen;
    uint8_t dummy_resp[] = {
        0x00, 0x01, 0x00, 0x00, 0x00, 0x07,
        0x01, 0x03, 0x04, 0x12, 0x34, 0x56, 0x78
    };
    int resp_len = sizeof(dummy_resp);
    for (int i = 0; i < resp_len && i < len; i++)
        buf[i] = dummy_resp[i];
    return resp_len;
}

// --- UNITY TEST SETUP/CLEANUP ---
void setUp(void) {}
void tearDown(void) {}

// --- TEST CASES ---
void test_MODBUS_CRC16(void)
{
    uint8_t data[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02};
    uint16_t crc = MODBUS_CRC16(data, sizeof(data));
    TEST_ASSERT_EQUAL_HEX16(0x0BC4, crc);
}

void test_MODBUS_UDP_InitConnection(void)
{
    ModbusStatus_t status = MODBUS_UDP_InitConnection();
    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    MODBUS_UDP_CloseConnection();
}

void test_MODBUS_UDP_Read(void)
{
    uint32_t value = 0;
    MODBUS_UDP_InitConnection();
    ModbusStatus_t status = MODBUS_UDP_Read("127.0.0.1", 502, 0x0000, 2, &value);
    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_EQUAL_HEX32(0x12345678, value);
    MODBUS_UDP_CloseConnection();
}

void test_MODBUS_UDP_Write(void)
{
    MODBUS_UDP_InitConnection();
    ModbusStatus_t status = MODBUS_UDP_Write("127.0.0.1", 502, 0x0001, 0x00AB);
    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    MODBUS_UDP_CloseConnection();
}

// --- UNITY MAIN RUNNER ---
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_MODBUS_CRC16);
    RUN_TEST(test_MODBUS_UDP_InitConnection);
    RUN_TEST(test_MODBUS_UDP_Read);
    RUN_TEST(test_MODBUS_UDP_Write);
    return UNITY_END();
}
