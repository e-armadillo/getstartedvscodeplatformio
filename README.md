# Programacion de sensores y comunicacion MQTT

En este repositorio nos hemos orientado a la programacion de diversos sensores (temperatura, humedad, ph, etc) de tal manera que se puedan comunicar mediante el protocolo MQTT a un MQTT Broker.

En la carpeta **DocumentacionPlatformIO** se encuentra el archivo Markdown que explica como instalar PlatformIO en nuestro VisualStudio.

En la carpeta **SensorTemperaturaHumedad** se encuentran dos sub carpetas:
- DHT11
- DHT_MQTT
 
 Y un archivo Markdown denominado *Instalacion DHT11* la cual explica como usar el sensor DHT11 (temperatura y humedad), como programarlo, conectarlo y comunicarlo mediante el protocolo mencionado.

 En la sub carpeta **DHT11** se encuentra un ejemplo basico mencionado en el markdown de como programar el sensor y en la sub carpeta **DHT_MQTT** se encuentra el archivo **main.cpp** responsable de comunicar el sensor con el MQTT Broker.
