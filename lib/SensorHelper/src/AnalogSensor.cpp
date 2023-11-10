#include "AnalogSensor.h"

AnalogSensor::AnalogSensor(uint8_t pin) {
    AnalogSensor(pin, DEFAULT_ANALOG_REFERENCE, DEFAULT_ANALOG_RESOLUTION);
}

AnalogSensor::AnalogSensor(uint8_t pin, _eAnalogReference analog_ref, int analog_resolution) {
    AnalogSensor(pin, analog_ref, analog_resolution, DEFAULT_OVERSAMPLING);
}

AnalogSensor::AnalogSensor(uint8_t pin, _eAnalogReference analog_ref, int analog_resolution, uint32_t oversampling) {
    this->pin = pin;
    this->analog_ref = analog_ref;
    this->analog_resolution = analog_resolution;
    this->oversampling = oversampling;
}

void AnalogSensor::ADCInit(uint8_t pin_mode) {
    pinMode(pin, pin_mode);
    setRealMVPerLSB();
    // Get a single ADC sample and throw it away
    getSensorMV();
}

void AnalogSensor::setCompensationFactor(float comp_factor) {
    this->compensation_factor = comp_factor;
    setRealMVPerLSB();
};

float AnalogSensor::getSensorMV(void) {
    // set the ADC params each time in case the adc is being used for multiple sensors
    analogReference(analog_ref);
    analogReadResolution(analog_resolution);
    analogOversampling(oversampling);

    // Let the ADC settle
    delay(1);

    // Get a raw ADC reading
    float sensor_mv = readMV();

    log(LOG_LEVEL::DEBUG, "ADC: %.2f mV", sensor_mv);

    return sensor_mv;
}

float AnalogSensor::readMV(void) {
    // Get the raw ADC value
    rawADC = analogRead(pin);

    // return converted raw ADC value
    //log(LOG_LEVEL::DEBUG, "raw value: %.2f, raw*real_MV_per_LSB %.2f", rawADC, rawADC * real_MV_per_LSB);

    return (rawADC * real_MV_per_LSB);
}

void AnalogSensor::setRealMVPerLSB() {
    float adc_analog_ref_mv = 0;

    switch (this->analog_ref) {
        case AR_DEFAULT:
            // same as case AR_INTERNAL
        case AR_INTERNAL: // 0.6V Ref * 6 = 0..3.6V
            adc_analog_ref_mv = 3600;
            break;
        case AR_INTERNAL_3_0: // 0.6V Ref * 5 = 0..3.0V
            adc_analog_ref_mv = 3000;
            break;
        case AR_INTERNAL_2_4: // 0.6V Ref * 4 = 0..2.4V
            adc_analog_ref_mv = 2400;
            break;
        case AR_INTERNAL_1_8: // 0.6V Ref * 3 = 0..1.8V
            adc_analog_ref_mv = 1800;
            break;
        case AR_INTERNAL_1_2: // 0.6V Ref * 2 = 0..1.6V
            adc_analog_ref_mv = 1600;
            break;
        case AR_VDD4: // 3.3V Ref / 4 = 0..0.825V
            adc_analog_ref_mv = 825;
            break;
        case AR_INTERNAL_5_0: // 0.6V Ref * 25/3 = 0..5.0V
            adc_analog_ref_mv = 5000;
            break;
        case AR_INTERNAL_3_3: // 0.6V Ref * 25/3 = 0..5.0V
            adc_analog_ref_mv = 3300;
            break;
    }

    real_MV_per_LSB = compensation_factor * (adc_analog_ref_mv / pow(2, analog_resolution));
}

void BatteryLevel::ADCInit(void) {
    setCompensationFactor(BATTERY_COMPENSATION_FACTOR);
    // Get a single ADC sample and throw it away
    getSensorMV();
}

// Lookup table converting battery voltage (mV) to SoC. Taken from standard 0.2C 3.7V LiPo discharge curve.
#define SOC_MV_LOOKUP_SIZE 11 // 0-100% with each value incrementing 10%
const float soc_mv_lookup[SOC_MV_LOOKUP_SIZE] = {
    3300.0, // mv = 0%
    3584.0, // mv = 10%
    3678.0, // mv = 20%
    3725.0, // mv = 30%
    3748.0, // mv = 40%
    3775.0, // mv = 50%
    3815.0, // mv = 60%
    3873.0, // mv = 70%
    3951.0, // mv = 80%
    4036.0, // mv = 90%
    4200.0  // mv = 100%
};

float BatteryLevel::mvToSoC(float mvolts) {
    float high_mv = soc_mv_lookup[SOC_MV_LOOKUP_SIZE - 1]; // 100%
    float low_mv = soc_mv_lookup[SOC_MV_LOOKUP_SIZE - 2];  // 90%
    float vbat_soc = 100.0;
    int i = 1;
    for (; i < SOC_MV_LOOKUP_SIZE; i++) {
        if (mvolts <= soc_mv_lookup[i]) {
            // find the range to interpolate between
            high_mv = soc_mv_lookup[i];
            low_mv = soc_mv_lookup[i - 1];
            // vbat_soc is atleast as big as the low SoC
            vbat_soc = (float)(i - 1) * 10.0F;
            break;
        }
    }
    vbat_soc += (mvolts - low_mv) * 10.0F / (high_mv - low_mv); // linear interpolation of the range

    // correct range
    if (vbat_soc < 0) {
        vbat_soc = 0.0;
    } else if (vbat_soc > 100.0) {
        vbat_soc = 100.0;
    }

    log(LOG_LEVEL::DEBUG, "LIPO: %.2f mV = %.2f%%", mvolts, vbat_soc);
    return vbat_soc;
}

void CurrentSensor::PowerOff(){
    /* WisBLOCK 12V 5811 Power Off*/
	pinMode(WB_IO1, OUTPUT);
	digitalWrite(WB_IO1, LOW);
	/* WisBLOCK 3.3 V 5811 Power Off*/
	pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, LOW);
}

void CurrentSensor::PowerOn(){
    /* WisBLOCK 12V 5811 Power On*/
	pinMode(WB_IO1, OUTPUT);
	digitalWrite(WB_IO1, HIGH);
	/* WisBLOCK 3.3 V 5811 Power On*/
	pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, HIGH);
}

void CurrentSensor::ADCInit(uint8_t pin_mode) {
    pinMode(pin, pin_mode);
    /* WisBLOCK 12V 5811 Power On*/
	pinMode(WB_IO1, OUTPUT);
	digitalWrite(WB_IO1, HIGH);
	/* WisBLOCK 3.3 V 5811 Power On*/
	pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, HIGH);

    // Currently set to 1/0.6
    setCompensationFactor(CURRENT_SENSOR_COMPENSATION_FACTOR);
    // Get a single ADC sample and throw it away
    getSensorMV();
}

 bool CurrentSensor::currentSensorCalibrationMode() {
    return current_sensor_zero_calibrate_mode;
 }

// Calibrates sensor to remove the zero offset
 void CurrentSensor::zeroCurrentOffsetCalibration() {
    // For number of samples creates a sum of current sensor mv value
    for (int i = 0; i < numberOfSamples; i++)                                                               
        {
        current_sensor_mV = readMV();  // mV offset 
        current_sensor_mV_sum = current_sensor_mV_sum + current_sensor_mV;                                      
        }
    
    current_sample_mv = current_sensor_mV_sum/numberOfSamples;

    log(LOG_LEVEL::DEBUG, "Current sample mv = %.2f%% mV", current_sample_mv);

    zeroCurrentOffset = 2500 - current_sample_mv; 
    current_sensor_mV_sum = 0;

    log(LOG_LEVEL::DEBUG, "Zero current offset = %.2f%% mV", zeroCurrentOffset);
 }


float CurrentSensor::readCurrentAmp() {

    // analogReference(analog_ref);
    // analogReadResolution(analog_resolution);
    // analogOversampling(oversampling);

       for (int i = 0; i < numberOfSamples; i++)                                                               
          {
            current_sensor_mV = readMV() + zeroCurrentOffset;  // mV offset 
            currentSensorADCvalSum = rawADC + currentSensorADCvalSum;
            currentSampleRead = (current_sensor_mV - 2500)*0.032; // 0.032; // 625 mV / 20 A = 31.25, 1/31.25 = 0.032
            currentSampleSum = currentSampleSum + currentSampleRead ;                                      
          }
        
        ADCaverage = currentSensorADCvalSum/numberOfSamples;
        currentSample = currentSampleSum/numberOfSamples;
        currentSampleSum =0;
        currentSensorADCvalSum = 0;
    log(LOG_LEVEL::DEBUG, "ADC average value = %.2f%% ", ADCaverage);
    log(LOG_LEVEL::DEBUG, "Current Sensor value = %.2f%% A", currentSample);

    return currentSample;
}
