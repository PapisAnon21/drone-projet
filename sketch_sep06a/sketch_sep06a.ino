// this is the drone project
// inclusons less biblio necessaires
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <WebSocketsServer.h>

AsyncWebServer server(80);
WebSocketsServer websocket = WebSocketsServer(81);

String message_recu;


char ssidAP[] = "WIFI_ESP"; // WLAN SSID and password
char passwordAP[] = "monmotdepasse";

#define motor_speed 16
#define direction_1 5
#define direction_2 4

pinMode(motor_speed,OUTPUT);
pinMode(direction_1, OUTPUT);
pinMode(direction_2, OUTPUT);

void wsEvent(uint8_t n, WStype_t type, uint8_t * message, 
size_t lengthh) // le gestionnaire d'evenements qui gere les messages recus depuis le clients
{
if(type == WStype_TEXT)
{
message_recu = "";
for (int i=0; i<lengthh; i++) message_recu = message_recu + char(message[i]);
// une fois ici message contient le nombre envoye par le range printons  le pour voir
Serial.println(message_recu);

analogWrite(motor_speed, parseInt(message_recu));
}
}
void setup() {

  Serial.begin(115200);
WiFi.mode(WIFI_AP); 

delay(1000); // on attend 1seconde
// etablit le mot de passe et le nom du wifi 
WiFi.softAP(ssidAP, passwordAP); 

IPAddress myIP = WiFi.softAPIP();
Serial.print("l'adresse IP est : ");
Serial.println(myIP);
if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
}

server.begin();
websocket.begin();

websocket.onEvent(wsEvent);
// ecoute des requetes passe au serveur
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html","text/html");
  });
  
  // Route to load style.css file
  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.css", "text/css");
});
// la je defini une direction de tournage
digitalWrite(direction_1 , HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
websocket.loop();
}
