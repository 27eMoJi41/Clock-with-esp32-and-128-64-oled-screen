#include <WiFi.h>
#include "time.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define I2C_SDA 15
#define I2C_SCL 14
TwoWire I2Cbus = TwoWire(0);

const char* ssid = "********";//the network you will connect to
const char* password = "********";//networks password
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600 * 3;//The number you multiply by is the number in your time zone
const int   daylightOffset_sec = 3600 * 0;

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define SCREEN_ADDRESS  0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &I2Cbus, OLED_RESET);

void setup() {
  Serial.begin(115200);
  delay(1000);

  I2Cbus.begin(I2C_SDA, I2C_SCL, 100000);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.printf("SSD1306 OLED display failed to initalize.\nCheck that display SDA is connected to pin %d and SCL connected to pin %d\n", I2C_SDA, I2C_SCL);
    while (true);
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  display.clearDisplay();
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  
  display.clearDisplay();
  display.setCursor(20, 5);
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.print(&timeinfo,"%H:%M");
  display.setCursor(15, 32);
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.print(&timeinfo,"%B %d %Y");
  display.setCursor(28, 45);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print(&timeinfo,"%A");
  display.display();
  delay(1000);
}