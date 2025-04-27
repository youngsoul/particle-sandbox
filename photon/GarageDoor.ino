#define RELAYPIN D6
bool garageDoorButtonPressed = false;


void doGarageDoor(String event, String data);

int uptime = 0;

void setup() {
    pinMode(RELAYPIN, OUTPUT);
    digitalWrite(RELAYPIN, 0);
    // Particle.subscribe("garagedoor", doGarageDoor, MY_DEVICES);
    Particle.variable("uptime", uptime);
    Particle.function("toggleGarageDoor", toggleGarageDoor);
}

void loop() {
    if( garageDoorButtonPressed == false ) {
        digitalWrite(RELAYPIN, 0);
    }
    delay(50);
}

int toggleGarageDoor(String ignored) {
    doGarageDoor("", "");
    return 1;
}

void doGarageDoor(String event, String data) {
    garageDoorButtonPressed = true;

    digitalWrite(RELAYPIN, 1);
    delay(800);
    digitalWrite(RELAYPIN, 0);

    garageDoorButtonPressed = false;

}