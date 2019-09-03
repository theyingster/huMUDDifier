#include "dht.h"
#define dht_apin A0 // Analog Pin sensor is connected to
 
dht DHT;

boolean state = false;
const double THRESHOLD = 70;

void setup(){
  pinMode(2, OUTPUT);
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
 
}//end "setup()"
 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    
    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    if(DHT.humidity>THRESHOLD){deactivate();}
    else{activate();}
    
    delay(50);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop()


void activate(){
  digitalWrite(2,HIGH);
}

void deactivate(){
  digitalWrite(2,LOW);
}

void toggle(){
  if (state == false){
    activate();
    state = true;
    }  
  else{
    deactivate();
    state = false;
    }
}
