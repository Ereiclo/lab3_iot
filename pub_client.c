
#include <WiFi.h>
#include <PubSubClient.h>
// WiFi
const char *ssid = "ERIC";     // Enter your WiFi name
const char *password = "12345678"; // Enter WiFi password
// MQTT Broker
const char *mqtt_broker = "192.168.181.74"; // Enter your WiFi or Ethernet IP
const char *topic = "test/topic";
const int mqtt_port = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

float duration, distance;  

#define triggerPIN 4
#define echoPIN 2

void distance_fun(){
    digitalWrite(triggerPIN, LOW);
    delayMicroseconds(2);

    digitalWrite(
        triggerPIN,
        HIGH);
    delayMicroseconds(
        10);

    digitalWrite(triggerPIN,
                 LOW);

    duration = pulseIn(echoPIN, HIGH);
    distance = duration * 0.0344 / 2.;
    Serial.println("Duration: " + String(duration));
    Serial.print("Distance: ");
    Serial.print(
        distance);
    Serial.println(" cm");

 
}

void setup()
{
    // Set software serial baud to 115200;
    Serial.begin(115200);
  pinMode(triggerPIN, OUTPUT );
  pinMode(echoPIN, INPUT );

    // connecting to a WiFi network
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");

    // connecting to a mqtt broker
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(callback);

    

}
void reconnected(){
  while (!client.connected())
    {
        String client_id = "esp8266-client-";
        client_id += String(WiFi.macAddress());

        Serial.printf("The client %s connects to mosquitto mqtt broker\n", client_id.c_str());

        if (client.connect(client_id.c_str()))
        {
            Serial.println("Public emqx mqtt broker connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
  
}
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    Serial.println();
    Serial.println(" - - - - - - - - - - - -");
}
void loop()
{
  reconnected(); 
  
  client.loop();
      // publish and subscribe
  distance_fun(); 
  String msg =  "La distancia es " + String(distance);
  client.publish(topic, msg.c_str());
    //client.subscribe(topic);
  

}



