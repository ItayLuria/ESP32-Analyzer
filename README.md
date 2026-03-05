# ESP32 Network & System Analyzer (HID Bypass Tool)

An ESP32-based HID tool with an OLED interface. This device emulates a Bluetooth keyboard to interact with a target Windows machine, performing automated diagnostics and stress testing.

---

## Hardware Configuration
To build this device, connect the components according to the following pinout:

| Component | ESP32 Pin | Details |
|-----------|-----------|---------|
| **OLED SDA** | GPIO 21 | I2C Data |
| **OLED SCL** | GPIO 22 | I2C Clock |
| **Button UP** | GPIO 19 | Internal Pull-up |
| **Button DOWN**| GPIO 18 | Internal Pull-up |
| **Button SEL** | GPIO 17 | Select/Execute |



---

##  Features & Modes

The device features a menu interface with three primary operation modes:

### 1. Network Report 
Collects network information and exfiltrates it to a Discord Webhook. It also scans the ARP cache to identify nearby device signatures.
- **Data collected:** SSID, Internal IPv4, External IP (via ipify API), Default Gateway, and DNS servers.

### 2. Hardware Report 
Performs a full system audit using Windows WMI (Windows Management Instrumentation).
- **Details:** Full CPU model, GPU name, exact RAM capacity, Motherboard manufacturer, and a list of all physical drives with their sizes.
- **System Info:** OS Caption, Build number, BIOS Serial Number, and System Uptime.

### 3. System Crash (Stress Test) 
Demonstrates system vulnerability to "Fork Bomb" attacks.
- **Mechanism:** Creates and executes a temporary `.bat` file that recursively spawns `msg` boxes and `notepad` instances.
- **Result:** Rapidly consumes CPU cycles and RAM, leading to a controlled system hang.

---

##  Demonstration

### Discord Integration
When a report is triggered, the data is formatted in Markdown and sent instantly:

<img width="1920" alt="Discord Webhook Report" src="https://github.com/user-attachments/assets/f453ef0a-1634-44ea-bf84-40883fc23f28" style="border-radius: 10px;" />

### System Crash in Action
Check out the video below to see the "Fork Bomb" execution:

<video src="https://github.com/user-attachments/assets/843e8618-cda3-4d05-b42a-90263d1faf64" controls="controls" style="max-width: 100%; border-radius: 10px;">
  Your browser does not support the video tag.
</video>

---

##  Setup Instructions
1. **Libraries:** Install `ESP32-BLE-Keyboard`, `Adafruit_SSD1306`, and `Adafruit_GFX` via Arduino IDE.
2. **Webhook:** Open `ESP-Analyzer.ino` and replace `[your-discord-webhook]` with your actual Discord Webhook URL.
3. **Flash:** Upload the code to your ESP32.
4. **Connect:** On the target Windows PC, go to Bluetooth settings and pair with **"ESP Analyzer"**.
5. **Execute:** Use the physical buttons to navigate and the 'SEL' button to start a task.

---

##  Disclaimer
This tool is developed for **educational purposes and authorized security auditing only**. The author is not responsible for any misuse or damage caused by this device. Always obtain explicit permission before testing on any system.
