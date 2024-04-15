#define BLYNK_TEMPLATE_ID "TMPL6hEYNFbtD"
#define BLYNK_TEMPLATE_NAME "Air alert"
#define BLYNK_AUTH_TOKEN "4uiAfWhDnbJRAPz__71gGThmSYAkzYYU"
#include <Wire.h>
#include <SSD1306Wire.h>
#include "SharpGP2Y10.h"
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#define BUZZER_PIN 12

// Initialize OLED display
SSD1306Wire display(0x3c, SDA, SCL);
// Define sensor pins
const int AOUTpin = 39;
const int DOUTpin = 14;

// Define pins and objects for air quality sensors
int PM25_SENSOR_PIN = 32;
int ledPin = 27;
float dustDensity = 0;
SharpGP2Y10 dustSensor(PM25_SENSOR_PIN, ledPin);

// WiFi credentials
const char* ssid = "Giangvien";
const char* password = "dhbk@2023";
char auth[] = BLYNK_AUTH_TOKEN;
bool alerted = false;
unsigned long lastAlertTime = 0;
unsigned long alertInterval = 2000; // Time to turn off the buzzer after activation (milliseconds)

void setup() {
  Serial.begin(9600);

  // Connect to Blynk
  Blynk.begin(auth, ssid, password);

  // Set pin modes
  pinMode(DOUTpin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialize display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.clear();
  display.display();
}

void loop() {
  Blynk.run();

  // Read sensor data
  dustDensity = dustSensor.getDustDensity();
  int CO_value = analogRead(AOUTpin);
  float CO_concentration = map(CO_value, 0, 4095, 0, 100);

  // Calculate Air Quality Index (AQI) for PM2.5 and CO
  int PM25_AQI = calculatePM25AQI(dustDensity);
  int CO_AQI = calculateCOAQI(CO_concentration);

  // Get status based on AQI
  String PM25_status = getAQIStatus(PM25_AQI);
  String CO_status = getAQIStatus(CO_AQI);

  // Display sensor data on OLED
  display.clear();
  display.drawString(0, 0, "PM2.5: " + String(dustDensity) + " ug/m3");
  display.drawString(0, 15, "PM2.5 Status: " + PM25_status);
  display.drawString(0, 30, "CO: " + String(CO_concentration) + " ppm");
  display.drawString(0, 45, "CO Status: " + CO_status);
  display.display();

  // Send sensor data to Blynk
  Blynk.virtualWrite(V0, CO_concentration);
  Blynk.virtualWrite(V1, dustDensity);
  Blynk.virtualWrite(V2, CO_concentration);
  Blynk.virtualWrite(V3, dustDensity);
  
  // Check for alert conditions and activate buzzer if necessary
  if ((CO_concentration > 10 || dustDensity > 35.4) && !alerted) {
    if (millis() - lastAlertTime > alertInterval) {
      alerted = true;
      // Log alert to Blynk
      Blynk.logEvent("air_alert", "Air in the house is unhealthy");
      digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer for alert
      lastAlertTime = millis();
    }
  } else if ((CO_concentration <= 10 && dustDensity <= 35.4) && alerted) {
    alerted = false;
    digitalWrite(BUZZER_PIN, LOW); // Turn off buzzer when not in alert state
  }

  // Delay for 1 second
  delay(1000);
}

// Function to calculate PM2.5 AQI
int calculatePM25AQI(float pm25Concentration) {
  int AQI = 0;

  if (pm25Concentration >= 0 && pm25Concentration <= 12.0) {
    AQI = map(pm25Concentration, 0, 12, 0, 50);
  } else if (pm25Concentration > 12.0 && pm25Concentration <= 35.4) {
    AQI = map(pm25Concentration, 12.1, 35.4, 51, 100);
  } else if (pm25Concentration > 35.4 && pm25Concentration <= 55.4) {
    AQI = map(pm25Concentration, 35.5, 55.4, 101, 150);
  } else if (pm25Concentration > 55.4 && pm25Concentration <= 150.4) {
    AQI = map(pm25Concentration, 55.5, 150.4, 151, 200);
  } else if (pm25Concentration > 150.4 && pm25Concentration <= 250.4) {
    AQI = map(pm25Concentration, 150.5, 250.4, 201, 300);
  } else if (pm25Concentration > 250.4 && pm25Concentration <= 350.4) {
    AQI = map(pm25Concentration, 250.5, 350.4, 301, 400);
  } else if (pm25Concentration > 350.4 && pm25Concentration <= 500.4) {
    AQI = map(pm25Concentration, 350.5, 500.4, 401, 500);
  } else {
    AQI = 500; // Maximum AQI value for PM2.5
  }

  return AQI;
}

// Function to calculate CO AQI
int calculateCOAQI(float CO_concentration) {
  int AQI = 0;

  if (CO_concentration >= 0 && CO_concentration <= 4.4) {
    AQI = map(CO_concentration, 0, 4.4, 0, 50);
  } else if (CO_concentration > 4.4 && CO_concentration <= 9.4) {
    AQI = map(CO_concentration, 4.5, 9.4, 51, 100);
  } else if (CO_concentration > 9.4 && CO_concentration <= 12.4) {
    AQI = map(CO_concentration, 9.5, 12.4, 101, 150);
  } else if (CO_concentration > 12.4 && CO_concentration <= 15.4) {
    AQI = map(CO_concentration, 12.5, 15.4, 151, 200);
  } else if (CO_concentration > 15.4 && CO_concentration <= 30.4) {
    AQI = map(CO_concentration, 15.5, 30.4, 201, 300);
  } else if (CO_concentration > 30.4 && CO_concentration <= 40.4) {
    AQI = map(CO_concentration, 30.5, 40.4, 301, 400);
  } else if (CO_concentration > 40.4 && CO_concentration <= 50.4) {
    AQI = map(CO_concentration, 40.5, 50.4, 401, 500);
  } else {
    AQI = 500; // Maximum AQI value for CO
  }

  return AQI;
}

// Function to get AQI status
String getAQIStatus(int AQI) {
    if (AQI >= 0 && AQI <= 50) {
        return "Good";
    } else if (AQI > 50 && AQI <= 100) {
        return "Moderate";
    } else if (AQI > 100 && AQI <= 150) {
        return "Unhealthy for Sensitive Groups";
    } else if (AQI > 150 && AQI <= 200) {
        return "Unhealthy";
    } else if (AQI > 200 && AQI <= 300) {
        return "Very Unhealthy";
    } else {
        return "Hazardous";
    }
}
