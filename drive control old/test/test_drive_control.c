#include "unity.h"
#include "drive_control.h"
#include <stdio.h>
#include <windows.h> // for Sleep mock (optional on Windows)

// ---- MOCKS ----
static int mock_write_call_count = 0;
static int mock_read_call_count = 0;
static uint32_t mock_pan_speed = 0;
static uint32_t mock_tilt_speed = 0;

ModbusStatus_t MODBUS_UDP_Write(const char *ip, uint16_t port,
                                uint16_t reg_addr, uint16_t reg_value)
{
    (void)ip; (void)port; (void)reg_addr; (void)reg_value;
    mock_write_call_count++;
    return MODBUS_OK;
}

ModbusStatus_t MODBUS_UDP_Read(const char *ip, uint16_t port,
                               uint16_t start_addr, uint16_t num_regs,
                               uint32_t *value)
{
    (void)ip; (void)port; (void)start_addr; (void)num_regs;
    mock_read_call_count++;

    // Simulate that speeds become 0 after 5 reads
    static int step = 0;
    if (step++ < 5)
        *value = 100U; // running
    else
        *value = 0U;   // stopped

    if (start_addr == REG_PAN_SPEED) mock_pan_speed = *value;
    if (start_addr == REG_TILT_SPEED) mock_tilt_speed = *value;

    return MODBUS_OK;
}

// ---- UNITY SETUP ----
void setUp(void)
{
    mock_write_call_count = 0;
    mock_read_call_count = 0;
    mock_pan_speed = mock_tilt_speed = 0;
}

void tearDown(void) {}

// ---- TEST CASES ----
void test_Send_Pan_Command(void)
{
    ModbusStatus_t status = Send_Pan_Command(0x0001);
    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_EQUAL(1, mock_write_call_count);
}

void test_Send_Tilt_Command(void)
{
    ModbusStatus_t status = Send_Tilt_Command(0x0002);
    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_EQUAL(1, mock_write_call_count);
}

void test_Drive_ParkMotion_completes_successfully(void)
{
    ModbusStatus_t status = Drive_ParkMotion();
    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_GREATER_OR_EQUAL(2, mock_write_call_count);
    TEST_ASSERT_GREATER_OR_EQUAL(5, mock_read_call_count);
}

// ---- UNITY MAIN ----
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Send_Pan_Command);
    RUN_TEST(test_Send_Tilt_Command);
    RUN_TEST(test_Drive_ParkMotion_completes_successfully);
    return UNITY_END();
}
