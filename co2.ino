#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Org_01.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include "config.h"

static const unsigned char PROGMEM image_co2__2__bits[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x30, 0x03, 0xfc, 0x1f, 0xe0, 0x00, 0x03, 0xfc, 0x30, 0x03, 0xfe, 0x3f, 0xf0, 0x00, 0x03, 0xfc, 0x30, 0x07, 0x06, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x06, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x00, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x00, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x00, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x00, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x00, 0x30, 0x30, 0x00, 0x03, 0xfc, 0x30, 0x06, 0x06, 0x30, 0x31, 0xf0, 0x03, 0xfc, 0x30, 0x07, 0x0e, 0x38, 0x73, 0xf8, 0x03, 0xfc, 0x30, 0x03, 0xfe, 0x1f, 0xe3, 0x18, 0x03, 0xfc, 0x30, 0x01, 0xf8, 0x0f, 0xc0, 0x18, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0x38, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0x70, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x03, 0xfc, 0x30, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x03, 0xf8, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Глобальные объекты
Adafruit_SSD1306 display(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, &Wire, Config::OLED_RESET);
SensirionI2cScd4x scd4x;

// FreeRTOS handles
QueueHandle_t sensorDataQueue;
TaskHandle_t sensorTaskHandle;
TaskHandle_t displayTaskHandle;

// Структура данных сенсора
struct SensorData
{
  uint16_t co2;
  float temperature;
  float humidity;
};

void drawProgress(uint16_t co2Value, uint16_t rangeMin, uint16_t rangeMax, int16_t xPosition)
{
  const int16_t BAR_WIDTH = 42;
  const int16_t BAR_HEIGHT = 5;
  const int16_t Y_POSITION = 59;

  display.drawRect(xPosition, Y_POSITION, BAR_WIDTH, BAR_HEIGHT, SSD1306_WHITE);

  if (co2Value >= rangeMin && co2Value <= rangeMax)
  {
    float percentage = (float)(co2Value - rangeMin) / (rangeMax - rangeMin);
    int16_t fillWidth = (int16_t)(percentage * (BAR_WIDTH - 2));

    if (fillWidth > (BAR_WIDTH - 2))
    {
      fillWidth = BAR_WIDTH - 2;
    }
    display.fillRect(xPosition + 1, Y_POSITION + 1, fillWidth, BAR_HEIGHT - 2, SSD1306_WHITE);
  }
  else if (co2Value > rangeMax)
  {
    display.fillRect(xPosition + 1, Y_POSITION + 1, BAR_WIDTH - 2, BAR_HEIGHT - 2, SSD1306_WHITE);
  }
}

void draw(const SensorData &data)
{
  Serial.print("CO2 concentration [ppm]: ");
  Serial.print(data.co2);
  Serial.println();
  Serial.print("Temperature [°C]: ");
  Serial.print(data.temperature);
  Serial.println();
  Serial.print("Relative Humidity [RH]: ");
  Serial.print(data.humidity);
  Serial.println();

  display.clearDisplay();

  // CO2 value
  display.setTextSize(4);
  display.setCursor(5, 40);
  display.print(data.co2);

  // Temperature
  display.setTextSize(2);
  display.setCursor(90, 22);
  display.print(int(data.temperature));
  display.print(F("."));

  // Humidity
  display.setCursor(90, 52);
  display.print((int)data.humidity);

  // Units and labels
  display.setTextSize(1);

  // Temperature unit and decimal
  display.setCursor(118, 14);
  display.print(F("C"));
  display.setCursor(118, 23);
  display.print(((int)(data.temperature * 10)) % 10);

  // Humidity unit
  display.setCursor(118, 53);
  display.print(F("%"));

  // CO2 label
  display.setCursor(5, 14);
  display.print(F("co2 ppm"));

  // Progress bars
  drawProgress(data.co2, 0, Config::CO2::RANGE1_MAX, 0);
  drawProgress(data.co2, Config::CO2::RANGE1_MAX, Config::CO2::RANGE2_MAX, 43);
  drawProgress(data.co2, Config::CO2::RANGE2_MAX, Config::CO2::RANGE3_MAX, 86);

  display.display();
}

void printUint64(uint64_t &value)
{
  Serial.print("0x");
  Serial.print((uint32_t)(value >> 32), HEX);
  Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}

bool initializeDisplay()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, Config::SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    return false;
  }

  display.setTextColor(SSD1306_WHITE);
  display.setFont(&Org_01);
  display.setTextWrap(false);

  display.clearDisplay();
  display.drawBitmap(32, 0, image_co2__2__bits, 64, 64, 1);
  display.display();

  return true;
}

int16_t initializeSensor()
{
  uint64_t serialNumber = 0;
  int16_t error = NO_ERROR;

  scd4x.begin(Wire, SCD41_I2C_ADDR_62);
  delay(30);

  // Ensure sensor is in clean state
  error = scd4x.wakeUp();
  if (error != NO_ERROR)
  {
    return error;
  }

  error = scd4x.stopPeriodicMeasurement();
  if (error != NO_ERROR)
  {
    return error;
  }
  error = scd4x.reinit();
  if (error != NO_ERROR)
  {
    return error;
  }
  // Read out information about the sensor
  error = scd4x.getSerialNumber(serialNumber);
  if (error != NO_ERROR)
  {
    return error;
  }
  Serial.print("serial number: ");
  printUint64(serialNumber);
  Serial.println();
  //
  // If temperature offset and/or sensor altitude compensation
  // is required, you should call the respective functions here.
  // Check out the header file for the function definitions.
  // Start periodic measurements (5sec interval)
  error = scd4x.startPeriodicMeasurement();
  if (error != NO_ERROR)
  {
    return error;
  }
  //
  // If low-power mode is required, switch to the low power
  // measurement function instead of the standard measurement
  // function above. Check out the header file for the definition.
  // For SCD41, you can also check out the single shot measurement example.
  //
  return NO_ERROR;
}

void sensorTask(void *parameter)
{
  int16_t error;
  SensorData data = {0, 0.0, 0.0};
  bool dataReady;
  char errorMessage[64];
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true)
  {
    dataReady = false;
    error = scd4x.getDataReadyStatus(dataReady);
    if (error != NO_ERROR)
    {
      Serial.print("Error trying to execute getDataReadyStatus(): ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      // Если ошибка связи с датчиком - просто ждем следующей итерации
      vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Config::MEASUREMENT_INTERVAL));
      continue;
    }

    if (dataReady)
    {
      error = scd4x.readMeasurement(data.co2, data.temperature, data.humidity);
      if (error != NO_ERROR)
      {
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.print("Error reading measurement: ");
        Serial.println(errorMessage);
        // При ошибке чтения оставляем предыдущие значения
      }
      else
      {
        // Данные успешно прочитаны - отправляем в очередь
        xQueueOverwrite(sensorDataQueue, &data);
      }
    }

    // Используем vTaskDelayUntil для точного тайминга
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(dataReady ? Config::MEASUREMENT_INTERVAL : 100));
  }
}

void displayTask(void *parameter)
{
  SensorData data;
  SensorData lastDisplayedData = {0, 0.0, 0.0};
  TickType_t xLastWakeTime = xTaskGetTickCount();

  while (true)
  {
    if (xQueueReceive(sensorDataQueue, &data, 0) == pdTRUE)
    {
      if (memcmp(&data, &lastDisplayedData, sizeof(SensorData)) != 0)
      {
        draw(data);
        lastDisplayedData = data;
      }
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Config::DISPLAY_CHECK_INTERVAL));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  Wire.begin(Config::I2C_SDA, Config::I2C_SCL);

  sensorDataQueue = xQueueCreate(1, sizeof(SensorData));
  if (!sensorDataQueue)
  {
    Serial.println("Failed to create queue");
    for (;;)
      ;
  }

  if (!initializeDisplay())
  {
    Serial.println("Display initialization failed");
    for (;;)
      ;
  }

  if (initializeSensor() != NO_ERROR)
  {
    Serial.println("Sensor initialization failed");
    for (;;)
      ;
  }

  xTaskCreate(
      sensorTask,
      "SensorTask",
      Config::RTOS::STACK_SIZE,
      NULL,
      Config::RTOS::SENSOR_PRIORITY,
      &sensorTaskHandle);

  xTaskCreate(
      displayTask,
      "DisplayTask",
      Config::RTOS::STACK_SIZE,
      NULL,
      Config::RTOS::DISPLAY_PRIORITY,
      &displayTaskHandle);
}

void loop()
{
  vTaskDelete(NULL);
}