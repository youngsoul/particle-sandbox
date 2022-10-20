// This #include statement was automatically added by the Particle IDE.
#include <Grove_4Digit_Display.h>

// This #include statement was automatically added by the Particle IDE.
#include <Grove_Temperature_And_Humidity_Sensor.h>

// This #include statement was automatically added by the Particle IDE.
// get values from private file
#define BLYNK_TEMPLATE_ID "TMPLIRdM85MP"
#define BLYNK_DEVICE_NAME "Argon"
#define BLYNK_AUTH_TOKEN "dh3Yp2sbgqO0QY-wWlGIpeJnlm0WUtBo"

#include <blynk.h>

// This function creates the timer object. It's part of Blynk library
BlynkTimer timer;

SerialLogHandler logHandler;

DHT dht(D2);
int photoresistor = 0;
double temp=0.0;
double humidity=0.0;
int wifi_strength = -1;

#define DISPLAY_CLK D4
#define DISPLAY_DIO D5

int use_photoresistor = 1;


TM1637 tm1637(DISPLAY_CLK, DISPLAY_DIO);



void updateBlynk()
{
    // This function describes what will happen with each timer tick
    // e.g. writing sensor value to datastream V5
    int blynk_temp = (int)temp*10;
    Blynk.virtualWrite(V4, blynk_temp/10.0);
}

void setup() {

    Serial.begin(9600);
    Blynk.begin(BLYNK_AUTH_TOKEN);

    timer.setInterval(10000L, updateBlynk);

    Particle.variable("temp", temp);
    Particle.variable("humidity", humidity);
    if(use_photoresistor == 1) {
        Particle.variable("light", photoresistor);
    }

    Particle.variable("wifi", wifi_strength);
    Particle.variable("file", "ArgonBlynk.ino");

    dht.begin();

    tm1637.init();
    tm1637.set(BRIGHT_TYPICAL);
    tm1637.point(POINT_ON);

}

unsigned long last_timestamp = 0;

int what_to_display_flag = 0; // 0-temp, 1-wifi
int max_display_values = 2;

void loop() {

    unsigned long timeNow = millis();
    int8_t ListDisp[4];

    Blynk.run();
    timer.run();

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

        double new_temp = dht.getTempFarenheit();
        if(!isnan(new_temp)) {
            temp = new_temp;
        }
        humidity = dht.getHumidity();

        if(use_photoresistor == 1) {
            photoresistor = analogRead(A0);  // analogRead values go from 0 to 4095, analogWrite values from 0 to 255
            Serial.printlnf("Photo: %d", photoresistor);
        }


        Serial.printlnf("Temp: %f", temp);
        Serial.printlnf("Humidity: %f", humidity);

        //Checks the strength
        int strength = WiFi.RSSI();

        //Converts the -127 to -1 strength range to 0-9
        wifi_strength = (((strength - -127) * 10) / (-1 - -127));

        if( what_to_display_flag==1) {
            tm1637.display(0,0);
            tm1637.display(1,0);
            tm1637.display(2,0);
            tm1637.display(3,wifi_strength);
            what_to_display_flag += 1;
        }


        if(what_to_display_flag==0) {
            // format temp for display
            int display_temp = int(temp*100.0);
            for(int x=0; x<4; x++) {
                ListDisp[x] = display_temp % 10;
                display_temp = int(display_temp/10);
            }
            tm1637.display(0,ListDisp[3]);
            tm1637.display(1,ListDisp[2]);
            tm1637.display(2,ListDisp[1]);
            tm1637.display(3,ListDisp[0]);
            what_to_display_flag += 1;

        }

        if( what_to_display_flag >= max_display_values) {
            what_to_display_flag = 0;
        }


    }

}