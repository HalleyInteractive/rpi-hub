#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <dht.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
// #include "printf.h"

#define DHT22_PIN 5
#define SLEEP_TIME 7.5 * 5 // 5 Minutes

RF24 radio(7,8);
RF24Network network(radio);

dht DHT;

const uint16_t NODE_ADDRESS = 01;
const uint16_t HUB_ADDRESS = 00;

typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;

struct payload_t {
  uint16_t id;
  uint16_t temperature;
  uint16_t humidity;
};

void setup(void)
{
  // Serial.begin(115200);
  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);
  network.begin(90, NODE_ADDRESS);
  network.setup_watchdog(wdt_8s);
}

void loop() {

  network.update();
  
  int dhtStatus = DHT.read22(DHT22_PIN);
  
  float temperature = 0;
  float humidity = 0;
  
  if(dhtStatus == DHTLIB_OK) {

    temperature = DHT.temperature;
    humidity = DHT.humidity;
  
    // Serial.println("SENDING:");
    // Serial.print("Temperature: ");
    // Serial.println(temperature * 10);
    // Serial.print("Humidity: ");
    // Serial.println(humidity * 10);
    
    payload_t payload = {NODE_ADDRESS, temperature * 10, humidity * 10};
    
    RF24NetworkHeader header(HUB_ADDRESS);
    
    bool sent = network.write(header, &payload, sizeof(payload));
    
    // if (sent) {
    //   Serial.println("OK");
    // } else {
    //   Serial.println("FAILED");
    // }

  } // else {
    // Serial.println("Error reading DHT");
  // }

  radio.stopListening();
  network.sleepNode(SLEEP_TIME, 0); // Pin 2
  // Serial.println("AWAKE"); 
}
