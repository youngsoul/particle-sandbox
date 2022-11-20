#include <math.h>

// used with the Grove Temperature v1.2
// https://www.seeedstudio.com/Grove-Temperature-Sensor.html




/* How it works:
    The TMP36 outputs a voltage proportional ot the current temperature.
    The resolution is 10 mv / degree centigrade (100 degrees centigrade / volt). There is a 500 mV offset (.5v) so negative temperatures can be measured
*/

// What pin on the Particle device is connected to the TMP36 VOUT pin (A4)
const int pinTempSensorA0 = A0;     // Grove - Temperature Sensor connect to A5
const int pinTempSensorA4 = A4;

// Need a global variable to hold the temperature
double tempA4 = 0.0;
double tempA0 = 0.0;
int wifi_strength = -1;


double convertCtoF(double c)
{
	return c * 9.0 / 5.0 + 32;
}

void read_tmp36() {
    // https://gist.github.com/harrisonhjones/f83adc50c81fa7622bb2
    // Grab the raw analog value
    // https://forums.adafruit.com/viewtopic.php?f=25&t=11597
    // some micro contollers have a single ADC for all of the inputs and when you
    // read from difference analog inputs, it takes a moment to do the multiplex
    // switch.
    // There, read the sensor value once, to initiate the switch, wait, then actually read it.

		//------------------------------  A4 sensor
    analogRead(pinTempSensorA4);
    delay(20);
    int rawVal = analogRead(pinTempSensorA4);

    // Convert the raw analog value to voltage
    float voltage = rawVal * 3.3; // First multiply by the input voltage
    voltage /= 4095.0;             // Then divide by the resolution of analogRead()

    // Convert the voltage to temperature
    double tempA4_c = (voltage - 0.5) * 100 ;  // (V - Offset) * 100 °C / V = °C
    tempA4 = convertCtoF(tempA4_c);

		//------------------------------  A0 Sensor
		analogRead(pinTempSensorA0);
    delay(20);
    rawVal = analogRead(pinTempSensorA0);

    // Convert the raw analog value to voltage
    voltage = rawVal * 3.3; // First multiply by the input voltage
    voltage /= 4095.0;             // Then divide by the resolution of analogRead()

    // Convert the voltage to temperature
    double tempA0_c = (voltage - 0.5) * 100 ;  // (V - Offset) * 100 °C / V = °C
    tempA0 = convertCtoF(tempA0_c);


}

void setup() {
    Particle.variable("file", "DualTMP36Sensors.ino");
		Particle.variable("tempA4", tempA4);
		Particle.variable("tempA0", tempA0);
		Particle.variable("wifi", wifi_strength);

}

unsigned long last_timestamp = 0;

void loop() {
	unsigned long timeNow = millis();
	if( timeNow < last_timestamp ) {
			// then the millis() have rolled over, reset
			// the last_timestamp
			// https://docs.particle.io/reference/device-os/api/time/time/
			// the number returned will overflow (go back to zero ) after about 49 days.
			last_timestamp = 0;
	}

	if( (timeNow - last_timestamp) > 300000 ) { // every 5 minutes

		last_timestamp = timeNow;

  	read_tmp36();
		//Checks the strength
		int strength = WiFi.RSSI();

		//Converts the -127 to -1 strength range to 0-9
		wifi_strength = (((strength - -127) * 10) / (-1 - -127));
		if( Particle.connected()) {
			Particle.publish("tempA0", String(tempA0));
			Particle.publish("tempA4", String(tempA4));
			Particle.publish("wifi", String(wifi_strength));
		}

	}

}
