#  Smart Water Tank Controller (ESP32)

The **Smart Water Tank Controller** is an IoT-based automation system built using the **ESP32 microcontroller**. It continuously monitors the water level in a storage tank using an **ultrasonic sensor (HC-SR04)** and intelligently controls a **water pump** through a **relay module**. The system ensures the tank maintains an optimal water level — automatically turning the pump ON when water is low and OFF when the tank is full — preventing overflow and dry running.

A **0.96” OLED display (SSD1306)** provides real-time readings including water level percentage, measured distance, pump status, and operating mode (manual or automatic). A **buzzer** offers audible alerts when the tank is either full or nearly empty, while an **LED indicator** shows pump activity at a glance.

Users can manually override pump control using a **physical push button**, or operate the system remotely through an intuitive **web-based dashboard** hosted on the ESP32. This web interface, accessible from any smartphone or PC on the same Wi-Fi network, shows live tank information and allows toggling between manual and automatic modes. The dashboard automatically refreshes every few seconds to provide up-to-date readings.

This project is perfect for **home automation**, **smart irrigation**, and **water management systems**. By combining ESP32’s Wi-Fi capability with sensor-based control, it offers a cost-effective and efficient solution to manage water resources intelligently.

---

##  Key Features

- 🌊 Real-time water level monitoring using ultrasonic sensing  
- ⚡ Automatic pump control based on tank status  
- 🖥️ OLED display for live system updates  
- 🌐 Web dashboard for remote access and control  
- 🔘 Manual override via physical button  
- 🔔 Buzzer alerts for full or empty tank conditions  
- 💡 LED indicator for pump status  
- 📱 Mobile-friendly, auto-refreshing web interface  

---

##  Applications

- Home water tank monitoring and control  
- Smart irrigation systems for gardens or farms  
- Industrial water storage automation  
- Rainwater harvesting systems  

---

##  Built With

- **ESP32** — main controller with Wi-Fi support  
- **HC-SR04** — ultrasonic distance sensor for level measurement  
- **Relay Module** — to control the water pump  
- **SSD1306 OLED Display** — visual feedback and status updates  
- **Buzzer & LED** — alert and indicator system  
- **Push Button** — manual pump control  

---

💧 *A simple yet powerful automation project for smarter water management.*
