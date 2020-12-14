#include <ESP8266WiFi.h>
const char* ssid = "Eshwar";  // Enter SSID/ wifi name here.
const char* password = "14/06/2001";  //Enter Password of the wifi here.
String header;
String LED1State = "off";// Indicating the initial state of the L.E.D. to on stage.
const int LED1 = D0;// Any pin can be selected among the GPIO pins .
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);
 Serial.print("Connecting to ");
 Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print(".");
 }
// Print's IP address and start web server
 Serial.println("");
  Serial.println("WiFi connected to Eshwar-->");
  Serial.println("Copy this IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   
  if (client) {                             // If a new client connects,
    Serial.println("new client connected");          
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected())
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /LED1/on") >= 0) {
              Serial.println("LED1 on");
              LED1State = "on";
              digitalWrite(LED1, HIGH);
            } else if (header.indexOf("GET /LED1/off") >= 0) {
              Serial.println("LED1 off");
              LED1State = "off";
              digitalWrite(LED1, LOW);
            } 
            //This is the html code for to and fro communication from mcu and server
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Cairo; display: inline; margin: 0px auto; text-align: center; background-color: #ccffb3;}");
            client.println(".button { background-color: #006699; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 35px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
        
            client.println("<svg width=\"300\" height=\"80\"><text fill=\"#00bfbf\" font-family=\"serif\" font-size=\"24\" id=\"svg_1\" stroke=\"#000000\" text-anchor=\"middle\" transform=\"matrix(1.35388 0 0 1.42308 -6.66283 -8.67308)\" x=\"86.5\" xml:space=\"preserve\" y=\"41.5\">Circuit Digest</text></svg>");
          
            // Web Page Heading
            client.println("<body><h1>Welcome to ESP12e Web Server</h1>");
            client.println("<p>LED1_State: " + LED1State + "</p>");
            // If the LED1State is off, it displays the ON button       
            if (LED1State=="off") {
              client.println("<p><a href=\"/LED1/on\"><button class=\"button\">ON</button></a></p>");
              client.println("<svg width=\"500\" height=\"300\"><ellipse cx=\"258.5\" cy=\"125.5\" fill=\"#ffffff\" rx=\"47\" ry=\"52\" stroke=\"#ffffaa\" stroke-width=\"5\"/><rect fill=\"#cccccc\" height=\"40\" stroke=\"#ffffaa\" stroke-width=\"5\" transform=\"rotate(-0.485546 261 187.5)\" width=\"39\" x=\"241.5\" y=\"167.5\"/></svg>");

            } else {
              client.println("<p><a href=\"/LED1/off\"><button class=\"button button2\">OFF</button></a></p>");
              client.println("<svg width=\"500\" height=\"300\"><ellipse cx=\"258.5\" cy=\"125.5\" fill=\"#ff7f00\" rx=\"47\" ry=\"52\" stroke=\"#ffffaa\" stroke-width=\"5\"/><rect fill=\"#cccccc\" height=\"40\" stroke=\"#ffffaa\" stroke-width=\"5\" transform=\"rotate(-0.485546 261 187.5)\" width=\"39\" x=\"241.5\" y=\"167.5\"/></svg>");

            }     
            client.println("</body></html>");
            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected");
    Serial.println("");
  }
}