#include "streaming-worker.h"
#include <chrono>
#include <thread>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <time.h>

class Rf24Hub : public StreamingWorker {
  public:
  Rf24Hub(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> & options)
        : StreamingWorker(data, complete, error_callback) {
  }

  void Execute (const AsyncProgressWorker::ExecutionProgress& progress) {

    RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
    RF24Network network(radio);

    // Address of our node in Octal format
    const uint16_t this_node = 00;

    struct payload_t {                  // Structure of our payload
      unsigned long ms;
      unsigned long counter;
      uint16_t id;
      float humidity;
      float temperature;
    };

    radio.begin();
    delay(5);
    network.begin(90, this_node);
    radio.printDetails();
    while(1) {
      network.update();
      while ( network.available() ) {     // Is there anything ready for us?
        RF24NetworkHeader header;        // If so, grab it and print it out
        payload_t payload;
        network.read(header,&payload,sizeof(payload));
        printf("Received payload # %lu at %lu \n", payload.counter, payload.ms);
        Message tosend("message", std::to_string(payload.counter));
	    writeToNode(progress, tosend);
      }
      delay(2000);
    }
  }
};

StreamingWorker * create_worker(Callback *data
    , Callback *complete
    , Callback *error_callback, v8::Local<v8::Object> & options) {
  return new Rf24Hub(data, complete, error_callback, options);
}

NODE_MODULE(rf24hub, StreamWorkerWrapper::Init)
