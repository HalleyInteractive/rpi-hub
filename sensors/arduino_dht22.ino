/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network 
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <dht.h>

dht DHT;

#define DHT22_PIN 5

RF24 radio(7,8);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;        // Address of our node in Octal format
const uint16_t other_node = 00;       // Address of the other node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already

unsigned long awakeTime = 500;                          // How long in ms the radio will stay awake after leaving sleep mode
unsigned long sleepTimer = 0;  

typedef enum { wdt_16ms = 0, wdt_32ms, wdt_64ms, wdt_128ms, wdt_250ms, wdt_500ms, wdt_1s, wdt_2s, wdt_4s, wdt_8s } wdt_prescalar_e;

struct payload_t {                  // Structure of our payload
  unsigned long ms;
  unsigned long counter;
  float humidity;
  float temperature;
  uint16_t id;
};

void setup(void)
{
  Serial.begin(9600);
  Serial.println("RF24Network/examples/helloworld_tx/");
  //network.setup_watchdog(7);
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  network.setup_watchdog(wdt_1s);
}

void loop() {
  network.update();                          // Check the network regularly
  int chk = DHT.read22(DHT22_PIN);
  float tmp = 0.0;
  if(chk == DHTLIB_OK) {
    tmp = DHT.temperature;
  } else {
    Serial.print("Error reading dht");
  }
  Serial.print("Sending...");
  payload_t payload = { millis(), tmp };
  RF24NetworkHeader header(/*to node*/ other_node);
  bool ok = network.write(header,&payload,sizeof(payload));
  if (ok) {
    Serial.println("ok.");
  } else {
      Serial.println("failed.");
  }
  
  Serial.println("Sleep");
  radio.stopListening();                           // Switch to PTX mode. Payloads will be seen as ACK payloads, and the radio will wake up
  network.sleepNode(600,255); // 10 minutes                          // Sleep the node for 8 cycles of 1second intervals
  Serial.println("Awake"); 
}

