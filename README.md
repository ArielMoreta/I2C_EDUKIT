# Documentación del Proyecto

## Descripción del Proyecto

Este proyecto tiene como objetivo capturar datos de un sensor MAX30105 con un ESP32 y visualizarlos en una pantalla OLED. Los datos, que incluyen las señales IR y Red del sensor, se envían a un servidor a través de HTTP, donde se almacenan en una base de datos MySQL para su posterior análisis.

## Componentes Utilizados

1. **ESP32**: Microcontrolador que recolecta los datos del sensor y maneja la comunicación con la pantalla OLED y el servidor.
2. **MAX30105**: Sensor óptico utilizado para medir la frecuencia cardíaca y los niveles de oxígeno en la sangre.
3. **Pantalla OLED 1.3 pulgadas**: Pantalla para mostrar los datos de manera gráfica en tiempo real.
4. **Servidor con PHP y MySQL**: Sistema que recibe y almacena los datos enviados por el ESP32.

## Archivos del Proyecto

1. **`database.php`**:
   - Ubicación: Carpeta de XAMPP
   - Descripción: Archivo PHP que maneja la conexión a la base de datos MySQL utilizando PDO para asegurar una comunicación segura y eficiente.

2. **`post.php`**:
   - Ubicación: Carpeta de XAMPP
   - Descripción: Script PHP encargado de recibir los datos POST enviados por el ESP32 y almacenarlos en la base de datos. Valida los datos recibidos y maneja posibles errores durante la inserción.

3. **`base_de_datos.sql`**:
   - Ubicación: Archivo de base de datos
   - Descripción: Script SQL que crea la base de datos `tesis` y la tabla `datos_oxi`, donde se almacenan las lecturas IR y Red del sensor.

## Conexiones de Hardware

1. **Conexión MAX30105 con ESP32**:
   - **SCL** del MAX30105 a **GPIO22** del ESP32
   - **SDA** del MAX30105 a **GPIO21** del ESP32
   - **VCC** del MAX30105 a **3.3V/5V** del ESP32
   - **GND** del MAX30105 a **GND** del ESP32

2. **Conexión Pantalla OLED con ESP32**:
   - **SCL** de la pantalla OLED a **GPIO22** del ESP32
   - **SDA** de la pantalla OLED a **GPIO21** del ESP32
   - **VCC** de la pantalla OLED a **3.3V/5V** del ESP32
   - **GND** de la pantalla OLED a **GND** del ESP32

## Programas Necesarios

1. **XAMPP**:
   - Descripción: Paquete de software que incluye Apache, MySQL, y PHP para el desarrollo local de aplicaciones web.
   - [Descargar XAMPP](https://sourceforge.net/projects/xampp/files/XAMPP%20Windows/8.2.12/xampp-windows-x64-8.2.12-0-VS16-installer.exe/download)

2. **Composer**:
   - Descripción: Herramienta para la gestión de dependencias en PHP.
   - [Descargar Composer](https://getcomposer.org/Composer-Setup.exe)

3. **Arduino IDE**:
   - Descripción: Entorno de desarrollo para programar el ESP32.
   - [Descargar Arduino IDE](https://www.arduino.cc/en/software)

4. **Grafana**:
   - Descripción: Plataforma para la visualización y análisis de datos en tiempo real.
   - [Descargar Grafana](https://grafana.com/grafana/download?pg=get&plcmt=selfmanaged-box1-cta1)

## Instrucciones de Configuración

1. **Configurar el ESP32**:
   - Carga el código en el ESP32 utilizando el Arduino IDE.
   - Asegúrate de conectar correctamente el hardware según el esquema de conexiones.

2. **Configurar XAMPP**:
   - Instala XAMPP y coloca los archivos `database.php` y `post.php` en la carpeta `htdocs`.
   - Inicia los servicios de Apache y MySQL desde el panel de control de XAMPP.

3. **Configurar la Base de Datos**:
   - Utiliza phpMyAdmin o una herramienta similar para importar el archivo `base_de_datos.sql` y crear la base de datos `tesis` con la tabla `datos_oxi`.

4. **Instalar Dependencias con Composer**:
   - Ejecuta `composer install` en la carpeta del proyecto para instalar las dependencias necesarias.

5. **Configurar Grafana**:
   - Instala Grafana y configura la fuente de datos para conectarla a la base de datos MySQL.
   - Crea dashboards para visualizar los datos capturados en tiempo real.

6. **Probar la Conexión**:
   - Asegúrate de que el ESP32 esté conectado a la red WiFi configurada.
   - Verifica que los datos se envíen correctamente al servidor y se almacenen en la base de datos.

## Flujo de Datos

1. El ESP32 obtiene los valores IR y Red del sensor MAX30105.
2. Los datos se visualizan en la pantalla OLED en tiempo real.
3. Si el ESP32 está conectado a WiFi, los datos se envían al servidor.
4. El servidor recibe los datos a través de `post.php` y los almacena en la base de datos MySQL.
5. Los datos pueden ser accedidos posteriormente para su análisis o visualización en Grafana.

---

**Realizado por:** Ariel Moreta y Adrian Vera
