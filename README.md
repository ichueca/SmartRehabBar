# SmartRehabBar

**Smart Rehab** es un proyecto innovador centrado en el **desarrollo de un sistema inteligente que se acopla a las barras de sujeción utilizadas en procesos de rehabilitación física por personas con dificultades de movilidad**. El dispositivo incorpora células de carga, que permiten registrar de manera precisa el tiempo de uso y la fuerza de apoyo ejercida sobre las barras durante los ejercicios realizados con el fisioterapeuta.

El sistema recoge estos datos y los visualiza de forma clara para profesionales de la salud, facilitando un seguimiento objetivo de la evolución del paciente. Además, el proyecto está diseñado para integrar técnicas de análisis avanzado e inteligencia artificial que permitan, en fases posteriores, detectar patrones, descompensaciones y tendencias de mejora que complementen el criterio clínico.

La solución se instala sobre barras existentes o rediseñadas y está pensada tanto para su uso en entornos sanitarios reales como para su aplicación educativa en Formación Profesional, permitiendo trabajar con un caso técnico completo y realista.

---

## 🎯 Objetivos del proyecto

- Medir de forma objetiva la **fuerza de apoyo** ejercida durante ejercicios de rehabilitación.
- Registrar el **tiempo de carga** y detectar posibles descompensaciones.
- Facilitar datos cuantitativos al fisioterapeuta para mejorar la toma de decisiones.
- Desarrollar un sistema **replicable, modular y de bajo coste**, orientado a FP y centros sociosanitarios.
- Servir como base para futuras ampliaciones (doble apoyo, análisis avanzado, IA).

---

## 🧩 Arquitectura general del sistema

El sistema electrónico se compone de los siguientes bloques:

1. Sensórica de fuerza mediante células de carga.
2. Amplificación y conversión ADC con HX711.
3. Microcontrolador ESP32 para procesado y comunicaciones.
4. Sistema de alimentación con batería Li-Ion y regulación a 3,3 V.
5. Comunicación WiFi con un servidor remoto para visualización y análisis de datos.

---

## 🧪 Material electrónico utilizado

### Sensores
- **Células de carga resistivas** (galgas extensométricas).
- Configuración en puente de Wheatstone para mejorar estabilidad y sensibilidad.

### Conversión y amplificación
- **HX711**
  - ADC de 24 bits específico para células de carga.
  - Uso del canal A con ganancia 128.
  - Alta resolución para mediciones de fuerza lenta y estable.

### Microcontrolador
- **ESP32**
  - Lectura del HX711.
  - Procesado de datos.
  - Comunicación WiFi.
  - Envío de datos a un backend REST.

### Alimentación
- **Batería Li-Ion 18650** (3,0–4,2 V).
- **Gestor de carga MCP73871**
  - Carga mediante USB-C.
  - Soporte de alimentación simultánea (power-path).
- **Convertidor Buck-Boost TP563020**
  - Generación de 3,3 V estables para el sistema.

> La medición de batería no está incluida en la primera versión de firmware.

---

## 🧠 Firmware

### PesajeV1 (versión actual)

En este repositorio se ha subido el firmware **PesajeV1**, que corresponde a una **primera versión funcional y simplificada** del sistema.

Características de esta versión:
- Medición de fuerza para **una sola pierna / punto de apoyo**.
- Lectura de una célula de carga mediante HX711.
- Envío de datos básicos de pesaje.
- **No incluye medición de batería**.
- Pensada como base estable para validación inicial y pruebas educativas.

### Versiones futuras

En futuras versiones del firmware se incorporarán:
- Medición independiente de **dos apoyos (pierna izquierda y derecha)**.
- Sistema de **monitorización del nivel de batería**.
- Mejora de filtros y tratamiento de señal.
- Ampliación del envío de datos y estados.
- Integración progresiva de análisis avanzado.

---

## 🔁 Pasos seguidos para la replicación electrónica

1. Definición del caso de uso clínico y educativo.
2. Selección de células de carga adecuadas para carga humana.
3. Elección del HX711 como ADC dedicado.
4. Selección del ESP32 por conectividad y versatilidad.
5. Diseño del sistema de alimentación autónomo.
6. Desarrollo del firmware base (PesajeV1).
7. Validación funcional con cargas controladas.

---

## ⚠️ Consideraciones importantes

- El sistema está diseñado como **herramienta de apoyo**, no como dispositivo médico certificado.
- La correcta fijación mecánica de las células de carga es crítica para la fiabilidad de las medidas.
- El firmware está pensado para ser **leído, modificado y ampliado** en contextos educativos.

---

## 📱 Repositorio de la App

Frontend y visualización de datos:
[SmartRehabBarApp](https://github.com/ichueca/SmartRehabBar-App)

---

## 👥 Autores

- Jonathan Arizala – [La Salle Berrozpe](https://lasalleberrozpe.eus)
- Ainhoa Domínguez – [Nazaret Zentroa](https://nazaret.eus/)
- Iñigo Chueca – [Zabalburu Ikastetxea](https://www.zabalburu.org)

Proyecto subvencionado por la Viceconsejería de Formación Profesional del Gobierno Vasco a través de [Tknika](https://tknika.eus/).

