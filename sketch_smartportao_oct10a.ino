 #include <ESP8266WiFi.h>
 #include <WiFiClient.h>
  
 #include <ESP8266WebServer.h>
 #include <ESP8266mDNS.h>
  
 const char* ssid = "yourSSID";
 const char* password = "yourPassword";
 MDNSResponder mdns;
 ESP8266WebServer server(80);
 
 
 //STATIC MODE!
 // Update these with values suitable for your network.
 IPAddress ip(192,168,0,128);  //Node static IP
 IPAddress gateway(192,168,0,1);  
 IPAddress subnet(255,255,255,0);
 
 
 const int led = 4;     //PIN
 const int input_4 = 2;  //PIN
 
 
 void handleRoot() {
   server.send(200, "text/plain", 
     "Welcome");
 }
 
 void handleNotFound(){
   String message = "File Not Found\n\n";
   server.send(404, "text/plain", message);
 }
  
 void setup(void){

  //initializing pin
   pinMode(led, OUTPUT); 
   pinMode(input_4, OUTPUT);
   digitalWrite(led, 0);
   digitalWrite(led, 1);
   digitalWrite(input_4, 0);
   digitalWrite(input_4, 1);
   
   Serial.begin(115200);
   WiFi.begin(ssid, password);
   WiFi.config(ip, gateway, subnet);
   
   Serial.println("");
 
   // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(ssid);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());
   
   if (mdns.begin("esp8266", WiFi.localIP())) {
     Serial.println("MDNS responder started");
   }
   
   server.on("/", handleRoot);
 
   
   server.on("/in02/state.json", [](){
        
         if( digitalRead(led) == LOW) 
         {
             digitalWrite(led, HIGH);
             server.send(200, "GET HTTP /1.1 200 OK\r\n Content -Type: text/json;", "{\n\"state\":false\n}");
         }
         else
         {
             digitalWrite(led, LOW);
             server.send(200, "GET HTTP /1.1 200 OK\r\n Content -Type: text/json;", "{\n\"state\":true\n}");
         }
   });
   server.on("/in04/state.json", [](){
     
         if( digitalRead(input_4) == LOW) 
         {
             digitalWrite(input_4, HIGH);
             server.send(200, "GET HTTP /1.1 200 OK\r\n Content -Type: text/json;", "{\n\"state\":false\n}");
         }
         else
         {
             digitalWrite(input_4, LOW);
             server.send(200, "GET HTTP /1.1 200 OK\r\n Content -Type: text/json;", "{\n\"state\":true\n}");
         }
   });


 
   server.onNotFound(handleNotFound);
   
   server.begin();
   Serial.println("HTTP server started");
 
   //delay a moment, 
   //for terminal to receive inf, such as IP address
   delay(1000);
   Serial.end();  
 }
  
 void loop(void){
   server.handleClient();
 } 
 
 
 
 
 
