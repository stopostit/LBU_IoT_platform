#include <DHT.h>

#define DHTPIN A1     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

int pinSon = A0; 
int pinLum = A5; 

void setup() {
  Serial.begin(9600);
  
  dht.begin();
}

void loop() {
    // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  
  delay(500);
      // La valeur de la tension est mesurée
  int rawValue = analogRead(pinLum);
  float voltage = rawValue * (5.0/1023) * 1000;
  float resistance = 10000 * ( voltage / ( 5000.0 - voltage) ); // Pont diviseur Vout = Vin*(R2/(R1 + R2))
  float Lux = ( 1000000 - resistance ) / 1998.4; //Pente de la courbe trouvée avec une datasheet .... Encore à revoir...
  //Ensuite calcul du Lux avec la résistance, difficile car pas une droite!!
  //https://www.carnetdumaker.net/articles/mesurer-la-luminosite-ambiante-avec-une-photoresistance-et-une-carte-arduino-genuino/
  
  Serial.print("Tension : ");   Serial.print(voltage); Serial.print("mV");
  Serial.print(", Resistance : "); Serial.print(resistance); Serial.println("Ohm");
  Serial.print("Luminosite : "); Serial.print(Lux); Serial.println("Lux");
  
  
  Serial.print("Sound Sensor Value: ");
  Serial.println(analogRead(pinSon)*5.0/1023.0);
  
  Serial.println("---------------------------------------");

 

}
