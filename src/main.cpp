
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "DHT.h"


const char* ssid = "pedro";       // nome da sua rede Wi-Fi
const char* password = "brasil123"; // senha da rede


ESP8266WebServer server(80);

// LED interno do NodeMCU (geralmente GPIO2 = D4)
const int ledPin = D2;

// DHT11
#define DHTPIN D1  // Use D1 para o DHT11 (ajuste se necessário)
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float temperatura = 0.0;
float umidade = 0.0;

void handleRoot() {
  String html = "<html>"
  "<head>"
  "<meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Controle LED e DHT11</title>"
  "<style>"
  "body {"
  "  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"
  "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
  "  margin: 0;"
  "  padding: 0;"
  "  min-height: 100vh;"
  "  display: flex;"
  "  justify-content: center;"
  "  align-items: center;"
  "}"
  ".container {"
  "  background: rgba(255, 255, 255, 0.95);"
  "  padding: 40px;"
  "  border-radius: 20px;"
  "  box-shadow: 0 20px 60px rgba(0, 0, 0, 0.3);"
  "  text-align: center;"
  "  max-width: 400px;"
  "  width: 90%;"
  "}"
  "h1 {"
  "  color: #333;"
  "  margin-bottom: 30px;"
  "  font-size: 28px;"
  "}"
  ".btn {"
  "  font-size: 20px;"
  "  padding: 15px 40px;"
  "  margin: 10px;"
  "  border: none;"
  "  border-radius: 50px;"
  "  cursor: pointer;"
  "  transition: all 0.3s ease;"
  "  font-weight: bold;"
  "  text-decoration: none;"
  "  display: inline-block;"
  "  box-shadow: 0 4px 15px rgba(0, 0, 0, 0.2);"
  "}"
  ".btn-on {"
  "  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);"
  "  color: white;"
  "}"
  ".btn-on:hover {"
  "  transform: translateY(-3px);"
  "  box-shadow: 0 6px 20px rgba(102, 126, 234, 0.4);"
  "}"
  ".btn-off {"
  "  background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);"
  "  color: white;"
  "}"
  ".btn-off:hover {"
  "  transform: translateY(-3px);"
  "  box-shadow: 0 6px 20px rgba(245, 87, 108, 0.4);"
  "}"
  ".sensor {"
  "  margin-top: 30px;"
  "  font-size: 22px;"
  "  color: #444;"
  "}"
  "</style>"
  "</head>"
  "<body>"
  "<div class='container'>"
  "<h1>🔆 Controle do LED & DHT11</h1>"
  "<div class='sensor' id='sensor'>"
  "🌡️ Temperatura: -- °C<br>💧 Umidade: -- %"
  "</div>"
  "<p><button class='btn btn-on' onclick='ligar()'>⚡ Ligar</button></p>"
  "<p><button class='btn btn-off' onclick='desligar()'>🌙 Desligar</button></p>"
  "<script>"
  "function ligar() {"
  "  fetch('/on').then(r => console.log('Requisição enviada'));"
  "}"
  "function desligar() {"
  "  fetch('/off').then(r => console.log('Requisição enviada'));"
  "}"
  "function atualizarSensor() {"
  "  fetch('/sensor').then(resp => resp.json()).then(data => {"
  "    document.getElementById('sensor').innerHTML = '🌡️ Temperatura: ' + data.temperatura.toFixed(1) + ' °C<br>💧 Umidade: ' + data.umidade.toFixed(1) + ' %';"
  "  });"
  "}"
  "setInterval(atualizarSensor, 2000);"
  "window.onload = atualizarSensor;"
  "</script>"
  "</div>"
  "</body>"
  "</html>";
  server.send(200, "text/html", html);
}
// Endpoint para retornar temperatura e umidade em JSON
void handleSensor() {
  String json = "{";
  json += "\"temperatura\":" + String(temperatura, 1) + ",";
  json += "\"umidade\":" + String(umidade, 1);
  json += "}";
  server.send(200, "application/json", json);
}

void handleOn() {
  digitalWrite(ledPin, LOW); // LOW liga o LED interno (ativo em nível baixo)
  Serial.println("Botão Ligar pressionado");
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleOff() {
  digitalWrite(ledPin, HIGH); // HIGH desliga o LED interno
  Serial.println("Botão Desligar pressionado");
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // começa desligado

  Serial.begin(9600);
  dht.begin();
  Serial.println("Sensor DHT11 iniciado!");

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado! IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/sensor", handleSensor);

  server.begin();
  Serial.println("Servidor web iniciado!");
}

void loop() {
  // Atualiza leitura do DHT11 a cada ciclo
  float temp = dht.readTemperature();
  float umi = dht.readHumidity();
  if (!isnan(temp) && !isnan(umi)) {
    temperatura = temp;
    umidade = umi;
  }

  // Gatilho: se temperatura < 22°C, liga o LED
  if (temperatura > 27.0) {
    digitalWrite(ledPin, LOW); // Liga LED
  } else {
    digitalWrite(ledPin, HIGH); // Desliga LED
  }

  server.handleClient();
  delay(2000); // Atualiza a cada 2s
}
