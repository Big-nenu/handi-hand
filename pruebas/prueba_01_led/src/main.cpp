
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include "wifi_secrets.h"

#define LED_PIN 4
#define BUTTON_PIN 5

#define SERVICE_UUID "a1700001-89ab-4cde-9012-123456789abc"
#define CONTROL_UUID "a1700002-89ab-4cde-9012-123456789abc"

WiFiMulti wifiMulti;
WebServer server(80);

bool ledRemoto = false;
bool servidorIniciado = false;
unsigned long ultimaRevisionWiFi = 0;

// ===== CONTROL DEL LED =====

void actualizarLED() {
    bool boton = digitalRead(BUTTON_PIN) == LOW;
    digitalWrite(LED_PIN, (ledRemoto || boton) ? HIGH : LOW);
}

// ===== BLUETOOTH BLE =====

class ControlBLE : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* caracteristica) override {
        String valor = caracteristica->getValue().c_str();

        if (valor == "1") {
            ledRemoto = true;
            Serial.println("BLE: LED ON");
        }
        else if (valor == "0") {
            ledRemoto = false;
            Serial.println("BLE: LED OFF");
        }
    }
};

void iniciarBluetooth() {
    BLEDevice::init("Handi-Hand");

    BLEServer* servidorBLE = BLEDevice::createServer();
    BLEService* servicio = servidorBLE->createService(SERVICE_UUID);

    BLECharacteristic* control = servicio->createCharacteristic(
        CONTROL_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );

    control->setCallbacks(new ControlBLE());
    control->setValue("0");

    servicio->start();

    BLEAdvertising* publicidad = BLEDevice::getAdvertising();
    publicidad->addServiceUUID(SERVICE_UUID);
    publicidad->start();

    Serial.println("Bluetooth BLE iniciado");
}

// ===== SERVIDOR WEB =====

const char PAGINA[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Handi-Hand</title>
<style>
body {
    background: #121524;
    color: white;
    font-family: Arial, sans-serif;
    text-align: center;
    padding: 60px 15px;
}
.card {
    background: #202538;
    padding: 35px;
    border-radius: 20px;
    max-width: 350px;
    margin: auto;
}
button {
    border: none;
    border-radius: 14px;
    padding: 20px 45px;
    margin: 10px;
    color: white;
    font-size: 22px;
    font-weight: bold;
}
.on { background: #15803d; }
.off { background: #b91c1c; }
</style>
</head>
<body>
<h1>Handi-Hand</h1>
<p>Control de StoneBot</p>

<div class="card">
    <h1>💡</h1>
    <h2>LED externo</h2>
    <p id="estado">Consultando...</p>
</div>

<br>
<button class="on" onclick="control('on')">ON</button>
<button class="off" onclick="control('off')">OFF</button>

<script>
async function actualizar() {
    try {
        const respuesta = await fetch('/status');
        const datos = await respuesta.json();
        document.getElementById('estado').innerText =
            datos.encendido ? 'ENCENDIDO' : 'APAGADO';
    } catch {
        document.getElementById('estado').innerText = 'SIN CONEXIÓN';
    }
}

async function control(accion) {
    await fetch('/' + accion, {method: 'POST'});
    actualizar();
}

actualizar();
setInterval(actualizar, 1000);
</script>
</body>
</html>
)HTML";

void iniciarServidorWeb() {
    server.on("/", HTTP_GET, []() {
        server.send_P(200, "text/html", PAGINA);
    });

    server.on("/on", HTTP_POST, []() {
        ledRemoto = true;
        server.send(200, "text/plain", "ON");
    });

    server.on("/off", HTTP_POST, []() {
        ledRemoto = false;
        server.send(200, "text/plain", "OFF");
    });

    server.on("/status", HTTP_GET, []() {
        bool encendido = ledRemoto ||
                         digitalRead(BUTTON_PIN) == LOW;

        server.send(200, "application/json",
            String("{\"encendido\":") +
            (encendido ? "true" : "false") + "}");
    });
}

// ===== CONFIGURACIÓN INICIAL =====

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_CASA, PASS_CASA);
    wifiMulti.addAP(WIFI_OFICINA, PASS_OFICINA);

    iniciarServidorWeb();
    iniciarBluetooth();

    Serial.println("Handi-Hand iniciado");
}

// ===== CICLO PRINCIPAL =====

void loop() {
    actualizarLED();

    if (millis() - ultimaRevisionWiFi >= 10000) {
        ultimaRevisionWiFi = millis();

        if (wifiMulti.run(3000) == WL_CONNECTED) {
            if (!servidorIniciado) {
                server.begin();
                servidorIniciado = true;
                Serial.println("Servidor web iniciado");
            }

            Serial.print("Wi-Fi IP: ");
            Serial.println(WiFi.localIP());
        }
    }

    if (servidorIniciado && WiFi.status() == WL_CONNECTED) {
        server.handleClient();
    }

    delay(10);
}
