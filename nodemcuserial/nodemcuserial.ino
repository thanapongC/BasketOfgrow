#include <ArduinoJson.h>
#include <ESP8266WiFi.h> 
#include <FirebaseArduino.h>
#include <time.h>
#include <SoftwareSerial.h>


SoftwareSerial NodeSerial(D2, D3); // RX | TX
//const char* ssid = "NOOMJUNGLE"; 
//const char* pass = "0876641405"; 

//const char* ssid = "NoomJungle"; 
//const char* pass = "0876641405"; 

const char* ssid = ".iApp"; 
const char* pass = "innovation"; 

//const char* ssid = "ICTES_Lab-2G"; 
//const char* pass = "officett4321"; 

//const char* ssid = "Not_2.4GHz"; 
//const char* pass = "0894122322"; 

const String deviceNumber = "130";

int timezone = 7 * 3600;
int dst = 0;
String Moisture,temp,humid,light,PH;

//define//https://basketofgrow.firebaseio.com
#define FIREBASE_HOST "basketofgrow.firebaseio.com"
#define FIREBASE_KEY "CPZ7GRwCunbmhTPXlXtSAynaJxH9BiRSPduqdGte"
#define pumpwaterrelay1 D5
#define lightrelay2 D6


void setup() {
  
  pinMode(pumpwaterrelay1,OUTPUT);
  pinMode(lightrelay2,OUTPUT);
  
  Serial.begin(115200);
  NodeSerial.begin(4800);

  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  
    Serial.println("Starting...");
      configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");
       Serial.println("\nLoading time");
              while (!time(nullptr)) {
              Serial.print("*");
              delay(1000);
              }
              
    WiFi.begin(ssid, pass);
       while (WiFi.status() != WL_CONNECTED) 
       {
          delay(250);
          Serial.print(".");
          digitalWrite(lightrelay2,LOW);
          digitalWrite(pumpwaterrelay1,LOW); 
       }

   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   
   digitalWrite(lightrelay2,HIGH);
   digitalWrite(pumpwaterrelay1,LOW); 
  
}

void loop() {

      if(NodeSerial.available() > 0){
         int val = NodeSerial.parseInt();
         if(NodeSerial.read() == '\n'){
         String valstr = String(val);
         String temp = valstr.substring(0,2);
         String humid = valstr.substring(2,4);
         String light = valstr.substring(4,5);
         String Moisture = valstr.substring(5,7);
         String PH = valstr.substring(7,11);
         Serial.print("Data Structure : ");
         Serial.print("Temperature : "+temp+"|");
         Serial.print("Humidity : "+humid+"|");
         Serial.print("Light : "+light+"|");
         Serial.print("Moisture : "+Moisture+"|");
         Serial.println("PH : "+PH+"|");  
         if(temp.toInt() >= 0 || temp.toInt() <= 60 ||
            humid.toInt() >= 1 || humid.toInt() <= 100 ||
            Moisture.toInt() >= 0 || Moisture.toInt() <= 100 ||
            PH.toInt() >= 3 || PH.toInt() <= 8 ||
            light.toInt() < 0 || light.toInt() > 1){ 
          StaticJsonBuffer<200> jsonBuffer;
          JsonObject& valueObject = jsonBuffer.createObject();
          valueObject["AirTemp"] = temp;
          valueObject["AirHumid"] = humid;
          valueObject["Soilmois"] = Moisture;
          valueObject["PHvalue"] = PH;
          valueObject["Light"] = light;
          Firebase.set("grow/"+ deviceNumber + "/value/", valueObject);
          Serial.println("Put Data To Firebase.......!!");
         }
    delay(10000);
  }
 }
  
     configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");  
     time_t now = time(nullptr);
     struct tm* p_tm = localtime(&now);
     String timenowhour = String(p_tm->tm_hour);
    
    if(timenowhour == "0" || timenowhour == "1" || timenowhour == "2" || timenowhour == "3" || timenowhour == "4" ||
       timenowhour == "5" || timenowhour == "6" || timenowhour == "7" || timenowhour == "8" || timenowhour == "9")
    {
     timenowhour = "0"+timenowhour;
    }else{
     timenowhour = timenowhour;
      }
      
    String timenowmin = String(p_tm->tm_min);

    if(timenowmin == "0" || timenowmin == "1" || timenowmin == "2" || timenowmin == "3" || timenowmin == "4" ||
       timenowmin == "5" || timenowmin == "6" || timenowmin == "7" || timenowmin == "8" || timenowmin == "9")
    {
     timenowmin = "0"+timenowmin;
    }else{
     timenowmin = timenowmin;
      }

    String timenowsec = String(p_tm->tm_sec);

    if(timenowsec == "0" || timenowsec == "1" || timenowsec == "2" || timenowsec == "3" || timenowsec == "4" ||
       timenowsec == "5" || timenowsec == "6" || timenowsec == "7" || timenowsec == "8" || timenowsec == "9")
    {
     timenowsec = "0"+timenowsec;
    }else{
     timenowsec = timenowsec;
      }
      
    String timenow = timenowhour+timenowmin+timenowsec;
    int inttimenow = timenow.toInt();
    

  String light = Firebase.getString("grow/"+ deviceNumber + "/device/relay220v1");
  String pump = Firebase.getString("grow/"+ deviceNumber + "/device/relay220v2");
  String setmoisture = Firebase.getString("grow/"+ deviceNumber +"/device/setmoisture");
  String switchselect = Firebase.getString("grow/"+ deviceNumber +"/switchselect/");
  String lightfirebasestart = Firebase.getString("grow/"+ deviceNumber +"/device/relay220v1starttime");
  String lightfirebaseend = Firebase.getString("grow/"+ deviceNumber +"/device/relay220v1endtime");
  String pumpfirebasestart = Firebase.getString("grow/"+ deviceNumber +"/device/relay220v2starttime");
  String pumpfirebaseend = Firebase.getString("grow/"+ deviceNumber +"/device/relay220v2endtime");

   if(switchselect == "manual"){

    Serial.println("Manual-System");
    
   if(light == "on"){
    digitalWrite(lightrelay2,HIGH);
   }else{
    digitalWrite(lightrelay2,LOW);
    }
   if(pump == "on"){
    digitalWrite(pumpwaterrelay1,HIGH);
    }else{
    digitalWrite(pumpwaterrelay1,LOW); 
    } 
    
   }

   else if(switchselect == "time"){

    Serial.println("Time-System");

       if(lightfirebasestart.toInt() <= inttimenow && lightfirebaseend.toInt() >= inttimenow){
        
        digitalWrite(lightrelay2,HIGH);
        }
      else{
        digitalWrite(lightrelay2,LOW);
        }  

      if(pumpfirebasestart.toInt() <= inttimenow && pumpfirebaseend.toInt() >= inttimenow){
        
        digitalWrite(pumpwaterrelay1,HIGH);
        }
      else{
        digitalWrite(pumpwaterrelay1,LOW);
        }
    
    }

   else if(switchselect == "auto"){

    Serial.println("Auto-System");
    
      if(06000 <= inttimenow && 180000 >= inttimenow){
        
        digitalWrite(lightrelay2,HIGH);
        }
      else{
        digitalWrite(lightrelay2,LOW);
        }  

      if(060000 <= inttimenow && 060020 >= inttimenow){
        
        digitalWrite(pumpwaterrelay1,HIGH);
        }
      else{
        digitalWrite(pumpwaterrelay1,LOW);
        }
   }

  else if(switchselect == "moisture"){

      Serial.println("Moisture-System");
      Serial.println("Moisture : "+Moisture);
    
      if (setmoisture.toInt() <= Moisture.toInt()){
        digitalWrite(pumpwaterrelay1,HIGH);
        delay(3000);
        digitalWrite(pumpwaterrelay1,LOW);
        delay(60000);
      }else{
        digitalWrite(pumpwaterrelay1,LOW);
           }
      if(06000 <= inttimenow && 210000 >= inttimenow){
        digitalWrite(lightrelay2,HIGH);
      }else{
        digitalWrite(lightrelay2,LOW);
           }
           
  }else{
    digitalWrite(lightrelay2,LOW);
    digitalWrite(pumpwaterrelay1,LOW);
    }

   delay(1000);

}



