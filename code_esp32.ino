//descarga las librerias mencionadas
#include <Wire.h>
#include <MAX30105.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_I2C_ADDRESS 0x3C 
#define MODE_BUTTON_PIN 14  
//conectate a internet
const char* ssid = "---------";
const char* password = "-----------";

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MAX30105 particleSensor;

// Tamaño de la gráfica
#define GRAPH_WIDTH (SCREEN_WIDTH / 2 - 2)
#define GRAPH_HEIGHT (SCREEN_HEIGHT - 2)
#define DATA_POINTS 64  // Número de puntos en la gráfica

int irData[DATA_POINTS];
int dataIndex = 0;
bool useWiFi = false;  // Variable para controlar el uso de WiFi
int modeButtonState = 0;
int lastModeButtonState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long buttonPressTime = 0;
unsigned long longPressDuration = 1000;
unsigned long anteriorMillis = 0;

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
    while (1); 
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println(F("Inicializando..."));
  display.display();
  delay(2000);

  if (!particleSensor.begin()) {
    Serial.println("No se encontró el sensor MAX30102. Asegúrate de que esté conectado.");
    while (1);
  }

  particleSensor.setup();  // Configuración básica del sensor
  particleSensor.setPulseAmplitudeRed(0x0A);  // Ajusta la amplitud de la señal roja
  particleSensor.setPulseAmplitudeIR(0x0A);   // Ajusta la amplitud de la señal IR

  for (int i = 0; i < DATA_POINTS; i++) {
    irData[i] = 0;
  }

  // Verifica el estado inicial del botón
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

  long irValue = particleSensor.getIR();  // Lee el valor IR
  long redValue = particleSensor.getRed();  // Lee el valor de la luz roja

  // Mueve los datos hacia la izquierda
  for (int i = 1; i < DATA_POINTS; i++) {
    irData[i - 1] = irData[i];
  }

  // Añade el nuevo dato al final
  irData[DATA_POINTS - 1] = map(irValue, 0, 100000, 0, GRAPH_HEIGHT); 

  // Envia los datos al Serial Plotter
  Serial.print("IR:");
  Serial.print(irValue);
  Serial.print(",");
  Serial.print("Red:");
  Serial.println(redValue);

  // Muestra los datos en la pantalla OLED
  display.clearDisplay();  // Limpia la pantalla

  // Dibuja la gráfica IR en la mitad izquierda de la pantalla
  for (int i = 1; i < DATA_POINTS; i++) {
    display.drawLine(i - 1, SCREEN_HEIGHT - irData[i - 1] - 1, i, SCREEN_HEIGHT - irData[i] - 1, SH110X_WHITE);
  }
  display.setCursor(GRAPH_WIDTH + 2, 0);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.print(F("IR: "));
  display.println(irValue);
  display.print(F("Red: "));
  display.println(redValue);

  display.display();  //

  // Solo enviar datos si los valores IR y Red son mayores o iguales a 1000
  if (useWiFi && WiFi.status() == WL_CONNECTED && irValue >= 1000 && redValue >= 1000) {
    HTTPClient http;
    String postData = "id=esp32_01&ir=" + String(irValue) + "&red=" + String(redValue);

    Serial.print("postData : ");
    Serial.println(postData);

    http.begin("http://192.168.0.0/prueba/test/post.php");  // Cambia la URL según tu servidor
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

  delay(10);  //
}
