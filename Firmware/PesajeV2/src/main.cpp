#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "HX711.h"

// --- CONFIG WiFi ---
const char* ssid     = "IoT";
const char* password = "Gt,N%2$R";

// --- SERVIDOR SMARTREHAB ---
const char* serverURL = "https://smartrehabbar-demo-7f620514b4ed.herokuapp.com";

// --- CONTROL DE ENVÍOS ---
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 100;
float lastWeightLeft = -1;
const float MIN_WEIGHT_CHANGE = 0.5;

// --- HX711 CONFIG ---
#define DOUT 19
#define SCK  18
HX711 scale;
long R0 = 0;
double SCALE = 24867;
int SIGN = -1;

// --- BATERÍA (ADC) ---
#define PIN_BATTERY_ADC 32
const float ADC_REF = 3.3;
const int ADC_MAX = 4095;
const float BAT_DIVIDER = 3.2;
const float CAL_FACTOR = 1.19;   // 🔥 calibración real medida

unsigned long lastBatteryRead = 0;
const unsigned long BATTERY_INTERVAL = 2000;

float lastBatteryVoltage = 0.0;
int batteryPercent = 0;

// --- FUNCIONES ---
long readRawAvg(int n = 10) {
  return scale.read_average(n);
}

void tareNow() {
  R0 = readRawAvg(30);
  Serial.print("TARE R0 = ");
  Serial.println(R0);
}

float leerSensorIzquierdo() {
  long r = readRawAvg(10);
  float kg = SIGN * ((double)(r - R0) / SCALE);
  return kg;
}

// 🔥 FUNCIÓN MEJORADA DE BATERÍA
void actualizarBateria() {

  // 🔹 Media de lecturas para estabilidad
  int adc = 0;
  for (int i = 0; i < 10; i++) {
    adc += analogRead(PIN_BATTERY_ADC);
    delay(2);
  }
  adc /= 10;

  float vOut = (adc / (float)ADC_MAX) * ADC_REF;

  // 🔥 Aplicamos calibración
  lastBatteryVoltage = vOut * BAT_DIVIDER * CAL_FACTOR;

  batteryPercent = constrain(
    100.0 * (lastBatteryVoltage - 3.20) / (4.20 - 3.20),
    0,
    100
  );

  // 🔍 PRINTS DE DIAGNÓSTICO
  Serial.println("----- 🔋 DIAGNÓSTICO BATERÍA -----");
  
  Serial.print("ADC raw (avg): ");
  Serial.println(adc);

  Serial.print("Voltaje en pin (vOut): ");
  Serial.print(vOut, 4);
  Serial.println(" V");

  Serial.print("Voltaje batería (calibrado): ");
  Serial.print(lastBatteryVoltage, 4);
  Serial.println(" V");

  Serial.print("Porcentaje: ");
  Serial.print(batteryPercent);
  Serial.println(" %");

  Serial.println("----------------------------------");
}

void sendMeasurementLeft(float weight, int batteryPct) {
  unsigned long now = millis();

  if (now - lastSendTime < SEND_INTERVAL) return;
  if (lastWeightLeft != -1 && abs(weight - lastWeightLeft) < MIN_WEIGHT_CHANGE) return;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi desconectado");
    return;
  }

  HTTPClient http;
  String url = String(serverURL) + "/api/hardware/left?peso=" + String(weight, 1) + "&bat=" + String(batteryPct);

  Serial.println("→ URL: " + url);

  http.begin(url);
  http.setTimeout(5000);
  int code = http.GET();

  if (code == 200) {
    Serial.print("✅ Enviado: ");
    Serial.print(weight, 1);
    Serial.print(" kg | ");
    Serial.print(batteryPct);
    Serial.println(" %");

    lastWeightLeft = weight;
    lastSendTime = now;
  } else {
    Serial.println("❌ HTTP error " + String(code));
  }

  http.end();
}

// --- SETUP ---
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n✅ WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  scale.begin(DOUT, SCK);
  delay(200);

  while (!scale.is_ready()) delay(10);

  tareNow();

  analogReadResolution(12);
  analogSetPinAttenuation(PIN_BATTERY_ADC, ADC_11db);
}

// --- LOOP ---
void loop() {
  float pesoIzq = leerSensorIzquierdo();

  if (millis() - lastBatteryRead > BATTERY_INTERVAL) {
    actualizarBateria();
    lastBatteryRead = millis();
  }

  Serial.print("Peso: ");
  Serial.print(pesoIzq, 2);
  Serial.print(" kg | Batería: ");
  Serial.print(lastBatteryVoltage, 2);
  Serial.print(" V (");
  Serial.print(batteryPercent);
  Serial.println(" %)");

  if (pesoIzq > 3.0) {
    sendMeasurementLeft(pesoIzq, batteryPercent);
  }

  delay(50);
}