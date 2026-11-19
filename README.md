# Handi-Hand

**Manos y sentidos para StoneBot · Experimentos con ESP32-S3 y Mini Flipper**

Handi-Hand es un proyecto de electrónica y software para conectar a [StoneBot](https://github.com/Big-nenu/StoneBot) con el mundo físico. El objetivo es que el agente pueda **percibir su entorno** mediante sensores y **actuar sobre él** mediante motores, indicadores y otros dispositivos, siempre con controles adecuados.

> StoneBot razona y recuerda; Handi-Hand le permite recibir señales y ejecutar acciones físicas.

Además, el mismo hardware servirá para experimentar con un **Mini Flipper**, un dispositivo portátil inspirado en la idea de explorar y controlar hardware, sin pretender reproducir todas las funciones del Flipper Zero.

## Dos líneas de desarrollo

### Handi-Hand — sentidos y movimiento para StoneBot

- Leer botones, luz, temperatura, sonido y otros sensores.
- Controlar LEDs, servomotores y motores.
- Recibir órdenes desde StoneBot y devolver estados o mediciones.
- Mantener el firmware del ESP32 independiente de StoneBot y de su motor de memoria [StoneWave](https://github.com/Big-nenu/StoneWave).
- Incorporar después cámara, audio y mecanismos físicos para explorar la construcción de una mano.

### Mini Flipper — herramienta portátil con ESP32-S3

- Usar inicialmente el **celular como pantalla y mando**.
- Crear una app Android para operar el ESP32 por Bluetooth Low Energy (BLE) y, cuando convenga, Wi-Fi.
- Añadir herramientas de infrarrojo y RFID utilizando módulos disponibles.
- Incorporar posteriormente una pantalla, botones y alimentación portátil.
- Compartir el firmware y los módulos comunes con Handi-Hand, evitando duplicar proyectos.

El ESP32-S3 admite Wi-Fi de 2.4 GHz y BLE; otras capacidades de un Flipper Zero, como radio Sub-GHz, necesitan hardware adicional. Las herramientas futuras se desarrollarán y probarán por separado.

## Hardware disponible

Punto de partida: **ESP32-S3 N16R8** (16 MB Flash, 8 MB PSRAM), LED, resistencia de 220 Ω, pulsador y protoboard.

También hay servomotor de 9 g, motor paso a paso con controlador ULN2003, LDR, LM35, sensor de sonido KY-037, módulo RFID RC522, componentes infrarrojos y más.

El listado único y completo de piezas, tanto las guardadas como las sueltas, está en [inventario_nuevo.md](inventario_nuevo.md). No se consideran inventarios separados.

Para alimentación portátil se dispone de una batería Li-ion **18650 de 3.7 V y 1200 mAh**, recuperada de un espumador de leche, y su placa original **FX-YPY16-DV3.2** con Micro-USB. Todavía **no se ha validado** si esa placa sirve como sistema de carga y alimentación para el ESP32. Para utilizar la batería será necesario verificar protección, regulación y capacidad de corriente; no conectarla directamente a los pines de alimentación de la placa.

## Avances verificados — 19 de noviembre de 2026

| Prueba | Resultado | Estado |
| --- | --- | --- |
| 01 · Detección y programación del ESP32 | Windows detectó el adaptador USB-serial CH343 en COM5; compilación y carga correctas con PlatformIO | ✅ Completada |
| 01 · LED integrado | LED verde parpadeó con el programa de prueba; LED rojo de alimentación permaneció fijo | ✅ Completada |
| 02 · LED externo y pulsador | Pulsador en GPIO 5 enciende el LED de GPIO 4, con resistencia de 220 Ω | ✅ Completada |
| 03 · Conexión a Wi-Fi | ESP32 conectado a INFINITUMCCB1 y dirección IP local obtenida | ✅ Completada en oficina |
| 04 · Servidor web | Panel Handi-Hand abrió en la PC y los botones ON/OFF controlaron el LED | ✅ Completada desde PC |
| 04 · Acceso web desde celular | El navegador del celular agotó el tiempo de espera, aunque estaba en el mismo SSID; causa sin confirmar | ⚠️ Pendiente de diagnosticar |
| 05 · Bluetooth BLE | nRF Connect detectó Handi-Hand, se conectó y permitió encender el LED mediante escritura BLE | ✅ Completada |
| Firmware combinado | Compilación y carga correctas para el código que incluye Wi-Fi, servidor HTTP, BLE y pulsador | ✅ Cargado; falta validar todos los modos juntos |

El funcionamiento del Wi-Fi en casa y la conmutación automática entre redes **todavía no se han probado**.

### Firmware actual

Ubicación: [`pruebas/prueba_01_led/src/main.cpp`](pruebas/prueba_01_led/src/main.cpp)

El código actual incluye:

- **GPIO 4:** LED externo.
- **GPIO 5:** pulsador con `INPUT_PULLUP`.
- **WiFiMulti:** dos redes configurables.
- **Servidor HTTP local:** página con botones ON/OFF y consulta de estado.
- **Bluetooth BLE:** dispositivo anunciado como `Handi-Hand`, con característica para escribir `1` (ON) o `0` (OFF).

La configuración de Wi-Fi se guarda localmente en `wifi_secrets.h`, que debe permanecer excluido de Git. Nunca se deben subir contraseñas al repositorio público.

La dirección IP obtenida en las pruebas de oficina fue `192.168.1.208`, pero es **dinámica** y no debe considerarse fija. El puerto `COM5` también corresponde a esa computadora y puede cambiar en casa.

## Próximos pasos

| Prioridad | Trabajo | Estado |
| --- | --- | --- |
| 1 | Validar simultáneamente botón físico, BLE y servidor Wi-Fi con el firmware actual | Pendiente |
| 2 | Separar el firmware en módulos: GPIO, Wi-Fi, BLE, servidor web y futuros periféricos | Pendiente |
| 3 | Crear una interfaz Android sencilla con ON/OFF por BLE, sin depender del router | Pendiente |
| 4 | Probar y documentar conexiones Wi-Fi en oficina y casa | Pendiente |
| 5 | Leer un sensor (LDR o temperatura) y comunicar su medición a la PC/celular | Pendiente |
| 6 | Controlar el servomotor de 9 g y probar movimientos simples | Pendiente |
| 7 | Añadir herramientas de infrarrojo y RFID RC522 al Mini Flipper | Pendiente |
| 8 | Elegir pantalla y sistema seguro de carga/alimentación portátil | Pendiente |
| 9 | Definir una interfaz de comandos para integrar los periféricos con StoneBot | Pendiente |
| 10 | Experimentar con mecanismos para una mano y nuevos sentidos | Futuro |

Los pasos se ajustarán según los resultados reales. No se consideran implementadas las funciones que aparecen como pendientes.

## Arquitectura prevista

```text
StoneBot (PC)                       App Android (futura)
  |                                    |
  | Comandos y lecturas                 | BLE / Wi-Fi
  +------------------+-----------------+
                     |
              ESP32-S3 (Handi-Hand)
                     |
          +----------+----------+
          |          |          |
       Sensores   Actuadores  Mini Flipper
       y botones  y motores   IR / RFID / pantalla
```

La arquitectura es **una meta**, no una integración ya terminada. El ESP32 debe seguir pudiendo probarse de manera independiente de StoneBot.

## Organización del desarrollo

El trabajo se realiza desde **la oficina y la casa**, sincronizando código y documentación mediante GitHub.

La carpeta `pruebas/` conserva los experimentos existentes. Como siguiente mejora, se planea extraer los componentes reutilizables a módulos o archivos separados, en lugar de hacer crecer indefinidamente un único `main.cpp`.

Principios:

1. Reutilizar el hardware disponible antes de comprar componentes nuevos.
2. Mantener pruebas reproducibles y registrar qué funcionó realmente.
3. Evitar duplicar lógica entre Handi-Hand y Mini Flipper.
4. No sobrescribir una prueba estable sin tener su código guardado.
5. Proteger credenciales, configuraciones privadas y operaciones físicas.
6. Integrar con StoneBot mediante una interfaz de comunicación clara, sin mezclar su código ni su memoria con el firmware.

## Repositorios relacionados

- [StoneBot](https://github.com/Big-nenu/StoneBot) — agente personal.
- [StoneWave](https://github.com/Big-nenu/StoneWave) — motor de memoria utilizado por StoneBot.
