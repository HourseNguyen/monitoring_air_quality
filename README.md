# monitoring_air_quality
## 🗺️ Overview
This project introduces an indoor air quality monitoring system specifically for CO gas and PM 2.5. The project utilizes a Node MCU ESP32 connected to a CO sensor, a PM 2.5 sensor, and peripherals to monitor and track indoor CO and PM2.5 concentrations, alerting users to real-time poor air quality conditions via Gmail and Blynk App. 
![VideoCapture_20231222-154150 (1)](https://github.com/Nguyenngoembedded/monitoring_air_quality/assets/135302509/6a52a9ad-3d19-4e26-97ae-b5a206b7494f)
## ❔ Problem 
Although the quality of outdoor air can give us some insight into environmental health, it doesn't fully represent the air we breathe indoors. Indoor air quality (IAQ) can be greatly influenced by multiple factors, even in adequately ventilated structures. This is where IAQ monitoring systems play a vital role. These systems monitor pollutants such as PM2.5 and CO inside our homes and buildings. By continuously tracking these levels, we can detect possible health hazards and implement measures to enhance indoor air quality, fostering a healthier and more pleasant indoor atmosphere. 
## 🗒️ Requirement 
- Low cost : Production cost should not exceed 1.000.000 VND
- Compact size : Product dimension: 15cm x 10cm x 5cm
- High Precision: Accurate result: Error mustnot exceed 10%
- Warning users about low air quality : 
- Low energy consumption : The capacity of the product is under 5W 
## 📦 Installation 
This project can be done on Arduino software
## 🪓 Material needed
+Node MCU ESP32    
+CO sensor(MQ-7)    
+PM2.5 (Sharp GP2Y1014AU0F)(include Capacitance : 220microfara and Resistance 150 ohm)    
+Buzzer     
+Oled 0.96 inch I2C 
## 🚀 Prototype schematic
![Circuit](https://github.com/HourseNguyen/monitoring_air_quality/assets/135302509/c35d3406-2e7b-4b94-a89d-fa0232eadddc)

