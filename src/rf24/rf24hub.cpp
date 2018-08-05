#include "streaming-worker.h"
#include <chrono>
#include <thread>
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <sstream>
#include <time.h>
#include <iomanip>

class Rf24Hub : public StreamingWorker {
  public:
  Rf24Hub(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> & options)
        : StreamingWorker(data, complete, error_callback) {
  }

  void Execute (const AsyncProgressWorker::ExecutionProgress& progress) {

    RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
    RF24Network network(radio);

    const uint16_t this_node = 00;

    struct payload_t {
      uint16_t id;
      uint16_t temperature;
      uint16_t humidity;
    };

    radio.begin();
    delay(5);
    network.begin(90, this_node);
    radio.printDetails();
    while(1) {
      network.update();
      while ( network.available() ) {
        RF24NetworkHeader header;
        
        payload_t payload;
        network.read(header, &payload, sizeof(payload));
	    
        float f_temperature = (float)payload.temperature / 10;
        float f_humidity = (float)payload.humidity / 10;
        
        stringstream stream;

        stream << fixed << setprecision(1) << f_temperature;
        string temperature = stream.str();
        
        stream.str("");
        stream.clear();
        stream << fixed << setprecision(1) << f_humidity;
        string humidity = stream.str();

        // printf("Received tmp: %s, hum: %s from %i\n", temperature.c_str(), humidity.c_str(), payload.id);
        
        stream.str("");
        stream.clear();

        stream << "{\"temperature\": "<< temperature << ",\"humidity\": " << humidity << ",\"id\": " << payload.id << "}";
        string json_payload = stream.str();

        Message tosend("message", json_payload);
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
