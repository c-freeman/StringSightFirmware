/**
 * @file AnalogSensor.h
 * @author Kalina Knight
 * @brief Class that uses the onboard ADC to read an analog sensor.
 * If using the RAK5811 board extension then set the compensation factor to 1/0.6.
 * @version 0.1
 * @date 2021-09-10
 *
 * @copyright (c) 2021 Kalina Knight - MIT License
 */

#include <LoRaWan-RAK4630.h> // Click to get library: https://platformio.org/lib/show/6601/SX126x-Arduino

#include "Logging.h" /**< Go here to change the logging level for the entire application. */

// Added ///
#include "SerialDataExporter.h"
#include <Arduino.h>
#include <Wire.h>
#ifdef _VARIANT_RAK4630_
#include <Adafruit_TinyUSB.h>
#endif

#define NO_OF_SAMPLES 32


static const _eAnalogReference DEFAULT_ANALOG_REFERENCE = AR_DEFAULT; // Analog reference to default = 3.6V.
static const int DEFAULT_ANALOG_RESOLUTION = 10;                      // Resolution to default 10-bit (0..4095).
static const uint32_t DEFAULT_OVERSAMPLING = 0;                       // Oversampling disabled

/**
 * @brief AnalogSensor uses the onboard ADC to find the voltage of an analog sensor.
 */
class AnalogSensor {
  public:
    /**
     * @brief Construct a new AnalogSensor object with default ADC values.
     */
    AnalogSensor(uint8_t pin);

    /**
     * @brief Construct a new AnalogSensor object with default oversampling.
     * @param pin Sensor pin number.
     * @param analog_ref ADC analog reference.
     * @param analog_resolution ADC resolution.
     */
    AnalogSensor(uint8_t pin, _eAnalogReference analog_ref, int analog_resolution);

    /**
     * @brief Construct a new AnalogSensor object.
     * @param pin Sensor pin number.
     * @param analog_ref ADC analog reference.
     * @param analog_resolution ADC resolution.
     * @param oversampling ADC oversampling setting.
     */
    AnalogSensor(uint8_t pin, _eAnalogReference analog_ref, int analog_resolution, uint32_t oversampling);

    /**
     * @brief Set up ADC to take readings.
     * @param pin_mode Pin mode of sensor pin (Default: INPUT).
     */
    void ADCInit(uint8_t pin_mode = INPUT);

    /**
     * @brief Set the Compensation Factor for the sensor/pin if there is one & update real_MV_per_LSB.
     * e.g. If using the RAK5811 then pass comp_factor = (1 / 0.6).
     * @param comp_factor New compensation factor.
     */
    void setCompensationFactor(float comp_factor);

    /**
     * @brief Get the sensor reading.
     * Uses the ADC parameters passed in object instantiation.
     * @return Sensor reading in mV.
     */
    float getSensorMV(void);

  //private:
    /**
     * @brief Read sensor voltage.
     * @return Reading in mV.
     */
    float readMV(void);

    /**
     * @brief Conversion factor that turns the raw ADC reading into sensor voltage (in mV).
     * Includes the compensation factor!!!
     */
    void setRealMVPerLSB();

    uint8_t pin;                   // Sensor pin number
    _eAnalogReference analog_ref;  // ADC analog reference
    int analog_resolution;         // ADC resolution
    uint32_t oversampling;         // ADC oversampling
    float compensation_factor = 1; // Compensation factor sensor/pin - depends on the board hardware.
    float real_MV_per_LSB;         // Conversion factor that turns the raw ADC reading into the voltage
    float rawADC = 0;
};

static const uint8_t BATTERY_PIN = WB_A0;
static const float BATTERY_COMPENSATION_FACTOR = 1.73; // Compensation factor for the VBAT divider - depends on the board.

/**
 * @brief BatteryLevel inherits the AnalogSensor class adding an SoC function for sending via LoRaWAN.
 */
class BatteryLevel : public AnalogSensor {
  public:
    /**
     * @brief Construct a new Battery Level object with default ADC values.
     * pin = BATTERY_PIN.
     * analog_ref = 3.0V (default = 3.6V).
     * analog_resolution = 12-bit (0..4095).
     * oversampling = DEFAULT_OVERSAMPLING.
     */
    BatteryLevel(void) : AnalogSensor(BATTERY_PIN, AR_INTERNAL_3_0, 12, DEFAULT_OVERSAMPLING){};

    /**
     * @brief Construct a new Battery Level object.
     * pin = BATTERY_PIN.
     * oversampling = DEFAULT_OVERSAMPLING.
     * @param analog_ref ADC analog reference.
     * @param analog_resolution ADC resolution.
     */
    BatteryLevel(_eAnalogReference analog_ref, int analog_resolution)
        : AnalogSensor(BATTERY_PIN, analog_ref, analog_resolution, DEFAULT_OVERSAMPLING){};

    /**
     * @brief Gets the ADC ready by setting the compensation factor to BATTERY_COMPENSATION_FACTOR.
     */
    void ADCInit(void);

    /**
     * @brief @brief Convert from mV to battery SoC.
     * Uses a lookup table created from the standard 0.2C 3.7V LiPo discharge curve.
     * Update table in Analog.cpp if a more suitable fit curve is known.
     * @param mvolts Battery voltage (mV).
     * @return Battery SoC (%).
     */
    float mvToSoC(float mvolts);
};

static const uint8_t CURRENT_SENSOR_PIN = WB_A1;
static const float CURRENT_SENSOR_COMPENSATION_FACTOR = 1/0.6; 

/**
 * @brief CurrentSensor inherits the AnalogSensor class adding an SoC function for sending via LoRaWAN.
 */
class CurrentSensor : public AnalogSensor {
  public:
    /**
     * @brief Construct a new Current Sensor object with default ADC values.
     * pin = CURRENT_SENSOR_PIN.
     * analog_ref = 3.0V (default = 3.6V).
     * analog_resolution = 10-bit (0-1023) // 12 bit would be (0..4095).
     * oversampling = 128 // DEFAULT_OVERSAMPLING. 
     */
    CurrentSensor(void) : AnalogSensor(CURRENT_SENSOR_PIN, AR_INTERNAL_3_0, 12, 128){};

    /**
     * @brief Construct a new Current Sensor object.
     * pin = CURRENT_SENSOR_PIN.
     * oversampling = DEFAULT_OVERSAMPLING.
     * @param analog_ref ADC analog reference.
     * @param analog_resolution ADC resolution.
     */
    CurrentSensor(_eAnalogReference analog_ref, int analog_resolution)
        : AnalogSensor(CURRENT_SENSOR_PIN, analog_ref, analog_resolution, 128){};

    /**
     * @brief Gets the ADC ready by setting the compensation factor to CURRENT_SENSOR_COMPENSATION_FACTOR.
     */

    void PowerOff();
    void PowerOn();

    void ADCInit(uint8_t pin_mode);

    bool currentSensorCalibrationMode();
    
    void zeroCurrentOffsetCalibration();

    /**
     * @brief  Read Sensor value and convert from mV to CURRENT SENSOR Amp.
     * Uses 
     * @return CURRENT SENSOR Amp value.
     */
    float readCurrentAmp();

    float zeroCurrentOffset = 0;                /* for zeroing current calibration */

    int numberOfSamples = 2000;
    float currentSample = 0;
    float current_sensor_mV = 0;
    float current_sensor_mV_sum = 0;            /* for zeroing current calibration */
    float current_sample_mv = 0;                /* for zeroing current calibration */
    float currentSampleRead  = 0;               /* to read the value of a sample */
    float currentSampleSum   = 0;               /* accumulation of sample readings */
    float currentSensorADCvalSum = 0;
    float ADCaverage = 0;

    // Enable this calibration mode if you want to calibrate the current sensor to with zero current while initalising
  bool current_sensor_zero_calibrate_mode = 1;
};