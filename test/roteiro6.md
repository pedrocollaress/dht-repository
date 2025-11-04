---
#  ROTEIRO 6 – NodeMCU com DHT11 e Servidor Web
---

## **Materiais Necessários**

| Quantidade | Componente          |
| ---------- | ------------------- |
| 1x         | NodeMCU (ESP8266)   |
| 1x         | Sensor DHT11        |
| 3x         | Jumpers macho-fêmea |
| 1x         | Cabo USB            |

---

## **Montagem do Circuito**

| Pino do DHT11 | Conexão no NodeMCU |
| ------------- | ------------------ |
| VCC           | 3V3                |
| GND           | GND                |
| DATA          | D2                 |

---

### Arquivo `platformio.ini`

Edite o arquivo e adicione o seguinte conteúdo:

```ini
[env:nodemcuv2]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 9600

lib_deps =
    adafruit/DHT sensor library @ ^1.4.6
    adafruit/Adafruit Unified Sensor @ ^1.1.14
```

- **DHT sensor library** → controla o sensor DHT11/DHT22
- **Adafruit Unified Sensor** → dependência da biblioteca DHT

---

## 💻 **Código apenas do DHT11**

```cpp
#include "DHT.h"


#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("Sensor DHT11 iniciado!");
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler o sensor DHT11!");
  } else {
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.print(" °C | Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
  }

  delay(2000);
}
```

---

### O trabalho é integrar com o script da aula passada.
