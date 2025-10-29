#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== WiFi Credentials =====
const char* ssid = "S20 fe";
const char* password = "shrey123";

// ===== Pin Definitions =====
#define TRIG_PIN   27
#define ECHO_PIN   26
#define RELAY_PIN  25
#define LED_PIN     2
#define BUTTON_PIN 12
#define BUZZER_PIN 13

// ===== OLED Settings =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ===== Tank Settings =====
const int EMPTY_TANK = 70;   // cm when tank is empty
const int FULL_TANK  = 30;   // cm when tank is full

// ===== Variables =====
float distanceCM = 0;
int waterLevel = 0;
bool pumpOn = false;
bool manualOverride = false;
unsigned long lastBeep = 0;

// ===== Web Server =====
WebServer server(80);

// ===== Measure Distance =====
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30 ms timeout
  return duration * 0.0343 / 2; // cm
}

// ===== Update Water Level =====
void updateWaterLevel() {
  distanceCM = getDistance();
  waterLevel = map((int)distanceCM, EMPTY_TANK, FULL_TANK, 0, 100);
  waterLevel = constrain(waterLevel, 0, 100);
}

// ===== Buzzer Alerts =====
void buzzerAlert() {
  if (waterLevel <= 10) { 
    // Continuous beep for empty
    digitalWrite(BUZZER_PIN, HIGH);
  } 
  else if (waterLevel >= 95) {
    // Alternate beep for full
    if (millis() - lastBeep >= 500) {
      digitalWrite(BUZZER_PIN, !digitalRead(BUZZER_PIN));
      lastBeep = millis();
    }
  } 
  else {
    digitalWrite(BUZZER_PIN, LOW);
  }
}

// ===== Pump Control Logic =====
void autoPumpControl() {
  if (!manualOverride) {
    if (waterLevel <= 10 && !pumpOn) {
      pumpOn = true;
      Serial.println("Auto: Water low → Pump ON");
    } 
    else if (waterLevel >= 95 && pumpOn) {
      pumpOn = false;
      Serial.println("Auto: Tank full → Pump OFF");
    }
  }

  digitalWrite(RELAY_PIN, pumpOn);
  digitalWrite(LED_PIN, pumpOn);
}

// ===== OLED Display =====
void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("💧 SMART TANK");

  display.setCursor(0, 15);
  display.print("Level: ");
  display.print(waterLevel);
  display.println("%");

  display.setCursor(0, 30);
  display.print("Dist: ");
  display.print(distanceCM, 1);
  display.println(" cm");

  display.setCursor(0, 45);
  display.print("Pump: ");
  display.println(pumpOn ? "ON" : "OFF");

  display.setCursor(70, 45);
  display.print(manualOverride ? "Manual" : "Auto");

  display.display();
}

// ===== Web Dashboard =====
String getHTML() {
  String html = "<html><head><meta name='viewport' content='width=device-width,initial-scale=1'>";
  html += "<title>Smart Tank</title>";
  html += "<style>body{font-family:Arial;text-align:center;background:#111;color:white;}";
  html += "button{font-size:20px;padding:10px 25px;margin:10px;background:#00bfff;border:none;border-radius:8px;color:white;cursor:pointer;}";
  html += "button:hover{background:#0099cc;}</style></head><body>";

  html += "<h1>💧 Smart Water Tank</h1>";
  html += "<h2>Level: " + String(waterLevel) + "%</h2>";
  html += "<h3>Distance: " + String(distanceCM, 1) + " cm</h3>";
  html += "<h3>Pump: " + String(pumpOn ? "ON ✅" : "OFF ❌") + "</h3>";
  html += "<h4>Mode: " + String(manualOverride ? "Manual" : "Auto") + "</h4>";

  html += "<form action='/toggle'><button>";
  html += pumpOn ? "STOP PUMP" : "START PUMP";
  html += "</button></form>";

  html += "<form action='/auto'><button>SET AUTO MODE</button></form>";

  html += "<meta http-equiv='refresh' content='3'>";
  html += "</body></html>";
  return html;
}

// ===== Web Routes =====
void handleRoot() {
  updateWaterLevel();
  autoPumpControl();
  buzzerAlert();
  updateDisplay();
  server.send(200, "text/html", getHTML());
}

void handleToggle() {
  manualOverride = true;
  pumpOn = !pumpOn;
  digitalWrite(RELAY_PIN, pumpOn);
  digitalWrite(LED_PIN, pumpOn);
  Serial.println(pumpOn ? "Manual: Pump ON" : "Manual: Pump OFF");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleAuto() {
  manualOverride = false;
  Serial.println("Switched to AUTO mode");
  server.sendHeader("Location", "/");
  server.send(303);
}

// ===== Manual Button =====
void checkButton() {
  static bool lastState = HIGH;
  bool state = digitalRead(BUTTON_PIN);
  if (state == LOW && lastState == HIGH) {
    manualOverride = true;
    pumpOn = !pumpOn;
    digitalWrite(RELAY_PIN, pumpOn);
    digitalWrite(LED_PIN, pumpOn);
    Serial.println(pumpOn ? "Manual Button: Pump ON" : "Manual Button: Pump OFF");
    delay(300);
  }
  lastState = state;
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Smart Tank Booting...");
    display.display();
    delay(1000);
  }

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/auto", handleAuto);
  server.begin();
  Serial.println("Web server started.");
}

// ===== Loop =====
void loop() {
  server.handleClient();
  checkButton();
  updateWaterLevel();
  autoPumpControl();
  buzzerAlert();
  updateDisplay();
  delay(500);
}
