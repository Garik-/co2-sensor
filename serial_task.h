#pragma once

#include <Arduino.h>

#define MAX_CMD_LENGTH 32
void serialTask(void *parameter);
bool setSSD1306Contrast(uint8_t contrast);