
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include "wifi_secrets.h"

#define LED_PIN 4
#define BUTTON_PIN 5

WiFiMulti wifiMulti;
WebServer server(80);

bool ledRemoto = false;
bool servidorIniciado = false;
unsigned long ultimaRevision = 0;

const char PAGINA[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport"
        content="width=device-width, initial-scale=1">
  <meta name="theme-color" content="#121524">
  <title>Handi-Hand</title>
  <style>
    * { box-sizing: border-box; }
    body {
      margin: 0;
      min-height: 100vh;
      display: grid;
      place-items: center;
      background: #121524;
      color: white;
      font-family: system-ui, sans-serif;
    }
    main {
      width: min(90%, 380px);
      text-align: center;
    }
    h1 { font-size: 32px; }
    .card {
      background: #202538;
      padding: 35px 15px;
      border-radius: 24px;
      margin: 25px 0;
    }
    .bulb { font-size: 70px; }
    #estado { color: #adb5c9; }
    .buttons { display: flex; gap: 15px; }
    button {
      flex: 1;
      border: 0;
      border-radius: 16px;
      padding: 25px 10px;
      color: white;
      font-size: 22px;
      font-weight: bold;
    }
    .on { background: #15803d; }
    .off { background: #b91c1c; }
  </style>
</head>
<body>
<main>
  <h1>Handi-Hand</h1>
  <p>Control de StoneBot</p>
  <div class="card">
    <div class="bulb">💡</div>
    <h2>LED externo</h2>
    <p id="estado">Consultando...</p>
  </div>
  <div class="buttons">
    <button class="on" onclick="control('on')">ON</button>
    <button class="off" onclick="control('off')">OFF</button>
  </div>
</main>
<script>
  async function actualizar() {
    try {
      const r = await fetch('/status');
      const d = await r.json();
      document.getElementById('estado').textContent =
        d.encendido ? 'ENCENDIDO' : 'APAGADO';
    } catch {
      document.getElementById('estado').textContent =
        'SIN CONEXIÓN';
    }
  }

  async function control(accion) {
    try {
      await fetch('/' + accion, {method: 'POST'});
      await actualizar();
    } catch {
      document.getElementById('estado').textContent =
        'ERROR DE CONEXIÓN';
    }
  }

  actualizar();
  setInterval(actualizar, 1000);
</script>
</body>
</html>
)HTML";

void aplicarLED() {
  bool boton = digitalRead(BUTTON_PIN) == LOW;
  digitalWrite(LED_PIN, (ledRemoto || boton) ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_CASA, PASS_CASA);
  wifiMulti.addAP(WIFI_OFICINA, PASS_OFICINA);

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", PAGINA);
  });

  server.on("/on", HTTP_POST, []() {
    ledRemoto = true;
    aplicarLED();
    server.send(200, "text/plain", "ON");
  });

  server.on("/off", HTTP_POST, []() {
    ledRemoto = false;
    aplicarLED();
    server.send(200, "text/plain", "OFF");
  });

  server.on("/status", HTTP_GET, []() {
    bool encendido = ledRemoto ||
                     digitalRead(BUTTON_PIN) == LOW;
    server.send(200, "application/json",
      String("{\"encendido\":") +
      (encendido ? "true" : "false") + "}");
  });

  Serial.println("Handi-Hand iniciado");
}

void loop() {
  aplicarLED();

  if (millis() - ultimaRevision >= 10000) {
    ultimaRevision = millis();

    if (wifiMulti.run(3000) == WL_CONNECTED) {
      if (!servidorIniciado) {
        server.begin();
        servidorIniciado = true;
        Serial.println("Servidor iniciado");
      }

      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
    }
  }

  if (servidorIniciado && WiFi.status() == WL_CONNECTED) {
    server.handleClient();
  }

  delay(10);
}
