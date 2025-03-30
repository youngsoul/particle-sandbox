#include <math.h>

// used with the Grove Temperature v1.2
// https://www.seeedstudio.com/Grove-Temperature-Sensor.html


const int B=4275;                 // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A5

double temp = 0.0;

/* How it works:
    The TMP36 outputs a voltage proportional ot the current temperature.
    The resolution is 10 mv / degree centigrade (100 degrees centigrade / volt). There is a 500 mV offset (.5v) so negative temperatures can be measured
*/

// What pin on the Particle device is connected to the TMP36 VOUT pin (A4)
int sensorPin = A4;

// Need a global variable to hold the temperature
double tmp36temp = 0.0;

int use_tmp36 = 0;

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
    analogRead(sensorPin);
    delay(20);
    int rawVal = analogRead(sensorPin);

    // Convert the raw analog value to voltage
    float voltage = rawVal * 3.3; // First multiply by the input voltage
    voltage /= 4095.0;             // Then divide by the resolution of analogRead()

    // Convert the voltage to temperature
    double tmp36temp_c = (voltage - 0.5) * 100 ;  // (V - Offset) * 100 °C / V = °C
    tmp36temp = convertCtoF(tmp36temp_c);
}

void setup() {
    Serial.begin(9600);
    Particle.variable("temp", temp);
    Particle.variable("file", "GroveTempV1.2.ino");
    if(use_tmp36 == 1) {
        Particle.variable("tmp36temp", tmp36temp);
    }
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

        analogRead(pinTempSensor);
        delay(20);
        int a = analogRead(pinTempSensor );
        Serial.println(a);

        double R = 4095.0/((double)a)-1.0;
        Serial.println(R);
        R = 100000.0*R;
        Serial.println(R);

        temp=1.0/(log(R/100000.0)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
        temp = convertCtoF(temp);
        Serial.print("temperature = ");
        Serial.println(temp);
		if( Particle.connected()) {
			Particle.publish("temp", String(temp));
		}

        if(use_tmp36==1) {
            read_tmp36();
        }

    }

}