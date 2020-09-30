#include <Ticker.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "index.h" //Our HTML webpage contents with javascripts
#define LED 2

LiquidCrystal_I2C lcd(0x27,20,4);
Ticker ticker;

const int  AlphaPIN = 15; //D8
const int  BetaPIN = 13; //D7
const int  GammaPIN = 12; //D6
int AlphaClickCounter = 0;   
int AlphaState = 0;         
int lastAlphaState = 0; 
int BetaClickCounter = 0;   
int BetaState = 0;         
int lastBetaState = 0;
int GammaClickCounter = 0;   
int GammaState = 0;         
int lastGammaState = 0;  
int i = 0;

//SSID and Password of your WiFi router
const char* ssid = "TIP-QC-WIFI";
const char* password = "T!pqc-938";

ESP8266WebServer server(80);

byte AlphaChar[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B01101,
  B10010,
  B10010,
  B01101
};

byte BetaChar[] = {
  B00000,
  B01100,
  B10010,
  B10010,
  B11100,
  B10010,
  B10001,
  B11111
};

byte GammaChar[] = {
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B01010,
  B01010,
  B00100
};

void Print(){
  lcd.clear();
  AlphaClickCounter = AlphaClickCounter;
  Serial.println("Alpha: ");
  Serial.println(AlphaClickCounter);
  lcd.createChar(0,AlphaChar);
  lcd.setCursor(0,0);
  lcd.write(0);
  lcd.print(":");
  lcd.print(AlphaClickCounter);
  AlphaClickCounter = 0;
  BetaClickCounter = BetaClickCounter;
  Serial.println("Beta: ");
  Serial.println(BetaClickCounter);
  lcd.createChar(1,BetaChar);
  lcd.setCursor(9,0);
  lcd.write(1);
  lcd.print(":");
  lcd.print(BetaClickCounter);
  BetaClickCounter = 0;
  GammaClickCounter = GammaClickCounter;
  Serial.println("Gamma: ");
  Serial.println(GammaClickCounter);
  lcd.createChar(2,GammaChar);
  lcd.setCursor(0,1);
  lcd.write(2);
  lcd.print(":");
  lcd.print(GammaClickCounter);
  GammaClickCounter = 0; 
}

int AlphaSave(){
  AlphaState = digitalRead(AlphaPIN);
  if (AlphaState != lastAlphaState) {
    if (AlphaState == HIGH) {
    // if the state has changed, increment the counter
      AlphaClickCounter++;
    }  
  }
lastAlphaState = AlphaState;
}

int BetaSave(){
  BetaState = digitalRead(BetaPIN);
  if (BetaState != lastBetaState) {
    if (BetaState == HIGH) {
    // if the state has changed, increment the counter
      BetaClickCounter++;
    }  
  }
lastBetaState = BetaState;
}

int GammaSave(){
  GammaState = digitalRead(GammaPIN);
  if (GammaState != lastGammaState) {
    if (GammaState == HIGH) {
    // if the state has changed, increment the counter
      GammaClickCounter++;
    }  
  }
lastGammaState = GammaState;
}

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleData() {
 int a = AlphaClickCounter;
 int b = BetaClickCounter;
 int c = GammaClickCounter;
 String AlphaValue = String(a);
 String BetaValue = String(b);
 String GammaValue = String(c);
 String RadData = AlphaValue + "~" + BetaValue+ "~" +GammaValue;
 digitalWrite(LED,!digitalRead(LED)); //Toggle LED on data request ajax
 server.send(200, "text/plane", RadData);
}



void setup() {
  // put your setup code here, to run once:
  pinMode(AlphaPIN, INPUT);
  pinMode(BetaPIN, INPUT);
  pinMode(GammaPIN, INPUT);
  Serial.begin(115200); 
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
  lcd.init();  //change to init or begin kung di gumana                    
  lcd.backlight();
  lcd.setCursor(0,0);
  // Wait for connection
  lcd.print("CONNECTING...");
  delay(5000); //Wait period hanggang makaconnect
  lcd.clear();
  //if not connected
  if (WiFi.status() != WL_CONNECTED) {
    while(i < 1){
      i = i+1;
      lcd.setCursor(0,0);
      lcd.print("Not Connected");
      lcd.setCursor(0,1);
      lcd.print("Offline Mode");
      delay(3000);
    }
    loop();
    ticker.attach(5,Print);
    //ticker.attach(5,BetaPrint);
    //ticker.attach(5,GammaPrint);
   
}

  //if connected
  //dito ka magsetup for web server
  if (WiFi.status() == WL_CONNECTED) {
  Serial.println("");
  lcd.setCursor(0,0);
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  lcd.setCursor(0,0);
  lcd.print("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  ticker.attach(5,Print);
  //ticker.attach(5,BetaPrint);
  //ticker.attach(5,GammaPrint); 
  server.on("/", handleRoot);
  server.on("/readData", handleData);
  //server.on("/readBeta", handleBeta);
  //server.on("/readGamma", handleGamma);
  server.begin();
  Serial.println("Server Started");
}
}

void loop() {  
  AlphaSave();
  BetaSave();
  GammaSave();
  server.handleClient();

}
