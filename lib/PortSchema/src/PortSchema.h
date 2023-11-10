#ifndef PORT_SCHEMA_H
#define PORT_SCHEMA_H

/**
 * @file PortSchema.h
 * @author Kalina Knight (kalina.knight77@gmail.com)
 * @brief Port schema definition as descibed the README.
 * Schema's include the functions for encoding and decoding the data to the payload as well.
 *
 * @version 0.1
 * @date 2021-08-24
 *
 * @copyright (c) 2021 Kalina Knight - MIT License
 */

#include "SensorPortSchema.h" /**< Go here for the individual sensor schema definitions. */

/** @brief portSchema describes which sensor data to include in each port and hence the payload. */
struct portSchema {
    uint8_t port_number;

    /**< Flags for if the sensors data is included in this port. */
    bool sendBatteryVoltage;
    bool sendTemperature;
    bool sendRelativeHumidity;
    bool sendAirPressure;
    bool sendGasResistance;
    bool sendLocation;
    bool sendCurrentSensor;
    /* An example of a new sensor:
    bool sendNewSensor;
    */

    /**
     * @brief Encodes the given sensor data into the payload according to the port's schema.
     * Calls sensorPortSchema::encodeData for each sensor.
     * @param sensor_data Sensor data to be encoded.
     * @param payload_buffer Payload buffer for data to be written into.
     * @param start_pos Start encoding data at this byte. Defaults to 0.
     * @return Total length of data encoded to payload_buffer.
     */
    uint8_t encodeSensorDataToPayload(sensorData *sensor_data, uint8_t *payload_buffer, uint8_t start_pos = 0);

    /**
     * @brief Decodes the given payload into the sensor data according to the port's schema.
     * Calls sensorPortSchema::decodeData for each sensor.
     * @param buffer Payload buffer to be decoded.
     * @param len Length of payload buffer.
     * @param start_pos Start decoding data at this byte. Defaults to 0.
     * @return Decoded sensor data.
     */
    sensorData decodePayloadToSensorData(uint8_t *buffer, uint8_t len, uint8_t start_pos = 0);

    /**
     * @brief Compares for full equivalence between two port objects.
     *
     * @param port2 Second port that this port is compared to.
     * @return True if they're equivalent, false if not.
     */
    bool operator==(const portSchema &port2);

    /**
     * @brief Combines two ports into separate port.
     * The port number is set to 0, and the send sensor flags are ||'ed.
     * Useful for sensor initiatlisation if using the port definition for this purpose.
     *
     * @param port2 Second port that this port is combined with.
     * @return Another port schema object that combines the given ports.
     */
    portSchema &operator+(const portSchema &port2) const;
};

/**
 * @brief Get the Port object for the given port number.
 * @param port_number
 * @return Returns the portSchema.
 */
portSchema getPort(uint8_t port_number);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SCHEMA DEFINITIONS: See readme for definitions in tabular format.

const portSchema PORTERROR = {
    __UINT8_MAX__, // port_number
    false,         // sendBatteryVoltage
    false,         // sendTemperature
    false,         // sendRelativeHumidity
    false,         // sendAirPressure
    false,         // sendGasResistance
    false,          // sendLocation
    false          // sendCurrentSensor
};

const portSchema PORT1 = {
    1,     // port_number
    true,  // sendBatteryVoltage
    false, // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT2 = {
    2,     // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT3 = {
    3,     // port_number
    true,  // sendBatteryVoltage
    true,  // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT4 = {
    4,     // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT5 = {
    5,     // port_number
    true,  // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT6 = {
    6,     // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    true,  // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};
const portSchema PORT7 = {
    7,     // port_number
    true,  // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    true,  // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT8 = {
    8,     // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    true,  // sendAirPressure
    true,  // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT9 = {
    9,    // port_number
    true, // sendBatteryVoltage
    true, // sendTemperature
    true, // sendRelativeHumidity
    true, // sendAirPressure
    true, // sendGasResistance
    false, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT10 = {
    10,     // port_number
    false,  // sendBatteryVoltage
    false, // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    true  // sendCurrentSensor
};

const portSchema PORT11 = {
    11,     // port_number
    true,  // sendBatteryVoltage
    false, // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false, // sendLocation
    true  // sendCurrentSensor
};

const portSchema PORT50 = {
    50,    // port_number
    false, // sendBatteryVoltage
    false, // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT51 = {
    51,    // port_number
    true,  // sendBatteryVoltage
    false, // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    true, // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT52 = {
    52,    // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT53 = {
    53,    // port_number
    true,  // sendBatteryVoltage
    true,  // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT54 = {
    54,    // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT55 = {
    55,    // port_number
    true,  // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT56 = {
    56,    // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    true,  // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};
const portSchema PORT57 = {
    57,    // port_number
    true,  // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    true,  // sendAirPressure
    false, // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT58 = {
    58,    // port_number
    false, // sendBatteryVoltage
    true,  // sendTemperature
    true,  // sendRelativeHumidity
    true,  // sendAirPressure
    true,  // sendGasResistance
    true,   // sendLocation
    false  // sendCurrentSensor
};

const portSchema PORT59 = {
    59,   // port_number
    true, // sendBatteryVoltage
    true, // sendTemperature
    true, // sendRelativeHumidity
    true, // sendAirPressure
    true, // sendGasResistance
    true, // sendLocation
    false // sendCurrentSensor
};



/* An example of a new port:
const portSchema PORTX = {
    X,     // port_number
    false, // sendBatteryVoltage
    false, // sendTemperature
    false, // sendRelativeHumidity
    false, // sendAirPressure
    false, // sendGasResistance
    false  // sendLocation
};
*/

#endif // PORT_SCHEMA_H