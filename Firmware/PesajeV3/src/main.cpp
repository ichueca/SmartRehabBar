#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "HX711.h"

// --- WIFI ---
const char* ssid     = "IoT";
const char* password = "Gt,N%2$R";

// --- SERVIDOR ---
const char* serverURL = "https://smartrehabbar-demo-7f620514b4ed.herokuapp.com";

// --- HX711 ---
#define DOUT_LEFT   4
#define DOUT_RIGHT  3
#define SCK         2

HX711 scaleLeft;
HX711 scaleRight;

long R0_left = 0;
long R0_right = 0;

double SCALE_left = 25268;
double SCALE_right = 24666;

int SIGN = -1;

// --- BATERÍA ---
#define PIN_BATTERY_ADC 1
const float ADC_REF = 3.3;
const int ADC_MAX = 4095;
const float BAT_DIVIDER = 3.2;

float batteryVoltage = 0;
int batteryPercent = 0;
unsigned long lastBatteryRead = 0;
const unsigned long BATTERY_INTERVAL = 2000;

// --- ENVÍO ---
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 150;

float lastWeightLeft = -1;
float lastWeightRight = -1;

const float MIN_WEIGHT_CHANGE = 0.5;

// --- FUNCIONES ---
long readRawAvg(HX711 &scale, int n = 10) {
  return scale.read_average(n);
}

void tareScales() {
  Serial.println("🔧 Tarando...");
  R0_left = readRawAvg(scaleLeft, 30);
  R0_right = readRawAvg(scaleRight, 30);
}

float leerPeso(HX711 &scale, long R0, double SCALE) {
  long r = readRawAvg(scale, 10);
  return SIGN * ((double)(r - R0) / SCALE);
}

void actualizarBateria() {
  int adc = analogRead(PIN_BATTERY_ADC);

  float vOut = (adc / (float)ADC_MAX) * ADC_REF;
  batteryVoltage = vOut * BAT_DIVIDER;

  batteryPercent = constrain(
    100.0 * (batteryVoltage - 3.2) / (4.2 - 3.2),
    0,
    100
  );
}

// --- ENVÍO IZQUIERDO ---
void sendLeft(float weight, int bat) {

  if (lastWeightLeft != -1 && abs(weight - lastWeightLeft) < MIN_WEIGHT_CHANGE) return;

  HTTPClient http;

  String url = String(serverURL) +
               "/api/hardware/left?peso=" + String(weight, 1) +
               "&bat=" + String(bat);

  Serial.println("→ LEFT: " + url);

  http.begin(url);
  int code = http.GET();

  if (code == 200) {
    lastWeightLeft = weight;
  } else {
    Serial.println("❌ LEFT error " + String(code));
  }

  http.end();
}

// --- ENVÍO DERECHO ---
void sendRight(float weight, int bat) {

  if (lastWeightRight != -1 && abs(weight - lastWeightRight) < MIN_WEIGHT_CHANGE) return;

  HTTPClient http;

  String url = String(serverURL) +
               "/api/hardware/right?peso=" + String(weight, 1) +
               "&bat=" + String(bat);

  Serial.println("→ RIGHT: " + url);

  http.begin(url);
  int code = http.GET();

  if (code == 200) {
    lastWeightRight = weight;
  } else {
    Serial.println("❌ RIGHT error " + String(code));
  }

  http.end();
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("🚀 SmartRehab V4");

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n✅ WiFi conectado");

  // HX711
  scaleLeft.begin(DOUT_LEFT, SCK);
  scaleRight.begin(DOUT_RIGHT, SCK);

  while (!scaleLeft.is_ready() || !scaleRight.is_ready()) {
    Serial.println("⏳ Esperando HX711...");
    delay(500);
  }

  tareScales();

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_BATTERY_ADC, ADC_11db);
}

// --- LOOP ---
void loop() {

  float pesoIzq = leerPeso(scaleLeft, R0_left, SCALE_left);
  float pesoDer = leerPeso(scaleRight, R0_right, SCALE_right);

  if (millis() - lastBatteryRead > BATTERY_INTERVAL) {
    actualizarBateria();
    lastBatteryRead = millis();
  }

  Serial.print("Izq: ");
  Serial.print(pesoIzq, 2);
  Serial.print(" | Der: ");
  Serial.print(pesoDer, 2);
  Serial.print(" | Bat: ");
  Serial.print(batteryPercent);
  Serial.println("%");

  if (pesoIzq > 3.0) {
    sendLeft(pesoIzq, batteryPercent);
  }

  if (pesoDer > 3.0) {
    sendRight(pesoDer, batteryPercent);
  }

  delay(50);
}