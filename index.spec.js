"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const index_1 = require("./index");
const chai_1 = require("chai");
require("mocha");
describe('getSensorsForType function', () => {
    it(`should return [SensorType.TEMPERATURE, SensorType.HUMIDITY]`, () => {
        const dht22_data = {
            type: 1,
            id: 1,
            data: {
                humidity: 0,
                temperature: 0
            }
        };
        const result = index_1.getSensorsForNodeType(dht22_data);
        chai_1.assert.isArray(result);
        chai_1.expect(result[0]).to.equal(index_1.SensorType.TEMPERATURE);
        chai_1.expect(result[1]).to.equal(index_1.SensorType.HUMIDITY);
    });
});
