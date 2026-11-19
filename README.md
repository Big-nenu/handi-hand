# Handi-Hand

**Manos y sentidos para StoneBot.**

Handi-Hand es un proyecto de electrónica, sensores, actuadores y comunicación entre microcontroladores y [StoneBot](https://github.com/Big-nenu/StoneBot), un agente personal que se ejecuta en una computadora.

La idea es que StoneBot no se limite a conversar en una pantalla: que también pueda **percibir señales del entorno** y, cuando tenga autorización, **interactuar con objetos físicos**.

> StoneBot piensa, recuerda y decide. Handi-Hand conecta esas decisiones con el mundo físico.

## Objetivo

Construir una capa de hardware independiente y modular para experimentar con:

- **Sentidos:** luz, temperatura, sonido, movimiento, botones y otros sensores.
- **Manos:** servomotores, motores, LEDs y otros actuadores.
- **Comunicación:** intercambio de datos y órdenes entre StoneBot y un ESP32-S3, primero mediante USB/serial y posteriormente, si resulta útil, por Wi-Fi.
- **Automatización:** combinar lecturas de sensores con acciones físicas, siempre con límites y controles.
- **Expansión futura:** visión mediante cámara, audio y mecanismos más complejos, conforme se validen las pruebas.

Estas son **metas del proyecto**. Por ahora, solo se ha validado la programación del ESP32-S3 y el parpadeo de un LED; la conexión con StoneBot sigue pendiente.

## Relación entre proyectos

```text
StoneBot (agente en la computadora)
    │
    │ Solicitudes de lectura / órdenes autorizadas
    ▼
Handi-Hand (puente de comunicación y control)
    │
    ▼
ESP32-S3 y otros módulos
    ├── Sensores       → información del entorno
    ├── Actuadores     → movimientos y respuestas
    └── Indicadores    → LEDs, pantallas y señales
```

**Separación de responsabilidades:**

- **StoneBot:** conversación, memoria, razonamiento y decisiones.
- **Handi-Hand:** firmware, comunicación, lectura de sensores, control de actuadores y pruebas de hardware.
- **StoneWave:** motor de memoria utilizado por StoneBot; no se duplica dentro de Handi-Hand.

Handi-Hand debe poder probarse **sin tener StoneBot abierto**. La integración llegará una vez que las funciones básicas de hardware estén funcionando.

## Hardware disponible

El punto de partida es un **ESP32-S3 N16R8** y un inventario de componentes de electrónica, incluyendo servomotor de 9 g, motor paso a paso con ULN2003, LDR, sensor de temperatura LM35, sensor de sonido KY-037, LEDs, pulsadores, protoboard y cables.

El inventario completo, con componentes del kit y piezas sueltas, se mantiene en **[inventario_nuevo.md](inventario_nuevo.md)**. Es **un solo inventario**, no dos colecciones separadas.

No todos los componentes tienen que utilizarse: primero se aprovecha lo que ya existe y solo se agregará hardware cuando un experimento concreto lo necesite.

## Ruta de desarrollo

| Etapa | Prueba | Estado |
| --- | --- | --- |
| 1 | Preparar entorno de desarrollo y conectar ESP32-S3 | ✅ Completada (oficina) |
| 2 | Encender y apagar un LED desde el microcontrolador | ✅ Completada (oficina) |
| 3 | Leer un botón o una LDR y devolver su valor por serial | Pendiente |
| 4 | Controlar un servomotor con alimentación adecuada | Pendiente |
| 5 | Crear comandos simples entre la PC y el ESP32-S3 | Pendiente |
| 6 | Conectar comandos y lecturas con StoneBot | Pendiente |
| 7 | Combinar sensores y actuadores en pequeños comportamientos | Pendiente |

Los pasos pueden cambiar en función de las pruebas y del hardware disponible.

## Desarrollo en dos lugares

El proyecto se trabajará **en la oficina y en casa**, desde equipos diferentes. GitHub será el punto de sincronización del código y la documentación.

- Guardar firmware, utilidades, esquemas y notas de pruebas en este repositorio.
- Documentar qué piezas requiere cada experimento.
- Mantener configuraciones locales, claves y datos privados fuera del repositorio.
- Poder probar por separado cada módulo, aunque el resto del hardware esté en otro lugar.

## Principios

1. **Probar una cosa a la vez:** primero verificar entradas y salidas básicas.
2. **Reutilizar lo que ya existe:** evitar compras o dependencias innecesarias.
3. **Módulos pequeños:** sensores y actuadores independientes y fáciles de sustituir.
4. **Control seguro:** no mover motores o activar cargas por iniciativa no autorizada; definir límites y un método de parada.
5. **No romper StoneBot:** integrar mediante una interfaz clara en lugar de mezclar los repositorios.
6. **Documentar resultados:** distinguir lo planeado de lo que realmente funciona.

## Estado actual

**Fase inicial / pruebas de hardware.** Ya se completó la primera prueba física en la computadora de la oficina.

### Prueba 01 — Parpadeo de LED (19 de noviembre de 2026)

**Resultado: completada.**

- **Placa:** ESP32-S3 N16R8 (16 MB Flash, 8 MB PSRAM).
- **Entorno:** Windows, VS Code y PlatformIO con framework Arduino.
- **Conexión:** adaptador USB-serial CH343 detectado en `COM5`.
- **Código:** alterna HIGH/LOW en GPIO 2 cada segundo y escribe mensajes por serial.
- **Compilación:** `SUCCESS`.
- **Carga:** `SUCCESS`; escritura verificada por esptool.
- **Observación física:** LED rojo encendido de forma fija y LED verde parpadeando.
- **Conclusión:** se confirmó la detección, compilación, carga y ejecución del firmware. El parpadeo observado es consistente con el control desde GPIO 2.

Proyecto de prueba: [`pruebas/prueba_01_led/`](pruebas/prueba_01_led/).

**Pendiente inmediato:** leer un botón o una LDR y observar sus valores por serial. La integración con StoneBot todavía no se ha realizado.

> El puerto `COM5` corresponde al equipo de oficina: puede cambiar al conectar la placa en otra computadora.

## Repositorios relacionados

- [StoneBot](https://github.com/Big-nenu/StoneBot): agente personal.
- [StoneWave](https://github.com/Big-nenu/StoneWave): motor de memoria.
