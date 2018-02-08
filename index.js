"use strict"
const path = require('path');
const worker = require('streaming-worker');
const addon_path = path.join(__dirname, 'build/Release/rf24hub');
const rf24hub = worker(addon_path);
const { createLogger, format, transports } = require('winston');
const { combine, timestamp, label, printf } = format;

const logFormat = printf(info => {
  return `${info.timestamp} - ${info.level}: ${info.message}`;
});

const logger = createLogger({
      level: 'info',
      format: combine(
          timestamp(),
          logFormat
      ),
      transports: [
          // - Write to all logs with level `info` and below to `combined.log`
          // - Write all logs error (and below) to `error.log`.
          new transports.File({ filename: 'error.log', level: 'error' }),
          new transports.File({ filename: 'combined.log' }),
          new transports.Console({ format: format.json() })
        ]
    });

rf24hub.from.on('message', function(value) {
  logger.info(value);
});
