/*********
  Based on code by Rui Santos  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "Claremont-ETC";
const char* password = "abcdeabcde";

//state of the humidifier
bool state=false;

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String roomate1State = "in";

// Assign output variables to GPIO pins
// LED short leg goes to D1
const int humid = 5;
// Button plugged into ground and D2
const int button1 = 4;

boolean pressed = false;

void setup() {
  Serial.begin(115200);
  // Initialize the button input with the pullup-resistor
  pinMode(button1, INPUT_PULLUP);
  // Initialize the output variables as outputs
  pinMode(humid, OUTPUT);
  // Set outputs to LOW
  digitalWrite(humid, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
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
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /1/out") >= 0) {
              Serial.println("LED1 off");
              roomate1State = "out";
              deactivate();
            } else if (header.indexOf("GET /1/in") >= 0) {
              Serial.println("LED1 on");
              roomate1State = "in";
              activate();
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p> 'Your Name Here' is " + roomate1State + "</p>");
            // If the output5State is off, it displays the ON button       
            if (roomate1State =="out") {
              client.println("<p><a href=\"/1/in\"><button class=\"button\">IN</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/out\"><button class=\"button button2\">OUT</button></a></p>");
            } 
            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
    }
 }

 void activate(){
    if(!state){
      digitalWrite(humid,HIGH);
      delay(200);
      digitalWrite(humid,LOW);
      delay(300);
      digitalWrite(humid,HIGH);
      delay(200);
      digitalWrite(humid,LOW);
      delay(300);
      state=true;
    }
 }

 void deactivate(){
    if(state){
      digitalWrite(humid,HIGH);
      delay(200);
      digitalWrite(humid,LOW);
      delay(300);
      state=false;
    }
 }
