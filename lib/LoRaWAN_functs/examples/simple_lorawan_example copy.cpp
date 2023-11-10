/**
 * @file low_power_lorawan_example.cpp
 * @author Kalina Knight
 * @brief A simple example of the LoRaWAN functions.
 * @version 0.1
 * @date 2021-08-27
 *
 * @copyright (c) 2021 Kalina Knight - MIT License
 */

#include <Arduino.h>
#include <LoRaWan-RAK4630.h> // Click to get library: https://platformio.org/lib/show/6601/SX126x-Arduino

#include "LoRaWAN_functs.h" /**< Go here to provide the OTAA keys & change the LoRaWAN settings. */
#include "Logging.h"        /**< Go here to change the logging level for the entire application. */
#include "OTAA_keys.h"      /**< Go here to set the OTAA keys (See README). */

//#include "SerialDataExporter.h"
#include <Wire.h>
#ifdef _VARIANT_RAK4630_
#include <Adafruit_TinyUSB.h>
#endif

        /* 0- General */

        int decimalPrecision = 4;                   // decimal places for all values shown in LED Display & Serial Monitor


        /* 1- AC Current Measurement */

        int CurrentAnalogInputPin = WB_A1;             // Which pin to measure Current Value (A0 is reserved for LCD Display Shield Button function)
        float mVperAmpValue = 31.25;                  // If using ACS712 current module : for 5A module key in 185, for 20A module key in 100, for 30A module key in 66
                                                    // If using "Hall-Effect" Current Transformer, key in value using this formula: mVperAmp = maximum voltage range (in milli volt) / current rating of CT
                                                    // For example, a 20A Hall-Effect Current Transformer rated at 20A, 2.5V +/- 0.625V, mVperAmp will be 625 mV / 20A = 31.25mV/A 
        float offsetSampleRead = 0;                 /* to read the value of a sample for offset purpose later */
        float currentSampleRead  = 0;               /* to read the value of a sample including currentOffset1 value*/
        float currentLastSample  = 0;               /* to count time for each sample. Technically 1 milli second 1 sample is taken */
        float currentSampleSum   = 0;               /* accumulation of sample readings */
        float currentSampleCount = 0;               /* to count number of sample. */
        float currentMean ;                         /* to calculate the average value from all samples, in analog values*/ 
        float RMSCurrentMean ;                      /* square roof of currentMean, in analog values */
        float adjustRMSCurrentMean ;                /* RMScurrentMean including currenOffset2, in analog values */    
        float FinalRMSCurrent ;                     /* the final RMS current reading*/
        float LoraLastTime = 0;

            /*1.1 Offset AC Current */
    
            float currentOffset1 = -1;                   // to Offset deviation and accuracy. Offset any fake current when no current operates. 
                                                        // Offset will automatically callibrate when SELECT Button on the LCD Display Shield is pressed.
                                                        // If you do not have LCD Display Shield, look into serial monitor to add or minus the value manually and key in here.
                                                        // 26 means add 26 to all analog value measured
            float currentOffset2 = -0.5;                   // to offset value due to calculation error from squared and square root.
      

// App payload interval value in [ms] = 30 seconds.
const int lorawan_app_interval = 30000;

// Buffer that payload data is placed in.
uint8_t payload_buffer[PAYLOAD_BUFFER_SIZE] = "Hello!";

// Struct that passes the payload data for a LoRaWAN frame.
lmh_app_data_t lorawan_payload = {
    payload_buffer, strlen("Hello!"), 1, 0, 0,
};

/**
 * @brief Setup code runs once on reset/startup.
 */
void setup() {
    // initialise the logging module - function does nothing if APP_LOG_LEVEL in Logging.h = NONE
    initLogging();
    log(LOG_LEVEL::INFO,
        "\n================================="
        "\nWelcome to Simple LoRaWAN Example"
        "\n=================================");

    /* WisBLOCK 5811 Power On*/
	pinMode(WB_IO1, OUTPUT);
	digitalWrite(WB_IO1, HIGH);
	/* WisBLOCK 5811 Power On*/
	pinMode(WB_IO2, OUTPUT);
	digitalWrite(WB_IO2, HIGH);

	pinMode(WB_A1, INPUT_PULLDOWN);
	analogReference(AR_INTERNAL_3_0);
	analogOversampling(128);

    // Init LoRaWAN
    if (!initLoRaWAN(OTAA_KEY_APP_EUI, OTAA_KEY_DEV_EUI, OTAA_KEY_APP_KEY)) {
        delay(1000);
        return;
    }

    // Attempt to join the network
    startLoRaWANJoinProcedure();

    // Not sure if need these
    LoraLastTime = millis();
    currentLastSample = micros();
}

/**
 * @brief Loop code runs repeated after setup().
 */
void loop() {
    
     if(micros() >= currentLastSample + 200)                                                                 /* every 0.2  milli second taking 1 reading */
          {
           offsetSampleRead = analogRead(CurrentAnalogInputPin)-512;                                          /* Read analog value. This is for offset purpose */ 
           currentSampleRead = analogRead(CurrentAnalogInputPin)-512 + currentOffset1;                        /* read the sample value including offset value*/
           currentSampleSum = currentSampleSum + sq(currentSampleRead) ;                                      /* accumulate total analog values for each sample readings*/
           
           currentSampleCount = currentSampleCount + 1;                                                       /* to count and move on to the next following count */  
           currentLastSample = micros();                                                                      /* to reset the time again so that next cycle can start again*/ 
          
          }
        
        if(currentSampleCount == 2000)                                                                        /* after 2000 count or 800 milli seconds (0.8 second), do this following codes*/
          {
            currentMean = currentSampleSum/currentSampleCount;                                                /* average accumulated analog values*/
            RMSCurrentMean = sqrt(currentMean);                                                               /* square root of the average value*/
            adjustRMSCurrentMean = RMSCurrentMean + currentOffset2;                                           /* square root of the average value including offset value */
            FinalRMSCurrent = (((adjustRMSCurrentMean /1024) *5000) /mVperAmpValue);                          /* calculate the final RMS current*/
            
            currentSampleSum =0;                                                                              /* to reset accumulate sample values for the next cycle */
            currentSampleCount=0;                                                                             /* to reset number of sample for the next cycle */
          }

          // Buffer that payload data is placed in.
        uint8_t payload_buffer[PAYLOAD_BUFFER_SIZE] = "Hello!";

        // Struct that passes the payload data for a LoRaWAN frame.
        lmh_app_data_t lorawan_payload = {
            payload_buffer, strlen("Hello!"), 1, 0, 0,
        };

    // every lorawan_app_interval milliseconds check if the device is connected
    if (millis() >= LoraLastTime + lorawan_app_interval) {
        if (isLoRaWANConnected()) {
            log(LOG_LEVEL::DEBUG, "Send payload");
            // send sendLoRaWANFrame will do nothing if not connected anyway, but it's best practice to check
            sendLoRaWANFrame(&lorawan_payload);
        } else {
            // else log that it's not connected
            log(LOG_LEVEL::DEBUG, "LoRaWAN not connected. Try again later.");
        }
        LoraLastTime = millis();
    }
    
}
