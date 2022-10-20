#include <math.h>

// used with the Grove Temperature v1.2
// https://www.seeedstudio.com/Grove-Temperature-Sensor.html


const int B=4275;                 // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = A0;     // Grove - Temperature Sensor connect to A5

double temp = 0.0;

double convertCtoF(double c)
{
	return c * 9.0 / 5.0 + 32;
}


void setup() {
    Serial.begin(9600);
    Particle.variable("temp", temp);
    Particle.variable("file", "GroveTempV1.2.ino");


}

void loop() {
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

    delay(2000);


}