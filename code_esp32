//descarga las librerias mencionadas
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "MAX30105.h"

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
#define MODE_BUTTON_PIN 14  // Botón para activar/desactivar WiFi

const char* ssid = "---------"; //escribe el nombre del internet al que se pueda conectar la ESP32
const char* password = "-------"; //escribe la contraseña de la red conectada

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MAX30105 particleSensor;

unsigned long anteriorMillis = 0;
int irValue = 0;
float graficaIR = 0;

int x[128]; 
int y[128];
bool useWiFi = false;  // Variable para controlar el uso de WiFi
int modeButtonState = 0;
int lastModeButtonState = 0;
unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50; 
unsigned long buttonPressTime = 0; 
unsigned long longPressDuration = 1000;

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println("-------------");
  Serial.print("Connecting");

  int connecting_process_timed_out = 400;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
    delay(100); 
  }

  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.println("-------------");
  delay(2000);
}

void setup() {                
  Serial.begin(115200);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLDOWN); 
  delay(100);  
  if (!display.begin(0x3C, OLED_RESET)) { 
    Serial.println(F("Fallo al inicializar la pantalla SH1106"));
    for(;;); 
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(F("Inicializando..."));
  display.display();
  delay(2000);

  if (!particleSensor.begin()) { 
    Serial.println("MAX30102 no encontrado. Por favor, revisa la conexión y la alimentación.");
    while (1);
  }
  
  byte ledBrightness = 0x1F; // Opciones: 0=Apagado a 255=50mA
  byte sampleAverage = 8; // Opciones: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; // Opciones: 1 = Solo rojo, 2 = Rojo + IR, 3 = Rojo + IR + Verde
  int sampleRate = 100; // Opciones: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; // Opciones: 69, 118, 215, 411
  int adcRange = 4096; // Opciones: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); 

  for(int i = 127; i >= 0; i--) {
    x[i] = 9999;
    y[i] = 9999;
  }


  int initialModeButtonState = digitalRead(MODE_BUTTON_PIN);

  if (initialModeButtonState == HIGH) {
    useWiFi = true;
    display.clearDisplay();
    display.setCursor(0, 30);
    display.print("Ejecutando modo");
    display.setCursor(0, 40);
    display.print("conexion...");
    display.display();
    delay(3000);
    setupWiFi();
  } else {
    useWiFi = false;
    display.clearDisplay();
    display.setCursor(0, 30);
    display.print("Ejecutando modo");
    display.setCursor(0, 40);
    display.print("sin conexion...");
    display.display();
    delay(3000);
  }
}

void loop() {
  int modeReading = digitalRead(MODE_BUTTON_PIN);

  if (modeReading != lastModeButtonState) {
    lastDebounceTime = millis();
    if (modeReading == HIGH) {
      buttonPressTime = millis();
    }
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (modeReading != modeButtonState) {
      modeButtonState = modeReading;
      if (modeButtonState == HIGH && (millis() - buttonPressTime) > longPressDuration) {
        useWiFi = !useWiFi;
        display.clearDisplay();
        display.setCursor(0, 30);
        display.print("WiFi ");
        if (useWiFi) {
          display.print("Activado");
          setupWiFi();
        } else {
          display.print("Desactivado");
          WiFi.disconnect();
        }
        display.display();
        delay(2000);
      }
    }
  }

  lastModeButtonState = modeReading;

  display.clearDisplay(); 

  irValue = particleSensor.getIR();
    
  if (irValue < 1000) {
    display.clearDisplay();
    display.setCursor(0, 20);
    display.print("Coloque el dedo");
    display.setCursor(0, 30);
    display.print("en el sensor");
    display.setCursor(0, 40);
    display.print("MAX30105");
  } else {
    graficaIR = map(irValue, 80000, 100000, 53, 0); 

    x[127] = graficaIR; 

    for(int i = 127; i > 0; i--) { 
      display.drawPixel(i, x[i], SH110X_WHITE); 
      y[i-1] = x[i]; 
    }

    Serial.print("IR: ");
    Serial.println(irValue);

    if (useWiFi && WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String postData = "id=esp32_01&sangre=" + String(irValue); 

      Serial.print("postData : ");
      Serial.println(postData);

      http.begin("http://0.0.0.0/prueba/test/post.php"); // escribir direccion IP de tu computador, la ruta y el nombre del archivo para el envio de datos el archivo se colocará en el siguiente archivo
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");

      int httpCode = http.POST(postData);
      String payload = http.getString();

      Serial.print("httpCode : ");
      Serial.println(httpCode);
      Serial.print("Response Payload: ");
      Serial.println(payload);

      http.end();
    }
    
    unsigned long tiempo = millis() - anteriorMillis;
    display.setCursor(25, 57); 
    display.print(tiempo);
    anteriorMillis = millis();
    display.print(F(" ms"));
  }

  display.display(); 
  for(int i = 127; i >= 0; i--) {
    x[i] = y[i]; 
  }

  delay(10);
}
