// const worker = require('streaming-worker');
// const addon_path = path.join(__dirname, 'build/Release/rf24hub');
// const rf24hub = worker(addon_path);

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

export enum NodeType {
    DHT22 = 1
}

export enum SensorType {
    TEMPERATURE = 1,
    HUMIDITY
}

export interface MessageData {
    type: NodeType,
    id: Number,
    data: Object
}

export interface Sensor {
    nodeID:Number,
    id:Number,
    type:SensorType,
    name:String,
}

// rf24hub.from.on('message', function(value) {
//   let data = JSON.parse(value);
//   let sensorDataFields = getSensorsForType(data);
// });

function getSensorData(nodeData:MessageData):Object {
    let sensors:Array<SensorType> = getSensorsForNodeType(nodeData);
    let sensorData:Array<Object> = [];
    sensors.forEach((sensor) => {
        sensorData.push(nodeData.data[sensor]);
    });
}

export function getSensorsForNodeType(nodeData:MessageData):Array<SensorType> {
    switch(nodeData.type) {
        case NodeType.DHT22:
            return [SensorType.TEMPERATURE, SensorType.HUMIDITY];
    }
    return [];
}