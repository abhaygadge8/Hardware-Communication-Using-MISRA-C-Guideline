#include "unity.h"
#include <stdio.h>

/* ---- DUMMY CONFIG CONSTANTS FOR TEST ---- */
#define ENCODER_RESOLUTION 4096
#define DRIVE_IP_ADDR "127.0.0.1"
#define DRIVE_PORT_UDP 502
#define REG_PAN_POSITION  0x0001
#define REG_PAN_SPEED     0x0002
#define REG_PAN_COUNTS    0x0003
#define REG_TILT_POSITION 0x0004
#define REG_TILT_SPEED    0x0005
#define REG_TILT_COUNTS   0x0006

#include "drive_feedback.h"


/* ================================
   MOCK FUNCTIONS for Modbus UDP
   ================================ */

static uint32_t dummy_value_counter = 0;

ModbusStatus_t MODBUS_UDP_Read(const char *ip, uint16_t port,
                               uint16_t start_addr, uint16_t num_regs,
                               uint32_t *value)
{
    (void)ip; (void)port; (void)start_addr; (void)num_regs;

    // Provide dummy incremental values for each register
    *value = 1000 + dummy_value_counter;
    dummy_value_counter += 100;
    return MODBUS_OK;
}

/* ================================
   UNITY SETUP / TEARDOWN
   ================================ */
void setUp(void)
{
    dummy_value_counter = 0;
}

void tearDown(void) {}

/* ================================
   TEST CASES
   ================================ */
void test_Read_Pan_Feedback_should_return_valid_data(void)
{
    AxisFeedback_t fb;
    ModbusStatus_t status = Read_Pan_Feedback(&fb);

    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 360.0f * fb.raw_position / (float)ENCODER_RESOLUTION, fb.position_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, (60.0f * fb.raw_speed) / (float)ENCODER_RESOLUTION, fb.speed_rpm);
}

void test_Read_Pan_Feedback_should_return_error_when_null(void)
{
    ModbusStatus_t status = Read_Pan_Feedback(NULL);
    TEST_ASSERT_EQUAL(MODBUS_ERROR, status);
}

void test_Read_Tilt_Feedback_should_return_valid_data(void)
{
    AxisFeedback_t fb;
    ModbusStatus_t status = Read_Tilt_Feedback(&fb);

    TEST_ASSERT_EQUAL(MODBUS_OK, status);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, 360.0f * fb.raw_position / (float)ENCODER_RESOLUTION, fb.position_deg);
    TEST_ASSERT_FLOAT_WITHIN(0.1f, (60.0f * fb.raw_speed) / (float)ENCODER_RESOLUTION, fb.speed_rpm);
}

void test_Read_Tilt_Feedback_should_return_error_when_null(void)
{
    ModbusStatus_t status = Read_Tilt_Feedback(NULL);
    TEST_ASSERT_EQUAL(MODBUS_ERROR, status);
}

/* ================================
   UNITY TEST RUNNER
   ================================ */
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Read_Pan_Feedback_should_return_valid_data);
    RUN_TEST(test_Read_Pan_Feedback_should_return_error_when_null);
    RUN_TEST(test_Read_Tilt_Feedback_should_return_valid_data);
    RUN_TEST(test_Read_Tilt_Feedback_should_return_error_when_null);
    return UNITY_END();
}
