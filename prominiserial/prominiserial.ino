#include <SoftwareSerial.h>
#include "DHT.h"

DHT dht;
 
SoftwareSerial ArduinoSerial(3, 2); // RX, TX
void setup() {
  Serial.begin(115200);
  //Serial.begin(9600);
  ArduinoSerial.begin(4800);
  dht.setup(9); // data pin 2
}
void loop() {
  unsigned char  i;
  
    float PH = 0;
    for(i=0;i<10;i++){PH  = PH + analogRead(A4);delay(1); } 
    PH = PH / 10;    
    if(PH >= 450){PH = 40-((PH - 450)/50);}else
    if(PH >= 280){PH = 50-((PH - 280)/17);}else
    if(PH >= 150){PH = 60-((PH - 150)/13);}else
    if(PH >=  20){PH = 70-((PH -  20)/13);}else
    if(PH >=   0){PH = 80-((PH -  0 )/2); }

 
   int Moisture = 0; 
   for(i=0;i<10;i++){Moisture = Moisture + analogRead(A5);delay(1);}
   Moisture = Moisture / 10;
   if(Moisture >= 530){Moisture = ((Moisture - 550)/15) + 90;}else
   if(Moisture >= 430){Moisture = ((Moisture - 450)/10) + 80;}else
   if(Moisture >= 130){Moisture = ((Moisture - 130)/6) + 30;}else
   if(Moisture >=   0){Moisture = ((Moisture)/5);}   
   if(Moisture > 100){Moisture = 100;}

   delay(dht.getMinimumSamplingPeriod());

   int humidity = dht.getHumidity();
   int temperature = dht.getTemperature();
   int light = digitalRead(A3);

   
    //if(humidity != 0 || temperature != 0){
    String full = String(temperature)+String(humidity)+String(light)+String(Moisture)+String(PH/10);
    ArduinoSerial.print(full.toInt());
    ArduinoSerial.println("\n");
    Serial.println(full.toInt());
    delay(1000);
    //}

//    Serial.print(humidity);
//    Serial.print(",");
//    Serial.print(temperature);
//    Serial.print(",");
//    Serial.print(light);
//    Serial.print(",");
//    Serial.print(Moisture);
//    Serial.print(",");
//    Serial.print(PH/10);
//    Serial.println(",");
//    delay(1000);
    

}
