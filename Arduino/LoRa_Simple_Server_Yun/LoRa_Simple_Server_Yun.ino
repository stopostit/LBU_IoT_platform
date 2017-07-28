#define BAUDRATE 115200

#include <Console.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

RH_RF95 rf95;

int led = A2;
float frequency = 433.0;

void setup() {
    pinMode(led, OUTPUT);     
    Bridge.begin(BAUDRATE);
    Console.begin();
    Console.println("Start Sketch");
    if (!rf95.init())
        Console.println("init failed");
    rf95.setFrequency(frequency);
    rf95.setTxPower(13);
}

void loop(){
    StaticJsonBuffer<200> jsonBuffer;
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    const char ID[RH_RF95_MAX_MESSAGE_LEN];
    const char sendbuf[RH_RF95_MAX_MESSAGE_LEN];

    String Rcvbuff;

    if (rf95.available()){
        if (rf95.recv(buf, &len)){
            digitalWrite(led, HIGH);
            Console.println((char*)buf);
            JsonObject& root = jsonBuffer.parseObject(buf);
            strcpy(ID,root["ID"]);
            rf95.send((const uint8_t *)ID, sizeof(ID));
            rf95.waitPacketSent();
            digitalWrite(led, LOW);

            Rcvbuff = Console.readString();
            if(Rcvbuff!=NULL){
                Rcvbuff.toCharArray(sendbuf, sizeof(sendbuf));
                Console.print("sending:");
                Console.println(sendbuf);
                rf95.send((const uint8_t *)sendbuf, sizeof(sendbuf));
                rf95.waitPacketSent();
                Rcvbuff=NULL;
            }
        }
        else{
            Console.println("recv failed");
        }
        // if(Console.available()){
        //     Rcvbuff = Console.readString();
        //     Rcvbuff.toCharArray(sendbuf, sizeof(sendbuf));
        //     Console.print("sending:");
        //     Console.println(sendbuf);
        //     rf95.send((const uint8_t *)sendbuf, sizeof(sendbuf));
        //     rf95.waitPacketSent();
        // }
    }
}



