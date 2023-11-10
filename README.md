# StringSight Firmware

## How to use this Repo

This repo contains a collection of four libraries in [lib](./lib) to aid in the development of firmware for a Rak WisBlock based device.

First you'll want to [set up your code environment](#environment-setup) so you can write, compile and flash the devices.

Then the best places to start are the README's of the libraries and the [example that combines them](#working-combined-example/).

Useful links:

- [Log Library](./lib/Logging/)
- [LoRaWAN Library](./lib/LoRaWAN_functs/) that puts all the basic LoRaWAN functions into one place
- [Port Schema Library](./lib/PortSchema) implements a LoRaWAN Port Schema design for encoding payload data
- [Sensor Helper Library](./lib/SensorHelper/) for reading Rak WisBlock and other sensors
- [Combined firmware example](./examples/Combined_lib_example/) that is a good leaping off point for further firmware development with the libraries

## Environment Setup

First you'll need to [install VS Code](https://code.visualstudio.com/Download) - if you don't have it already. It's an excellent text editor, and probably the only truely decent thing to come out of Microsoft.

Then you'll want to follow [this](https://docs.rakwireless.com/Knowledge-Hub/Learn/Board-Support-Package-Installation-in-PlatformIO/) guide to setup Platform IO and install the necessary files for _LoRaWan-RAK4630.h_. If the link in step 6 is broken, use [this](https://github.com/RAKWireless/WisBlock) one.

> NOTE: Kalina has found that the wiscore files may need to be re-copied after an update to PIO.

Then depending on what you're doing you can either:

- (Recommended) Clone the repo and create your own branch to work on. You can do this directly in VS code by going to Source Control on the left (Crtl + Shift + G).
- [Fork](https://docs.github.com/en/get-started/quickstart/fork-a-repo) this one to break off and continue development separately.
- (Only recommended if just flashing the example code) Download this repo's libraries and create your own PIO project from scratch. NOTE: that the libraries should still be placed under the lib folder so they compile correctly.

Then go to PIO Home and Open the firmware folder as a project (if cloning or forking the repo), to use the PIO extension to compile and flash the code.

Then make sure you add any libraries to the project that you need, by going to PIO Home -> Libraries then you can search for and install them, e.g.:

- "SX126x-Arduino"
- "SparkFun SHTC3 Humidity and Temperature Sensor Library" needed for RAK1901
- "Adafruit BME680 Library" needed for RAK1906

You should then be ready to start using the [libraries](./lib/) from this repo and writing code in the src/main.cpp file.

## Examples

Kalina would recommend starting with the lib/LoRaWAN_functs/examples/simple_lorawan_example.cpp to get the LoRaWAN module set up.

Just **copy the example code into your main.cpp** and follow the [instructions](./lib/LoRaWAN_functs/#otaa-keys) to set up your OTAA keys (remember they need to be stored in the same directory as LoRaWAN_functs.h).

Then move onto the example below.

### Working Combined Firmware Example

Kalina has provided an [example](./examples/Combined_lib_example/) that that combines the four libraries, along with the Semaphore module that allows the chips to achieve their low power states. This is the example Kalina would base further development of WisBlock firmware off of. It is based upon this example that StringSight's firmware has been developed.
