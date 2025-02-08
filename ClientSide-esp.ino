#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>


// Defing wifi and MQTT credentials
const char * ssid = "";
const char * pswrd = " ";
const char * mqtt_server = "10.0.0.53";
// const char * mqtt_server = "192.168.122.1";


//Defin Pins of peripherals
const int tempPin   =    32;
const int presPin   =    33;
const int airPin    =    12;
const int lightPin  =    14;


Servo myServo; // create a servo object
int servoPin = 13;


// Setup Wifi and MQTT Client
WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi(){
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pswrd);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("");
  Serial.println("WiFi connected");

}



void reconnect(){
  // Loop until we're reconnected
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if(client.connect("ESP32Client")){
      Serial.println("connected");

      // suscribe to the topic from the broker
      client.subscribe("esp/cmd");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // try to reconnect in 5 seconds
    }
  }
}



// callback function tohandle incoming messages
void callback(char * topic, byte * payload, unsigned int length){
  // Serial.print("Message arrived on topic: ");
  // Serial.print(topic);
  // Serial.print(". Message: ");
  String message;
  for(int i=0; i < length; i++){
    message += (char)payload[i];
  }
  Serial.println(message);

  // Handle the received messages
  if (String(topic) == "esp/cmd"){
    Serial.println("Received message: " + message);
    // Add code here to handle specific commands
    parseStr(message);
  }
}


void parseStr(const String& str) {
  // Find the position of the comma
  int commaIndex = str.indexOf(',');

  // Extract the text and state parts
  String sensorName = str.substring(0, commaIndex);
  int pinState = str.substring(commaIndex + 1).toInt();
  Serial.println("sensorName-> " + sensorName + ", pinState-> " + pinState);

  // Use a more compact switch-case statement
  if (sensorName == "temperature") {
    digitalWrite(tempPin, pinState);
  } else if (sensorName == "pressure") {
    digitalWrite(presPin, pinState);
  } else if (sensorName == "air") {
    //digitalWrite(airPin, pinState);
    run_servo(pinState);
  } else if (sensorName == "light") {
    digitalWrite(lightPin, pinState);
  } else {
    // Handle unknown sensor names if needed
  }
}










void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  myServo.attach(servoPin); // attach servo
  // set the peripheral pins as outputs
  pinMode(tempPin, OUTPUT);
  pinMode(presPin, OUTPUT);
  pinMode(airPin, OUTPUT);
  pinMode(lightPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
    reconnect();
  }
  client.loop();

  //run_servo();

  // Simulate sensor reading
  // float temperature = 25.0 + (rand() % 100) / 10.0;
  float temperature = random(0, 100) + random(0, 100) / 100.0; 
  float pressure = random(0, 1000) + random(0, 100) / 100.0; 
  float airQuality = random(0, 500) + random(0, 100) / 100.0; 
  float light = random(0, 100) + random(0, 100) / 100.0; 
  // String temp_str = String(temperature);

   // Combine all sensor values into a single comma-separated string using String()
  String payload = String(temperature) + "," + 
                   String(pressure) + "," + 
                   String(airQuality) + "," + 
                   String(light);

  // publish temperature to MQTT topic
  // client.publish("h/l/t", temp_str.c_str());
  client.publish("home/sensors/data", payload.c_str());
  Serial.print("Sensor data sent4: ");
  Serial.println(payload);

  delay(5000); // Publish every 5 seconds

}

void run_servo(int value){
  if (value == 0){
     myServo.write(0);
  } else {
    myServo.write(180);
    //delay(1000);
  }

}







