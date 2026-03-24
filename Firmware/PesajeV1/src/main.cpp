#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "HX711.h"

// -----------------------------
// CONFIG WiFi
// -----------------------------
const char* ssid     = "IoT";
const char* password = "Gt,N%2$R";

// -----------------------------
// SERVIDOR SMARTREHAB
// -----------------------------
const char* serverURL = "https://smartrehabbar-demo-7f620514b4ed.herokuapp.com";

// -----------------------------
// CONTROL DE ENVÍOS
// -----------------------------
unsigned long lastSendTime = 0;
const unsigned long SEND_INTERVAL = 100; // Máx 10 Hz

float lastWeightLeft = -1;
const float MIN_WEIGHT_CHANGE = 0.5; // kg

// -----------------------------
// HX711 CONFIG TUYA
// -----------------------------
#define DOUT 19
#define SCK  18

HX711 scale;
long R0 = 0;
double SCALE = 24867;  // <-- tu valor
int SIGN = -1;         // <-- el que funciona en tu caso

long readRawAvg(int n=10) {
  return scale.read_average(n);
}

void tareNow() {
  R0 = readRawAvg(30);
  Serial.print("TARE R0="); Serial.println(R0);
}

// -----------------------------
//  LECTURA REAL DEL SENSOR
// -----------------------------
float leerSensorIzquierdo() {
  long r = readRawAvg(10);
  float kg = SIGN * ( (double)(r - R0) / SCALE );
  return kg;
}

// -----------------------------
// ENVÍO AL SERVIDOR
// -----------------------------
void sendMeasurementLeft(float weight) {

  unsigned long now = millis();

  // 1. Frecuencia máx 10 Hz
  if (now - lastSendTime < SEND_INTERVAL) return;

  // 2. Cambio mínimo 0.5 kg
  if (lastWeightLeft != -1 && abs(weight - lastWeightLeft) < MIN_WEIGHT_CHANGE) return;

  // 3. WiFi OK?
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi desconectado");
    return;
  }

  HTTPClient http;
  String url = String(serverURL) + "/api/hardware/left?peso=" + String(weight, 1);
  http.begin(url);
  http.setTimeout(5000);

  int code = http.GET();
  if (code == 200) {
    Serial.println("✅ LEFT: " + String(weight, 1) + "kg → Enviado OK");
    lastWeightLeft = weight;
    lastSendTime = now;
  } 
  else if (code == 429) {
    Serial.println("⚠️ Filtrado por servidor");
  } 
  else {
    Serial.println("❌ Error " + String(code));
  }

  http.end();
}

// -----------------------------
// SETUP
// -----------------------------
void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\n✅ WiFi conectado");
  Serial.print("IP: "); Serial.println(WiFi.localIP());

  scale.begin(DOUT, SCK);
  delay(200);
  while (!scale.is_ready()) delay(10);

  tareNow();  // Tara inicial
}

// -----------------------------
// LOOP
// -----------------------------
void loop() {
  float pesoIzq = leerSensorIzquierdo();

  if (pesoIzq > 3.0) {     // No enviar si está vacío
    sendMeasurementLeft(pesoIzq);
  }

  delay(50); // 20 Hz lectura
}
