#include "serial_task.h"

static char cmdBuffer[MAX_CMD_LENGTH];
static uint8_t cmdIndex = 0;

void processCommand(const char *cmd)
{
    if (strncmp(cmd, "set_contrast ", 12) == 0)
    {
        const char *arg = cmd + 12;
        uint8_t contrast = (uint8_t)atoi(arg);

        if (arg[0] != '\0' && contrast >= 0 && contrast <= 255)
        {
            if (setSSD1306Contrast(contrast))
            {

                Serial.print("set_contrast: ");
                Serial.println(contrast);
            }
            else
            {
                Serial.println("ERROR: Failed to set contrast");
            }
        }
        else
        {
            Serial.println("ERROR: Invalid contrast (0-255)");
        }
    }
    else
    {
        Serial.println("ERROR: Unknown command");
    }
}

void serialTask(void *parameter)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); // Частый опрос (20 мс)

    memset(cmdBuffer, 0, sizeof(cmdBuffer));

    while (true)
    {
        // Чтение данных без блокировки
        while (Serial.available() && cmdIndex < MAX_CMD_LENGTH - 1)
        {
            char c = Serial.read();
            if (c == '\n' || c == '\r')
            {
                if (cmdIndex > 0)
                {
                    processCommand(cmdBuffer);
                    cmdIndex = 0;
                    memset(cmdBuffer, 0, sizeof(cmdBuffer));
                }
            }
            else if (isprint(c))
            { // Только печатные символы
                cmdBuffer[cmdIndex++] = c;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, xFrequency); // Точно соблюдаем период
    }
}
