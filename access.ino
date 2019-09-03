#include <ESP8266WiFi.h>

IPAddress ip(172, 28, 93, 230);

const char* host="172.28.93.230";
const char* turnOffAddress="172.28.93.230";
const char* turnOnAddress="172.28.93.230";

// Replace with your network credentials
const char* ssid     = "Claremont-ETC";
const char* password = "abcdeabcde";

int status = WL_IDLE_STATUS;

void deactivate();

void setup(){
  pinMode(5,INPUT);
  Serial.begin(115200);
  Serial.println("Are you fk dumb or sth");
  // attempt to connect to Wifi network:
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // print your WiFi shield's IP address:
  //Serial.print("IP Address:");
  //Serial.println(WiFi.localIP()); 
}
 
void loop(){
  Serial.println(digitalRead(5));
  //Serial.println(DHT.humidity);
  if(digitalRead(5)){
    activate();
  }else{
    deactivate();
  }
 
}// end loop() 

void activate(){
  WiFiClient client;

  //Serial.println("123");
  
  if (client.connect(host, 80))
  {
    //Serial.println("connected & activated");  

    client.print(String("GET /1/in") + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: close\r\n" +
             "\r\n"
            );
            
    client.stop();
    //Serial.println("\n[Disconnected]");
  }
  
  
  //Serial.println(WiFi.localIP());
}

void deactivate(){
  WiFiClient client;

  //Serial.println("333");
  
  if (client.connect(host, 80))
  {
    //Serial.println("connected & deactivated");
    client.print(String("GET /1/out") + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" +
             "Connection: close\r\n" +
             "\r\n"
            );
    client.stop();
    //Serial.println("\n[Disconnected]");
  } 
}
