#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <DHT.h>

#include <SPI.h>
#include <RH_RF95.h>
#include <Time.h>


#define DHTPIN A1 
#define DHTTYPE DHT11 
#define SOUNDPIN A0
#define LIGHTPIN A5
#define LEDPIN 7

#define PERIOD 5000
#define TIMEOUT 4500


RH_RF95 rf95;
float frequency = 433.0;
uint8_t ID[]="arduino2"; // must be replaced if not using the arduino number 2

DHT dht(DHTPIN, DHTTYPE);


void setup(){
    Serial.begin(9600);
    Serial.println("Start LoRa Client");
    if (!rf95.init())
        Serial.println("init failed");
    rf95.setFrequency(frequency);
    rf95.setTxPower(13);
    pinMode(LEDPIN, OUTPUT); 
}

void send(){
    /* Data collection */
    uint8_t json[RH_RF95_MAX_MESSAGE_LEN];
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float s = analogRead(SOUNDPIN);
    float lightsensor = analogRead(LIGHTPIN);
    float voltage = lightsensor * (5.0/1023) * 1000;
    float resistance = 10000 * ( voltage / ( 5000.0 - voltage) ); 
    float Lux = ( 1000000 - resistance ) / 1998.4;
    int tps=micros();

    /*
        Creation of nested json objects one in another
        "arduino2" must be modified when an other arduino is concerned 
    */
    StaticJsonBuffer<200> jsonsendBuffer;
    JsonArray& root = jsonsendBuffer.createArray();
    JsonObject& send = root.createNestedObject().createNestedObject("send");
    JsonObject& arduino2=send.createNestedObject("arduino2");
    JsonObject& Temperature=arduino2.createNestedObject("Temperature");
    JsonObject& Humidity=arduino2.createNestedObject("Humidity");
    JsonObject& Light=arduino2.createNestedObject("Light");
    JsonObject& Sound=arduino2.createNestedObject("Sound");

    /*
        Filling od each object with timestamp and corresponding data
        For Humidity and temperature, we integrate the "NaN test" which
        indicates a fail in the DHT reading. If so, we send an string instead 
        of sanding NaN as a number. Doing so prevents errors on the server side.
    */
    Temperature["time"]=tps;
    if (isnan(t)){
        Temperature["data"]="NaN";
    }
    else{
        Temperature["data"]=t;
    }

    Humidity["time"]=tps;
    if (isnan(h)){
        Humidity["data"]="NaN";
    }
    else{
        Humidity["data"]=h;
    }

    Light["time"]=tps;
    Light["data"]=Lux;

    Sound["time"]=tps;
    Sound["data"]=s*5/1025;

    send.printTo(json,sizeof(json)); // formating the objects into a string 
  
    Serial.print("Sending to LoRa Server : ");
    Serial.println((char*)json);
    rf95.send(json, sizeof(json));
    rf95.waitPacketSent();
}

void receive(){
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    uint8_t rcvID[50];
    StaticJsonBuffer<200> RcvBuffer;
    int value;
    
    /*
        Waiting for a message during the wanted timeout time.
        If received, the meassage is prossessed. If not, we carry on.
    */
    if (rf95.waitAvailableTimeout(TIMEOUT)){ 
        if (rf95.recv(buf, &len)){ 
            Serial.println("received :");
            Serial.println((char*)buf);
            JsonObject& rcv = RcvBuffer.parseObject(buf); 
            strcpy(rcvID,rcv["ID"]);
            if (!rcv.success()){
                Serial.println("Not Json");
            }
            else{

                /*
                    Proccessinf of a message, checking if its format is correct,
                    if it is destined to us and acting with the state received.
                    Wanted format : {"ID":"arduino2","actuator":"LED","state":1}
                */

                if(!strcmp(rcvID,ID)){
                    if(!strcmp(rcv["actuator"],"LED")){
                        switch ((int)rcv["state"]) {
                            case 1:
                                digitalWrite(LEDPIN, HIGH);
                                break;
                            case 0:
                                digitalWrite(LEDPIN, LOW);
                                break;
                            default:
                                Serial.println("Wrong entry");
                                break;
                        }
                    }
                    else{
                        Serial.println("wrong actuator");
                    }
                }
                else{
                    Serial.println("Not for me");
                }
            }
        }
        else{
            Serial.println("recv failed");
        }
    }
    else{
      Serial.println("No reply.");
    }
}

void loop(){
    send();
    receive();
    delay(PERIOD);
}
