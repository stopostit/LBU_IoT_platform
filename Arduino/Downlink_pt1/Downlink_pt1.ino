#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#define LEDPIN 7

void setup(){
	Serial.begin(9600);
	pinMode(LEDPIN, OUTPUT); 
}

void loop() {
	String Rcvbuff;
	StaticJsonBuffer<200> jsonBuffer;
	int value=0;
	if (Serial.available()  ) {
        Rcvbuff = Serial.readString();
  		JsonObject& root = jsonBuffer.parseObject(Rcvbuff);
        Serial.print("I received: ");
        Serial.println(Rcvbuff);
        Serial.print("Value: ");
        Serial.println((int)root["data"]);
        switch ((int)root["data"]) {
            case 1:
            	digitalWrite(LEDPIN, HIGH);
                Serial.println("ON");
              	break;
            case 0:
            	digitalWrite(LEDPIN, LOW);
                Serial.println("OFF");
              	break;
            default:
            	Serial.println("mauvaise entrée");
            	break;
        }
    }
}


// {"ID":"arduino1","time":1351824120,"data":1}