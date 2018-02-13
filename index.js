"use strict"
const path = require('path');
const worker = require('streaming-worker');
const addon_path = path.join(__dirname, 'build/Release/rf24hub');
const rf24hub = worker(addon_path);
const { createLogger, format, transports } = require('winston');
const { combine, timestamp, label, printf } = format;
const http = require("http");

const logFormat = printf(info => {
  return `[${info.level.toUpperCase()}] ${info.timestamp} -> node: ${info.message.id}, temperature: ${info.message.temperature}, humidity: ${info.message.humidity}`;
});

const logger = createLogger({
      transports: [
          // - Write to all logs with level `info` and below to `combined.log`
          // - Write all logs error (and below) to `error.log`.
          new transports.File({ filename: 'error.log', level: 'error' }),
          new transports.File({ filename: 'combined.log', format: combine(
            timestamp(),
            logFormat
          )}),
          new transports.Console({ format: format.json(), level: 'debug' })
        ]
    });

rf24hub.from.on('message', function(value) {
  let data = JSON.parse(value);
  logger.info(data);
  postNodeData(data.id, '˚C', data.temperature);
  postNodeData(data.id, '%', data.humidity);
});

function postNodeData(node_id, unit_of_measurement, state) {
  let options = {
    hostname: 'http://127.0.0.1',
    port: 8123,
    path: `/states/sensor.node_${node_id}`,
    method: 'POST',
    headers: {
        'Content-Type': 'application/json',
    }
  };
  
  let req = http.request(options, function(res) {
    logger.debug(`Status: ${res.statusCode}`);
    logger.debug(`Headers: ${JSON.stringify(res.headers)}`);
    res.setEncoding('utf8');
    res.on('data', function (body) {
      logger.debug(`Body: ${body}`);
    });
  });
  
  req.on('error', function(e) {
    logger.error(`problem with request: ${e.message}`);
  });
  
  let data = {
    state: state,
    attributes: {
      unit_of_measurement: unit_of_measurement,
      friendly_name: getFriendlyNodeName(node_id)
    }
  }
  
  req.write(JSON.stringify(data));
  req.end();
}

function getFriendlyNodeName(node_id){
  switch (node_id) {
    case 1:
      return "Test apparaat";
      break;
    default:
      return `Node ${node_id}`;
      break;
  }
}