#include "unity.h"
#include <stdio.h>

/* ---- Dummy Config Values ---- */
#define LCU_IP_ADDR "127.0.0.1"
#define LCU_PORT_UDP 502
#define REG_PAN_LIMIT_UPPER  0x0100
#define REG_PAN_LIMIT_LOWER  0x0101
#define REG_TILT_LIMIT_UPPER 0x0200
#define REG_TILT_LIMIT_LOWER 0x0201
#define PAN_LIMIT_UPPER_DEG  180.0f
#define PAN_LIMIT_LOWER_DEG  -180.0f
#define TILT_LIMIT_UPPER_DEG 90.0f
#define TILT_LIMIT_LOWER_DEG -90.0f

#include "limit_angle.h"

/* ---- Mock Modbus UDP Functions ---- */
static uint8_t mock_read_calls = 0;

ModbusStatus_t MODBUS_UDP_Read(const char *ip, uint16_t port,
                               uint16_t reg_addr, uint16_t num_regs,
                               uint32_t *value)
{
    (void)ip; (void)port; (void)num_regs;
    mock_read_calls++;

    // Return alternating dummy values for upper/lower limits
    switch (reg_addr)
    {
    case REG_PAN_LIMIT_UPPER:  *value = 1; break;
    case REG_PAN_LIMIT_LOWER:  *value = 0; break;
    case REG_TILT_LIMIT_UPPER: *value = 1; break;
    case REG_TILT_LIMIT_LOWER: *value = 0; break;
    default: return MODBUS_ERROR;
    }
    return MODBUS_OK;
}

ModbusStatus_t MODBUS_UDP_InitConnection(void)
{
    return MODBUS_OK; // Pretend connection succeeded
}

/* ---- UNITY setup / teardown ---- */
void setUp(void) { mock_read_calls = 0; }
void tearDown(void) {}

/* ---- TEST CASES ---- */
void test_LimitAngle_Init_should_return_OK(void)
{
    TEST_ASSERT_EQUAL(MODBUS_OK, LimitAngle_Init());
}

void test_LimitAngle_ReadPan_should_return_valid_values(void)
{
    LimitAngle_t limit;
    ModbusStatus_t status = LimitAngle_ReadPan(&limit);

    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_EQUAL_UINT8(1, limit.upper_state);
    TEST_ASSERT_EQUAL_UINT8(0, limit.lower_state);
    TEST_ASSERT_EQUAL_FLOAT(PAN_LIMIT_UPPER_DEG, limit.upper_angle_deg);
    TEST_ASSERT_EQUAL_FLOAT(PAN_LIMIT_LOWER_DEG, limit.lower_angle_deg);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(2, mock_read_calls);
}

void test_LimitAngle_ReadTilt_should_return_valid_values(void)
{
    LimitAngle_t limit;
    ModbusStatus_t status = LimitAngle_ReadTilt(&limit);

    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_EQUAL_UINT8(1, limit.upper_state);
    TEST_ASSERT_EQUAL_UINT8(0, limit.lower_state);
    TEST_ASSERT_EQUAL_FLOAT(TILT_LIMIT_UPPER_DEG, limit.upper_angle_deg);
    TEST_ASSERT_EQUAL_FLOAT(TILT_LIMIT_LOWER_DEG, limit.lower_angle_deg);
    TEST_ASSERT_GREATER_OR_EQUAL_UINT8(2, mock_read_calls);
}

void test_LimitAngle_ReadPan_should_return_ERROR_if_null(void)
{
    TEST_ASSERT_EQUAL(MODBUS_ERROR, LimitAngle_ReadPan(NULL));
}

void test_LimitAngle_ReadTilt_should_return_ERROR_if_null(void)
{
    TEST_ASSERT_EQUAL(MODBUS_ERROR, LimitAngle_ReadTilt(NULL));
}

/* ---- UNITY MAIN RUNNER ---- */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_LimitAngle_Init_should_return_OK);
    RUN_TEST(test_LimitAngle_ReadPan_should_return_valid_values);
    RUN_TEST(test_LimitAngle_ReadTilt_should_return_valid_values);
    RUN_TEST(test_LimitAngle_ReadPan_should_return_ERROR_if_null);
    RUN_TEST(test_LimitAngle_ReadTilt_should_return_ERROR_if_null);
    return UNITY_END();
}
