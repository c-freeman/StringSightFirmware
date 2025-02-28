#include "PortSchema.h"


uint8_t portSchema::encodeSensorDataToPayload(sensorData *sensor_data, uint8_t *payload_buffer, uint8_t start_pos) {
    /* If the data should be included in the payload according to the portSchema, then encode the value.
     * he order of these if statements is the order the sensor data will be encoded into the payload and should match
     * the schema.
     * The buffsize is increased by the amount of data encoded in each step.
     */
    uint8_t payload_length = start_pos;
    if (sendBatteryVoltage) {
        payload_length = batteryVoltageSchema.encodeData(
            sensor_data->battery_mv.value, sensor_data->battery_mv.is_valid, payload_buffer, payload_length);
    }
    if (sendTemperature) {
        payload_length = temperatureSchema.encodeData(sensor_data->temperature.value, sensor_data->temperature.is_valid,
                                                      payload_buffer, payload_length);
    }
    if (sendRelativeHumidity) {
        payload_length = relativeHumiditySchema.encodeData(sensor_data->humidity.value, sensor_data->humidity.is_valid,
                                                           payload_buffer, payload_length);
    }
    if (sendAirPressure) {
        payload_length = airPressureSchema.encodeData(sensor_data->pressure.value, sensor_data->pressure.is_valid,
                                                      payload_buffer, payload_length);
    }
    if (sendGasResistance) {
        payload_length = gasResistanceSchema.encodeData(sensor_data->gas_resist.value, sensor_data->gas_resist.is_valid,
                                                        payload_buffer, payload_length);
    }
    if (sendLocation) {
        payload_length = locationSchema.encodeData(sensor_data->location.latitude, sensor_data->location.is_valid,
                                                   payload_buffer, payload_length);
        payload_length = locationSchema.encodeData(sensor_data->location.longitude, sensor_data->location.is_valid,
                                                   payload_buffer, payload_length);
    }
    if (sendCurrentSensor) {
        payload_length = currentSensorSchema.encodeData(
            sensor_data->current_A.value, sensor_data->current_A.is_valid, payload_buffer, payload_length);
        // Added to try send through raw ADC value
        payload_length = currentSensorSchema.encodeData(
            sensor_data->current_A.ADCval, sensor_data->current_A.is_valid, payload_buffer, payload_length);
    }
    return payload_length;
}

sensorData portSchema::decodePayloadToSensorData(uint8_t *buffer, uint8_t len, uint8_t start_pos) {
    sensorData sensor_data = {};
    uint8_t buff_pos = start_pos;

    if (sendBatteryVoltage && (buff_pos < len)) {
        buff_pos = batteryVoltageSchema.decodeData(&sensor_data.battery_mv.value, &sensor_data.battery_mv.is_valid, buffer, buff_pos);
    }
    if (sendTemperature && (buff_pos < len)) {
        buff_pos = temperatureSchema.decodeData(&sensor_data.temperature.value, &sensor_data.temperature.is_valid, buffer, buff_pos);
    }
    if (sendRelativeHumidity && (buff_pos < len)) {
        buff_pos = relativeHumiditySchema.decodeData(&sensor_data.humidity.value, &sensor_data.humidity.is_valid, buffer, buff_pos);
    }
    if (sendAirPressure && (buff_pos < len)) {
        buff_pos = airPressureSchema.decodeData(&sensor_data.pressure.value, &sensor_data.pressure.is_valid, buffer, buff_pos);
    }
    if (sendGasResistance && (buff_pos < len)) {
        buff_pos = gasResistanceSchema.decodeData(&sensor_data.gas_resist.value, &sensor_data.gas_resist.is_valid, buffer, buff_pos);
    }
    if (sendLocation && (buff_pos < len)) {
        buff_pos = locationSchema.decodeData(&sensor_data.location.latitude, &sensor_data.location.is_valid, buffer, buff_pos);
        buff_pos = locationSchema.decodeData(&sensor_data.location.longitude, &sensor_data.location.is_valid, buffer, buff_pos);
    }
    if (sendCurrentSensor && (buff_pos < len)) {
        buff_pos = currentSensorSchema.decodeData(&sensor_data.current_A.value, &sensor_data.current_A.is_valid, buffer, buff_pos);
        // added to send through ADC value
        buff_pos = currentSensorSchema.decodeData(&sensor_data.current_A.ADCval, &sensor_data.current_A.is_valid, buffer, buff_pos);
    }

    return sensor_data;
}

bool portSchema::operator==(const portSchema &port2) {
    // clang-format off
    return ((port_number          == port2.port_number         ) &&
            (sendBatteryVoltage   == port2.sendBatteryVoltage  ) &&
            (sendTemperature      == port2.sendTemperature     ) &&
            (sendRelativeHumidity == port2.sendRelativeHumidity) &&
            (sendAirPressure      == port2.sendAirPressure     ) &&
            (sendGasResistance    == port2.sendGasResistance   ) &&
            (sendLocation         == port2.sendLocation        ) &&
            (sendCurrentSensor    == port2.sendCurrentSensor   ));
    // clang-format on
}

portSchema &portSchema::operator+(const portSchema &port2) const {
    portSchema combined_port = PORTERROR;
    // clang-format off
    combined_port.port_number = 0;
    combined_port.sendBatteryVoltage   = (this->sendBatteryVoltage   || port2.sendBatteryVoltage  );
    combined_port.sendTemperature      = (this->sendTemperature      || port2.sendTemperature     );
    combined_port.sendRelativeHumidity = (this->sendRelativeHumidity || port2.sendRelativeHumidity);
    combined_port.sendAirPressure      = (this->sendAirPressure      || port2.sendAirPressure     );
    combined_port.sendGasResistance    = (this->sendGasResistance    || port2.sendGasResistance   );
    combined_port.sendLocation         = (this->sendLocation         || port2.sendLocation        );
    combined_port.sendCurrentSensor    = (this->sendCurrentSensor    || port2.sendCurrentSensor   );
    // clang-format on
    return combined_port;
}

portSchema getPort(uint8_t port_number) {
    switch (port_number) {
        case 1: {
            return PORT1;
        }
        case 2: {
            return PORT2;
        }
        case 3: {
            return PORT3;
        }
        case 4: {
            return PORT4;
        }
        case 5: {
            return PORT5;
        }
        case 6: {
            return PORT6;
        }
        case 7: {
            return PORT7;
        }
        case 8: {
            return PORT8;
        }
        case 9: {
            return PORT9;
        }
        case 10: {
            return PORT10;
        }
        case 11: {
            return PORT11;
        }
        case 50: {
            return PORT50;
        }
        case 51: {
            return PORT51;
        }
        case 52: {
            return PORT52;
        }
        case 53: {
            return PORT53;
        }
        case 54: {
            return PORT54;
        }
        case 55: {
            return PORT55;
        }
        case 56: {
            return PORT56;
        }
        case 57: {
            return PORT57;
        }
        case 58: {
            return PORT58;
        }
        case 59: {
            return PORT59;
        }
        default: {
            return PORTERROR;
        }
    }
}