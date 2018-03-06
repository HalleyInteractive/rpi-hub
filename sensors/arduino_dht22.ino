#include <dht.h>
#include <avr/pgmspace.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "printf.h"
#include <avr/sleep.h>
#include <avr/power.h>

#define DHT22_PIN 5
#define SLEEP_TIME 60 * 10 // 10 minutes

RF24 radio(7,8);
RF24Network network(radio);

dht DHT;

const uint16_t NODE_ADDRESS = 01;
const uint16_t HUB_ADDRESS = 00;

struct payload_t {
  uint16_t id;
  uint16_t temperature;
  uint16_t humidity;
};

void setup(void)
{
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  network.begin(90, NODE_ADDRESS);
  network.setup_watchdog(6 /* 1 Second */);
}

void loop() {
  network.update();
  int dhtStatus = DHT.read22(DHT22_PIN);
  float temperature = 0;
  float humidity = 0;
  
  if(dhtStatus == DHTLIB_OK) {
    temperature = DHT.temperature;
    humidity = DHT.humidity;
  } else {
    Serial.println("Error reading DHT");
  }
  
  Serial.println("SENDING:");
  
  Serial.print("Temperature: ");
  Serial.println(temperature * 10);
  Serial.print("Humidity: ");
  Serial.println(humidity * 10);
  
  payload_t payload = {NODE_ADDRESS, temperature * 10, humidity * 10};
  
  RF24NetworkHeader header(HUB_ADDRESS);
  
  bool sent = network.write(header, &payload, sizeof(payload));
  
  if (sent) {
    Serial.println("OK");
  } else {
    Serial.println("FAILED");
  }

  radio.stopListening();
  network.sleepNode(SLEEP_TIME, 0); // Pin 2
  Serial.println("AWAKE"); 
}
