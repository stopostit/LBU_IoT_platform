# Leeds Beckett University IoT project
## description ;
The goal is to deploy an IoT platform.
it is structured as follow: 
sensors->arduino with LoRa interface -> Gateway (dragino LG1 which is basically an arduino Yun with openwrt) 
-> firebase cloud-> android/web client app

Initially we planned to use zetta (based on node js) but node needed to be cross compiled and this brought numerous issues.
The gateway receive json containing arduino+sensors data and send it to firebase through a REST API with libcurl.
