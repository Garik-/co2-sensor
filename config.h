#pragma once

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

namespace Config
{
    // I2C пины
    constexpr uint8_t I2C_SDA = 5;
    constexpr uint8_t I2C_SCL = 6;

    // Параметры дисплея
    constexpr uint8_t SCREEN_ADDRESS = 0x3C;
    constexpr uint8_t SCREEN_WIDTH = 128;
    constexpr uint8_t SCREEN_HEIGHT = 64;
    constexpr int8_t OLED_RESET = -1;

    // Интервалы
    constexpr uint16_t MEASUREMENT_INTERVAL = 5000;   // ms
    constexpr uint16_t DISPLAY_CHECK_INTERVAL = 1000; // ms

    // Диапазоны CO2
    namespace CO2
    {
        constexpr uint16_t RANGE1_MAX = 800;
        constexpr uint16_t RANGE2_MAX = 1200;
        constexpr uint16_t RANGE3_MAX = 1500;
    }

    // RTOS configuration
    namespace RTOS
    {
        constexpr uint16_t STACK_SIZE = 3072;
        constexpr uint8_t SENSOR_PRIORITY = 2;
        constexpr uint8_t DISPLAY_PRIORITY = 1;
    }
}