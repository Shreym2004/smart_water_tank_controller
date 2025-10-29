# 💧 Smart Water Tank Controller (ESP32)

This project automates water tank monitoring using an **ESP32**, **ultrasonic sensor**, **relay**, **buzzer**, and **OLED display**.  
It also includes a web dashboard to monitor the water level and control the pump remotely.

---

## 🚀 Features
- Real-time water level monitoring (HC-SR04 ultrasonic sensor)
- Automatic pump control based on tank level
- Manual control via button or web interface
- OLED display status updates
- Buzzer alerts for full and empty tank
- Simple responsive web dashboard (auto-refresh)

---

## 🧠 Hardware Required
| Component | Description |
|------------|-------------|
| ESP32 Dev Board | Main controller |
| HC-SR04 | Ultrasonic distance sensor |
| 5V Relay Module | Pump control |
| OLED 128x64 (I2C) | Display tank status |
| Buzzer | Audio alerts |
| Push Button | Manual pump control |
| LED | Pump indicator |

---

## ⚙️ Pin Connections
| Component | ESP32 Pin |
|------------|-----------|
| Trigger (HC-SR04) | GPIO 27 |
| Echo (HC-SR04) | GPIO 26 |
| Relay | GPIO 25 |
| LED | GPIO 2 |
| Button | GPIO 12 |
| Buzzer | GPIO 13 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |

---

## 🔧 Setup

1. Install required libraries in Arduino IDE:
   - **WiFi.h** (ESP32 core)
   - **WebServer.h** (ESP32 core)
   - **Adafruit GFX Library**
   - **Adafruit SSD1306**

2. Open the `Smart_Tank_Controller.ino` file.

3. Edit your WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_SSID";
   const char* password = "YOUR_PASSWORD";
4. Upload the code to your ESP32.

5. Open Serial Monitor → copy the IP address → open it in your browser.
