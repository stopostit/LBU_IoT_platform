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

#define TIMEOUT 3000

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

void loop(){
    uint8_t json[RH_RF95_MAX_MESSAGE_LEN];
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float s = analogRead(SOUNDPIN);
    float lightsensor = analogRead(LIGHTPIN);
    float voltage = lightsensor * (5.0/1023) * 1000;
    float resistance = 10000 * ( voltage / ( 5000.0 - voltage) ); // Pont diviseur Vout = Vin*(R2/(R1 + R2))
    float Lux = ( 1000000 - resistance ) / 1998.4;

    if (isnan(t) || isnan(h)){
        Serial.println("Failed to read from DHT");
    } 

    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    uint8_t rcvID[50];
    // uint8_t copy[RH_RF95_MAX_MESSAGE_LEN];

    StaticJsonBuffer<200> jsonsendBuffer;
    JsonObject& send = jsonsendBuffer.createObject();
    send["ID"]=ID;
    send["time"] = micros();

    StaticJsonBuffer<200> RcvBuffer;
    int value;

    for(int i=0;i<4;i++){
        switch(i){
            case 0:
                send["sensor"] = "Temperature";
                send["data"]=t;
                break;
            case 1:
                send["sensor"] = "Humidity";
                send["data"]=h;
                break;
            case 2:
                send["sensor"] = "Sound";
                send["data"] = s*5/1025; 
                break;
            case 3:
                send["sensor"]= "Light";
                send["data"]=Lux;
                break;
            default:
                Serial.println("Case error");
                exit(1);
                break;
        }

        send.prettyPrintTo(json,sizeof(json)); 
      
        Serial.println("Sending to LoRa Server");
        rf95.send(json, sizeof(json));
        rf95.waitPacketSent();
        if (rf95.waitAvailableTimeout(TIMEOUT)){ 
            if (rf95.recv(buf, &len)){  
                // strcpy(copy,buf);
                Serial.println("received :");
                Serial.println((char*)buf);
                while(strcmp(buf,ID)){
                    Serial.println("Waiting...");
                    // JsonObject& rcv = RcvBuffer.parseObject(buf); // Dowlink handling
                    // strcpy(rcvID,rcv["ID"]);
                    // if (!rcv.success()){
                    //     Serial.println("Not Json");
                    // }
                    // else{
                    //     if(!strcmp(rcvID,ID)){
                    //         switch ((int)rcv["data"]) {
                    //             case 1:
                    //                 digitalWrite(LEDPIN, HIGH);
                    //                 break;
                    //             case 0:
                    //                 digitalWrite(LEDPIN, LOW);
                    //                 break;
                    //             default:
                    //                 Serial.println("Wrong entry");
                    //                 break;
                    //         }
                    //     }
                    //     else{
                    //         Serial.println("Not for me");
                    //     }
                    // }
                    delay(500);
                }
                Serial.println("OK received ack ! Let's carry on.");
            }
            else{
                Serial.println("recv failed");
            }
        }
        else{
          Serial.println("No reply, is LoRa server running?");
        }
        delay(PERIOD);
    }
}