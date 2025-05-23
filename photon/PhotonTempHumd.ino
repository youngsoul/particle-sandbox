// This #include statement was automatically added by the Particle IDE.
#include <Grove_Temperature_And_Humidity_Sensor.h>

// This device is currently in the florida house garage.

SerialLogHandler logHandler;

DHT dht(D6, 32);
double temp=0.0;
double humidity=0.0;
int wifi_strength = -1;
double max_temp=0.0;
double min_temp=300.0;
double max_humidity=0.0;
double min_humidity=300.0;

void setup() {
    Serial.begin(9600);

    Particle.variable("temp", temp);
    Particle.variable("humidity", humidity);
    Particle.variable("wifi_strength", wifi_strength);
    Particle.variable("file", "photontemphumdity.ino");
    Particle.variable("max_temp", max_temp);
    Particle.variable("min_temp",min_temp);
    Particle.variable("max_humidity", max_humidity);
    Particle.variable("min_humidity", min_humidity);

    dht.begin();

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

    if( (timeNow - last_timestamp) > 10000 ) {
        // 10 seconds has passed
        last_timestamp = timeNow;

        temp = dht.getTempFarenheit();
        // look for new max temp
        if (temp > max_temp) {
            max_temp = temp;
        }


        // look for new min temp
        if (temp < min_temp) {
            min_temp = temp;
        }
        humidity = dht.getHumidity();
        // look for new max humidity
        if (humidity > max_humidity) {
            max_humidity = humidity;
        }

        // look for new min humidity
        if (humidity < min_humidity) {
            min_humidity = humidity;
        }

        if (isnan(humidity) || isnan(temp) )
	    {
		    Serial.printlnf("Failed to read from DHT11 sensor!");
	    }

        Serial.printlnf("Temp: %f", temp);
        Serial.printlnf("Humidity: %f", humidity);

        //Checks the strength
        int strength = WiFi.RSSI();

        //Converts the -127 to -1 strength range to 0-9
        wifi_strength = (((strength - -127) * 10) / (-1 - -127));


    }

}