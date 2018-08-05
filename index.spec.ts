import { getSensorsForNodeType, MessageData, SensorType } from './index';
import { expect, assert } from 'chai';
import 'mocha';

describe('getSensorsForType function', () => {
    it(`should return [SensorType.TEMPERATURE, SensorType.HUMIDITY]`, () => {
      const dht22_data:MessageData = {
        type: 1,
        id: 1,
        data: {
          humidity:0,
          temperature: 0
        }
      }
      const result = getSensorsForNodeType(dht22_data);
      assert.isArray(result);
      expect(result[0]).to.equal(SensorType.TEMPERATURE);
      expect(result[1]).to.equal(SensorType.HUMIDITY);
    });
});