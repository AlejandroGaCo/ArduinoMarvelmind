# Marvelmind Arduino Library
Esta documentación tiene como objetivo explicar las clases y funciones necesarias para hacer interfaz entre un *beacon* Marvelmind y una tarjeta Arduino Mega 2560. Se usó una librería desarrollada por racarla96 [1], que a su vez está basada en la librería Marvelmind C Library [2].

## Principio de funcionamiento
Una vez realizada la conexión entre el *beacon* móvil y la placa Arduino, el sensor comienza un flujo de información. Este flujo consiste en diferentes tipos de mensajes con códigos únicos:

| Código | Tipo de mensaje                                                   | Origen                                          |
| ------ | ----------------------------------------------------------------- | ----------------------------------------------- |
| 0x0001 | Posición con resolución en cm                                     | Hedgehog                                        |
| 0x0011 | Posición con resolución en mm                                     | Hedgehog                                        |
| 0x0081 | Posición con resolución en mm y marcas de tiempo real             | Hedgehog                                        |
| 0x0002 | Posición de los otros *beacons* con resolución en cm               | Beacon                                          |
| 0x0012 | Posición de los otros *beacons* con resolución en mm               | Beacon                                          |
| 0x0003 | Datos inerciales sin procesar                                     | 'Raw inertial sensors data' habilitado          |
| 0x0083 | Datos inerciales sin procesar y marcas de tiempo real             | 'Raw inertial sensors data' habilitado          |
| 0x0004 | Distancias sin procesar a *beacons* fijos                         | 'Raw distances data' habilitado                 |
| 0x0084 | Distancias sin procesar a *beacons* fijos y marcas de tiempo real | 'Raw distances data' habilitado                 |
| 0x0005 | Datos inerciales procesados                                       | 'Processed IMU data' habilitado                 |
| 0x0085 | Datos inerciales procesados y marcas de tiempo real               | 'Processed IMU data' habilitado                 |
| 0x0006 | Datos de telemetría                                               | 'Telemetry stream' habilitado                   |
| 0x0007 | Datos de calidad y localización extendida                         | 'Quality and extended location data' habilitado |

La librería Marvelmind_Arduino_Library se encarga de tomar estos mensajes y convertirlos en objetos con diferentes atributos, dependiendo del mensaje. **La librería no soporta los mensajes de tipo 0x008X o con marcas en tiempo real**. Se necesita desactivar esta configuración en el *Dashboard.*

Consultar el manual de interfaces de Marvelmind [3] para información más detallada sobre la estructura de cada paquete. 

## Clases
Las clases necesarias para el funcionamiento de la librería se encuentra en el archivo *Marvelmind.h*

### MarvelMindHedge
Principal clase de la librería. Contiene todas las funciones necesarias para recibir y procesar la información transmitida por un *beacon* móvil
```cpp
lass MarvelmindHedge
{
public:
	\\ Funciones de inicialización
    void begin(Stream *serial);
    void begin(Stream *serial, Stream *serialDebug);

	\\ Función de lectura
    void read();
    
	\\ Funciones de impresión
    void printPositionFromMarvelmindHedge(bool onlyNew);
    void printStationaryBeaconsPositionsFromMarvelmindHedge(bool onlyNew);
    void printRawDistancesFromMarvelmindHedge(bool onlyNew);
    void printRawIMUFromMarvelmindHedge(bool onlyNew);
    void printFusionIMUFromMarvelmindHedge(bool onlyNew);
    void printTelemetryFromMarvelmindHedge(bool onlyNew);
    void printQualityFromMarvelmindHedge(bool onlyNew);

	\\ Funciónes de obtención de información
    bool getPositionFromMarvelmindHedge(bool onlyNew, PositionValuePro *positionValuePro);
    bool getRawDistancesFromMarvelmindHedge(bool onlyNew, RawDistancesPro *rawDistancesPro);
    bool getRawIMUFromMarvelmindHedge(bool onlyNew, RawIMUValuePro *rawIMUPro);
    bool getFusionIMUFromMarvelmindHedge(bool onlyNew, FusionIMUValuePro *fusionIMUValuePro);
private: ...
};
```
Esta clase tiene funciones y atributos adicionales que sirven solo internamente al procesamiento de los datos. 

### PositionValuePro
Clase usada para guardar las coordenadas y ángulo de un *beacon*. 
```cpp
class PositionValuePro
{
public:
    float x, y, z; // en metros
    float angle; // en grados
};
```

### RawDistancesPro
Clase usada para guardar las distancias entre el *beacon* actual y los demás *beacons*.
```cpp
class RawDistancesPro
{
public:
    uint8_t address_hedge;
    float distances[NDISTANCES]; // en metros 
    uint8_t translate_beacons[NDISTANCES]; // ordenadas de menor a mayor por dirección de beacon
    uint8_t address_beacons[NDISTANCES]; // ordenadas de menor a mayor por dirección de beacon
    bool sorted = false;
};
```

### FusionIMUValuePro
Clase usada para guardar las coordenadas, orientaciones, velocidades y aceleración de un *beacon*.
 ```cpp
 class FusionIMUValuePro
{
public:
    float x;
    float y;
    float z; // en metros

    float roll;
    float pitch;
    float yaw; // ángulos de Euler en radianes

    float vx;
    float vy;
    float vz; // velocidad en m/s

    float ax;
    float ay;
    float az; // aceleración en m/s^2
};
```

### RawIMUValuePro
Clase usada para guardas las lecturas sin procesar del acelerómetro, giróscopos y brújula. 
```cpp
class RawIMUValuePro
{
public:
    float acc_x; 
    float acc_y; 
    float acc_z; // m/s^2

    float gyro_x;
    float gyro_y;
    float gyro_z; // rad/s

    float compass_x;
    float compass_y;
    float compass_z; // µT
};
```

## Funciones
Las funciones definidas dentro de la clase *MarvelmindHedge* se encuentran en *Marvelmind.cpp*. Se dividen en cuatro grandes categorías:
1. Funciones de inicilización
2. Función de lectura
3. Funciones de impresión
4. Funciones de obtención de información. 

### Funciones de inicialización
Dos funciones que inicializan todas las variables necesarias para el procesamiento de información y establecen la conexión a través de los puertos seriales.
```cpp
MarvelmindHedge Sensor;

Sensor.begin(&Serial2);
Sensor.begin(&Serial2,&Serial);
```
Reciben como input el puerto serial de comunicación, y opcionalmente el puerto serial de impresión. No regresan un output. 

### Función de lectura
Función de lectura y actualización de datos. Espera la llegada de los paquetes y los procesa y guarda internamente en el objeto *MarvelmindHedge*. Se necesita ejecutar periódicamente para actualizar la información. 
```cpp
hedge.read();
```
No tiene inputs y no regresa ningún output. 

### Funciones de impresión
Ejecutadas después de una lectura, imprimen la información recibida al serial de comunicación. Cada mensaje tiene su correspondiente función.
```cpp
hedge.printPositionFromMarvelmindHedge(false);
hedge.printStationaryBeaconsPositionsFromMarvelmindHedge(false);
hedge.printRawDistancesFromMarvelmindHedge(false);
hedge.printRawIMUFromMarvelmindHedge(false);
hedge.printFusionIMUFromMarvelmindHedge(false);
hedge.printTelemetryFromMarvelmindHedge(false);
hedge.printQualityFromMarvelmindHedge(false);
```
Reciben como input un parámetro booleano que indica si imprime solo información nueva (*true*) o imprime información que contenga (*false*). Su output se visualiza directo en la consola (Ctrl+Shift+M)

### Funciones de obtención de información
Ejecutadas después de una lectura, actualizan la información de las clases correspondientes dadas (PositionValuePro, RawDistancesPro, RawIMUValuePro, FusionIMUValuePro). Cada clase tiene su correspondiente función. 

```cpp
hedge.getPositionFromMarvelmindHedge(false, PositionValuePro);
hedge.getRawDistancesFromMarvelmindHedge(false, RawDistancesPro);
hedge.getRawIMUFromMarvelmindHedge(false, RawIMUValuePro);
hedge.getFusionIMUFromMarvelmindHedge(false, FusionIMUValuePro);
```
Reciben como input un parámetro booleano que indica si imprime solo información nueva (*true*) o imprime información que contenga (*false*). También recibe una clase correspondiente al tipo de información que se está pidiendo. Como output regresa un booleano indicando si la información fue actualizada. 

## Ejemplo
El siguiente código ejecuta una impresión de la información del IMU del sensor y compara la posición actual contra una posición deseada, regresando el error en metros. 
```cpp
//Librería Marvelmind
#include <Marvelmind.h> 

//Declaramos bjetos a utilizar
MarvelmindHedge hedge; //hedgehog a declarar
PositionValuePro Objetvio, Actual; //Posición objetivo y actual

unsigned long t1; //para función millis()
const unsigned long period = 125; //tasa de refresco en mseg, 8 Hz
long baudrate = 500000; //baudrate para interfaz Marvelmind
const int res = 2; //resolución para imprimir valores, número de decimales

float errorPosX, errorPosY; //Variables de error

void setup() {
  Serial.begin(baudrate);
  Serial2.begin(baudrate);

  hedge.begin(&Serial2, &Serial); //Declaramos objeto hedge
  t1 = millis();

  //Declaramos posición Objetivo, (2,2) en el plano en metros
  Objetivo.x = 2;
  Objetivo.y = 2;
}

void loop() {
  hedge.read();
  if(millis() - t1 > period){

    //Obtenemos la posición del hedgehog y la guardamos en Actual
    if (hedge.getPositionFromMarvelmindHedge(true, &Actual)) {
	Serial.println("Actualice mi posicion");
    }

    errorPosX = Objetivo.x - Actual.x;
    errorPosY = Objetivo.y - Actual.y;
    
    Serial.print("Errores: errorPosX=");
    Serial.print(errorPosX, res);
    Serial.print(" PosX=");
    Serial.print(Actual.x, res);
    Serial.print(" errorPosY=");
    Serial.print(errorPosY, res);
    Serial.print(" PosY=");
    Serial.print(Actual.y, res);
    Serial.print(" PosZ=");
    Serial.println(Actual.Z, res);

    // Imprimimos datos de fusion IMU
    hedge.printFusionIMUFromMarvelmindHedge(true);

    // Actualizamos tiempo
    t1 = millis();
  }
}
```

## Notas finales 
Para funcionalidades y clases adicionales, revisar los códigos de *Marvelmind.h* y *Marvelmind.cpp* [1].

## Referencias
[1] Marvelmind Robotics, “Hardware interfaces and protocols of data exchange with Marvelmind devices,” Mar. 2024. Consultado el: 14, Abril, 2024. [Online]. Disponible en: https://marvelmind.com/pics/marvelmind_interfaces.pdf

[2] racarla96, “Marvelmind_Arduino_Library,” _GitHub_, Nov. 01, 2020. https://github.com/racarla96/Marvelmind_Arduino_Library (Consultado el 05, Mayo, 2024).

[3] Marvelmind Robotics, “Marvelmind C,” _GitHub_, Mar. 11, 2018. https://github.com/MarvelmindRobotics/marvelmind.c (Consultado el 05, Mayo, 2024).

## Contacto
Documentación elaborada por Alejandro García Cortez a partir de la librería desarrollada por racarla96. Contacto en [alejandrogaco31@gmail.com](mailto:alejandrogaco31@gmail.com)
