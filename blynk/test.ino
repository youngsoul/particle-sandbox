// this is a test
char auth[] = "****************5kBsPZqjHUJgH";

#include <blynk.h>
#define BLYNK_TEMPLATE_ID "TMPLsRJE8EmO"
#define BLYNK_DEVICE_NAME "24v motor amp sensor"


void setup()
{
  
  Serial.begin(9600);
  
  Blynk.begin(auth);
}

void loop()
{
  Blynk.run();
}
