"use strict";
// const worker = require('streaming-worker');
// const addon_path = path.join(__dirname, 'build/Release/rf24hub');
// const rf24hub = worker(addon_path);
Object.defineProperty(exports, "__esModule", { value: true });
// export interface RF24HubFrom {
//     callbacks:Array<any>,
//     intervalID:Number,
//     on:Function
// }
// export interface RF24Hub {
//     from:RF24HubFrom
// }
// const rf24hub:RF24Hub = {
//     from: {
//         callbacks:[],
//         intervalID:0,
//         on: (event:String, callback:Function):void => {
//             setInterval(() => { callback(`{"type":1, "id":1, "data":{"temperature": "21.2","humidity": "56.4"}}`); }, 1000);
//         }
//     }
// }
// PROGRAM
var NodeType;
(function (NodeType) {
    NodeType[NodeType["DHT22"] = 1] = "DHT22";
})(NodeType = exports.NodeType || (exports.NodeType = {}));
var SensorType;
(function (SensorType) {
    SensorType[SensorType["TEMPERATURE"] = 1] = "TEMPERATURE";
    SensorType[SensorType["HUMIDITY"] = 2] = "HUMIDITY";
})(SensorType = exports.SensorType || (exports.SensorType = {}));
// rf24hub.from.on('message', function(value) {
//   let data = JSON.parse(value);
//   let sensorDataFields = getSensorsForType(data);
// });
function getSensorData(nodeData) {
    let sensors = getSensorsForNodeType(nodeData);
    let sensorData = [];
    sensors.forEach((sensor) => {
        sensorData.push(nodeData.data[sensor]);
    });
}
function getSensorsForNodeType(nodeData) {
    switch (nodeData.type) {
        case NodeType.DHT22:
            return [SensorType.TEMPERATURE, SensorType.HUMIDITY];
    }
    return [];
}
exports.getSensorsForNodeType = getSensorsForNodeType;
