#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <DHT.h>

#include <SPI.h>
#include <RH_RF95.h>
#include <Time.h>


#define DHTPIN A1  // what pin we're connected to
#define SOUNDPIN A0
#define LIGHTPIN A5

#define DHTTYPE DHT11   

#define PERIOD 5000

#define TIMEOUT 4500

#define LEDPIN 7

// Singleton instance of the radio driver
RH_RF95 rf95;
float frequency = 433.0;
uint8_t ID[]="arduino2";

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
    uint8_t json[RH_RF95_MAX_MESSAGE_LEN];
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float s = analogRead(SOUNDPIN);
    float lightsensor = analogRead(LIGHTPIN);
    float voltage = lightsensor * (5.0/1023) * 1000;
    float resistance = 10000 * ( voltage / ( 5000.0 - voltage) ); // Pont diviseur Vout = Vin*(R2/(R1 + R2))
    float Lux = ( 1000000 - resistance ) / 1998.4;
    int tps=micros();

    StaticJsonBuffer<200> jsonsendBuffer;
    JsonArray& root = jsonsendBuffer.createArray();
    JsonObject& send = root.createNestedObject().createNestedObject("send");
    JsonObject& arduino2=send.createNestedObject("arduino2");
    JsonObject& Temperature=arduino2.createNestedObject("Temperature");
    JsonObject& Humidity=arduino2.createNestedObject("Humidity");
    JsonObject& Light=arduino2.createNestedObject("Light");
    JsonObject& Sound=arduino2.createNestedObject("Sound");

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

    send.printTo(json,sizeof(json)); 
  
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
    
    if (rf95.waitAvailableTimeout(TIMEOUT)){ 
        if (rf95.recv(buf, &len)){ 
            Serial.println("received :");
            Serial.println((char*)buf); 
            // strcpy(copy,buf);
            // while(strcmp(buf,ID)){
                // Serial.println("Waiting...");
                // if (rf95.waitAvailableTimeout(TIMEOUT)){ 
                //     if (rf95.recv(buf, &len)){ 
                        JsonObject& rcv = RcvBuffer.parseObject(buf); // Dowlink handling
                        strcpy(rcvID,rcv["ID"]);
                        if (!rcv.success()){
                            Serial.println("Not Json");
                        }
                        else{
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
            //         }
            //         else{
            //             Serial.println("recv failed");
            //         }
            //     }
            //     else{
            //         Serial.println("Timeout");
            //     }
            //     delay(500);
            // }
            // Serial.println("OK received ack ! Let's carry on.");
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



// {
//     "arduino2": {
//         "light":{
//             "data":24,
//             "timestamp":14
//         }
//         "sound":{
//             "data":24,
//             "timestamp":14
//         }
//     }
// }


// {"ID":"arduino2","actuator":"LED","state":1}