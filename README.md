##  Smart Home System with ESP32 sensor to monitor temperature, light level for my indoor plant growth

A real-time smart home tracking system using an ESP32 and sensor to monitor temperature,  air quality, and light levels for my indoor plants. 

The project involves a smart home system using an ESP32 to monitor temperature, air quality, and light levels for indoor plants.

The `ClientSide-esp.ino` file contains the code for the ESP32 client, which connects to WiFi and an MQTT broker, subscribes to a topic, and publishes sensor data.

The `index.js` file contains the server-side code, which uses Express, MQTT, and Socket.IO to handle incoming sensor data, update the web client, and save data to a PostgreSQL database.

The `public/detail.ejs` and `public/graph.html` files contain the front-end code for displaying sensor data and graphs. 



