# SmartRehabBar

**SmartRehabBar** es un proyecto centrado en el **desarrollo de un sistema inteligente que se integra en barras de rehabilitación**, permitiendo medir de forma objetiva la fuerza de apoyo ejercida por pacientes durante ejercicios de recuperación funcional.

El sistema utiliza **células de carga** para registrar la carga aplicada, enviando los datos en tiempo real a una plataforma web para su visualización. Esto permite a los profesionales sanitarios realizar un seguimiento cuantitativo de la evolución del paciente.

Además, el proyecto está diseñado con un enfoque **educativo y modular**, facilitando su uso en Formación Profesional y permitiendo futuras ampliaciones como análisis avanzado e integración de inteligencia artificial.

---

## 🎯 Objetivos del proyecto

* Medir de forma objetiva la **fuerza de apoyo** durante ejercicios de rehabilitación.
* Registrar el **tiempo de carga** y detectar descompensaciones.
* Facilitar datos cuantitativos al fisioterapeuta.
* Desarrollar un sistema **replicable, modular y de bajo coste**.
* Servir como base para futuras ampliaciones (doble apoyo, IA, análisis avanzado).

---

## 🧩 Arquitectura general del sistema

El sistema electrónico se compone de los siguientes bloques:

1. Sensórica de fuerza mediante células de carga.
2. Amplificación y conversión ADC con HX711.
3. Microcontrolador ESP32 para procesado y comunicaciones.
4. Sistema de alimentación autónomo con batería Li-Ion.
5. Comunicación WiFi con servidor remoto para visualización de datos.

---

## 🧪 Material electrónico utilizado

### Sensores

* **Células de carga resistivas** (galgas extensométricas).
* Configuración en puente de Wheatstone para mayor estabilidad.

### Conversión y amplificación

* **HX711**

  * ADC de 24 bits específico para células de carga.
  * Alta precisión para señales de baja variación.

### Microcontrolador

* **ESP32 / ESP32-C3 SuperMini**

  * Lectura del HX711.
  * Procesado de datos.
  * Comunicación WiFi.
  * Envío de datos a backend REST.

### Alimentación

* **Batería Li-Ion 18650** (3.0–4.2 V).
* **MCP73871**

  * Gestión de carga.
  * Power-path (uso y carga simultánea).
* **Convertidor Buck-Boost (TPS63020)**

  * Generación de tensión estable para el sistema (3.3 V).
* **Divisor resistivo**

  * Medición del voltaje de batería mediante ADC.

---

## 🧠 Firmware

En este repositorio se incluyen varias versiones del firmware:

### 🔹 PesajeV1

Primera versión funcional del sistema.

**Características:**

* Medición de fuerza en **un único punto de apoyo**.
* Lectura mediante HX711.
* Envío de datos de peso.
* No incluye medición de batería.
* Base para validación inicial.

---

### 🔹 PesajeV2

Versión mejorada del sistema.

**Novedades:**

* ✅ Medición de **nivel de batería en porcentaje**.
* ✅ Lectura de batería mediante ADC + divisor resistivo.
* ✅ Calibración del ADC para mayor precisión.
* ✅ Mejora en estabilidad de lectura.

---

### 🔹 PesajeV3

Versión actual del sistema.

**Novedades principales:**

* ✅ Medición de **doble apoyo (pie izquierdo y derecho)**.
* ✅ Uso de **dos módulos HX711** (uno por cada plataforma).
* ✅ Migración a **ESP32-C3 SuperMini** (reducción de tamaño y consumo).
* ✅ Lectura simultánea de ambos sensores.
* ✅ Envío independiente de datos de cada pie al backend.
* ✅ Mejora en la arquitectura del firmware (estructura modular).

Esta versión permite analizar **asimetrías de carga entre ambos pies**, siendo mucho más útil en entornos reales de rehabilitación.

---

### 🔮 Versiones futuras

* Mejora de filtrado y tratamiento de señal.
* Detección de eventos (apoyo, descarga, equilibrio).
* Optimización de consumo energético.
* Integración de análisis avanzado e IA.
* Visualización avanzada de métricas (balance, evolución, tendencias).

---

## 🔌 Hardware (KiCad)

El repositorio incluye el diseño electrónico completo desarrollado en **KiCad**:

### Contenido:

* 📄 Esquema eléctrico
* 🧩 Diseño de PCB
* 📦 Archivos listos para fabricación

### Versiones:

* Hardware compatible con **PesajeV1**
* Hardware actualizado para **PesajeV2 (medición de batería)**
* 🆕 Hardware **SmartRehab V3**

  * Soporte para **doble HX711**
  * Integración con **ESP32-C3 SuperMini**
  * Optimización del sistema de alimentación
  * Diseño compacto orientado a integración en plataforma

---

## 🔁 Pasos para la replicación

1. Definición del caso de uso.
2. Selección de sensores de fuerza.
3. Integración con HX711.
4. Implementación con ESP32.
5. Diseño del sistema de alimentación.
6. Desarrollo del firmware.
7. Validación con cargas reales.

---

## ⚠️ Consideraciones importantes

* Este sistema es una **herramienta de apoyo**, no un dispositivo médico certificado.
* La fijación mecánica de las células de carga es crítica.
* El sistema está diseñado para ser **modular y ampliable**.
* La medición de batería requiere calibración del ADC.
* Las mediciones de peso requieren **calibración (tara + escala)** tras cambios mecánicos.

---

## 📁 Estructura del repositorio

* `/firmware`

  * `PesajeV1`
  * `PesajeV2`
  * `PesajeV3` 🆕
* `/hardware`

  * `PCB_SmartRehab_V1`
  * `PCB_SmartRehab_V2`
  * `PCB_SmartRehab_V3` 🆕
* `/docs` → Documentación adicional

---

## 📱 Repositorio de la App

Frontend y visualización de datos:
https://github.com/ichueca/SmartRehabBar-App

---

## 👥 Autores

* Jonathan Arizala – https://lasalleberrozpe.eus
* Ainhoa Domínguez – https://nazaret.eus/
* Iñigo Chueca – https://www.zabalburu.org

Proyecto subvencionado por la Viceconsejería de Formación Profesional del Gobierno Vasco a través de https://tknika.eus/
