"use strict"
const path = require('path');
const worker = require('streaming-worker');
const addon_path = path.join(__dirname, 'build/Release/rf24hub');
const rf24hub = worker(addon_path);

rf24hub.from.on('message', function(value) {
  console.log('int: ' + value);
});
