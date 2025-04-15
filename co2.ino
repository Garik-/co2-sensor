#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <SensirionI2cScd4x.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Org_01.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define I2C_SDA 5 // GPIO 5 (SDA)
#define I2C_SCL 6 // GPIO 6 (SCL)

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

SensirionI2cScd4x sensor;

void printUint64(uint64_t &value)
{
  Serial.print("0x");
  Serial.print((uint32_t)(value >> 32), HEX);
  Serial.print((uint32_t)(value & 0xFFFFFFFF), HEX);
}

int16_t init_sdc41()
{
  uint64_t serialNumber = 0;
  int16_t error = NO_ERROR;

  sensor.begin(Wire, SCD41_I2C_ADDR_62);
  delay(30);

  // Ensure sensor is in clean state
  error = sensor.wakeUp();
  if (error != NO_ERROR)
  {
    return error;
  }

  error = sensor.stopPeriodicMeasurement();
  if (error != NO_ERROR)
  {
    return error;
  }
  error = sensor.reinit();
  if (error != NO_ERROR)
  {
    return error;
  }
  // Read out information about the sensor
  error = sensor.getSerialNumber(serialNumber);
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
  error = sensor.startPeriodicMeasurement();
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

void drawProgress(uint16_t co2Value, uint16_t rangeMin, uint16_t rangeMax, int xPosition)
{
  const uint16_t BAR_WIDTH = 42;
  const uint16_t BAR_HEIGHT = 5;
  const uint16_t Y_POSITION = 59;

  // Рисуем пустой прямоугольник (контур)
  display.drawRect(xPosition, Y_POSITION, BAR_WIDTH, BAR_HEIGHT, SSD1306_WHITE);

  // Проверяем, находится ли значение CO2 в текущем диапазоне
  if (co2Value >= rangeMin && co2Value <= rangeMax)
  {
    // Вычисляем процент заполнения для текущего диапазона
    float percentage = (float)(co2Value - rangeMin) / (rangeMax - rangeMin);
    // Вычисляем ширину заполненной части
    uint16_t fillWidth = (uint16_t)(percentage * (BAR_WIDTH - 2));

    if (fillWidth > (BAR_WIDTH - 2)) {
      fillWidth = BAR_WIDTH - 2;
    }

    // Рисуем заполненный прямоугольник внутри контура
    display.fillRect(xPosition + 1, Y_POSITION + 1, fillWidth, BAR_HEIGHT - 2, SSD1306_WHITE);
  }
  else if (co2Value > rangeMax)
  {
    // Если значение превышает максимум диапазона, заполняем полностью
    display.fillRect(xPosition + 1, Y_POSITION + 1, BAR_WIDTH - 2, BAR_HEIGHT - 2, SSD1306_WHITE);
  }
  // Если значение меньше минимума диапазона, оставляем пустым
}

void draw(uint16_t co2Concentration, float temperature, float relativeHumidity)
{
  display.clearDisplay();

  display.setTextSize(4);
  display.setCursor(5, 40);
  display.print(co2Concentration);

  display.setTextSize(2);
  display.setCursor(90, 22);
  display.print(int(temperature)); // temperature
  display.print(F("."));

  display.setCursor(90, 52);
  display.print((int)relativeHumidity); // relativeHumidity

  display.setTextSize(1);
  display.setCursor(118, 14);
  display.print(F("C"));

  display.setCursor(118, 23);
  display.print(((int)(temperature * 10)) % 10); // temperatue first

  display.setCursor(118, 53);
  display.print(F("%"));

  display.setCursor(5, 14);
  display.print(F("co2 ppm"));

  drawProgress(co2Concentration, 0, 800, 0);
  drawProgress(co2Concentration, 800, 1200, 43);
  drawProgress(co2Concentration, 1200, 1500, 86);

  display.display();
}

void setup()
{
  char errorMessage[64];
  int16_t error = NO_ERROR;

  Serial.begin(115200);
  delay(10);

  Wire.begin(I2C_SDA, I2C_SCL);

  error = init_sdc41();
  if (error != NO_ERROR)
  {
    Serial.print("Error initializing SCD41: ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    for (;;)
      ; // Don't proceed, loop forever
  }

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  display.setTextColor(SSD1306_WHITE);
  display.setFont(&Org_01);
  display.setTextWrap(false);

  draw(0, 0.0, 0.0);
}

void loop()
{
  char errorMessage[64];
  int16_t error = NO_ERROR;

  bool dataReady = false;
  uint16_t co2Concentration = 0;
  float temperature = 0.0;
  float relativeHumidity = 0.0;
  //
  // Slow down the sampling to 0.2Hz.
  //
  delay(5000);
  error = sensor.getDataReadyStatus(dataReady);
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute getDataReadyStatus(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  while (!dataReady)
  {
    delay(100);
    error = sensor.getDataReadyStatus(dataReady);
    if (error != NO_ERROR)
    {
      Serial.print("Error trying to execute getDataReadyStatus(): ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      return;
    }
  }
  //
  // If ambient pressure compenstation during measurement
  // is required, you should call the respective functions here.
  // Check out the header file for the function definition.
  error =
      sensor.readMeasurement(co2Concentration, temperature, relativeHumidity);
  if (error != NO_ERROR)
  {
    Serial.print("Error trying to execute readMeasurement(): ");
    errorToString(error, errorMessage, sizeof errorMessage);
    Serial.println(errorMessage);
    return;
  }
  //
  // Print results in physical units.
  Serial.print("CO2 concentration [ppm]: ");
  Serial.print(co2Concentration);
  Serial.println();
  Serial.print("Temperature [°C]: ");
  Serial.print(temperature);
  Serial.println();
  Serial.print("Relative Humidity [RH]: ");
  Serial.print(relativeHumidity);
  Serial.println();

  draw(co2Concentration, temperature, relativeHumidity);
}
