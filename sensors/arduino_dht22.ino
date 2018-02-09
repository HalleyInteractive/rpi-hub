#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <dht.h>
#include <JeeLib.h>

#define DHT22_PIN 5
#define SLEEP_TIME 5000 // 5 Seconds

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

RF24 radio(7,8);
RF24Network network(radio);

dht DHT;

const uint16_t address = 01;
const uint16_t hub_address = 00;
int counter = 0;

struct payload_t {
  float humidity;
  float temperature;
  uint16_t id;
};

void setup(void)
{
  Serial.begin(115200);
  SPI.begin();
  radio.begin();
  network.begin(90, address);
}

void loop() {
  network.update();
  int chk = DHT.read22(DHT22_PIN);
  float tmp = 0.0;
  if(chk == DHTLIB_OK) {
    tmp = DHT.temperature;
  } else {
    Serial.print("Error reading DHT");
  }
  Serial.print("Sending...");
  payload_t payload = { millis(), counter++ };
  Serial.print(tmp);
  RF24NetworkHeader header(hub_address);
  bool ok = network.write(header, &payload, sizeof(payload));
  if (ok) {
    Serial.println("OK");
  } else {
      Serial.println("FAILED");
  }
  
  Serial.println("SLEEP");
  radio.stopListening();
  delay(5000);
  Sleepy::loseSomeTime(SLEEP_TIME);
  
 Serial.println("AWAKE"); 
}
