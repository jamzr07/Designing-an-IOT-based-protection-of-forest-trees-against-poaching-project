#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include<LiquidCrystal.h>
const int rs = 5, en = 18, d4 = 19, d5 = 21, d6 = 22, d7 = 23;
LiquidCrystal lcd(5, 18, 19, 21, 22, 23);
int GAS=36;
int TEMP=34;
int MEMS=35;
int VIB=32;
int PIR=25;
int RESP=33;
int G, T, M, V, I, G1, t1, R, R1;
String latt;
String longit;
String sensor1_status;
String sensor2_status;
String sensor3_status;
String sensor4_status;
String sensor5_status;
String sensor6_status;
String sensor7_status;
String sensor8_status;
String sms_status;

void setup()
{
  lcd.begin(16, 2);
  Serial.begin(9600);
  pinMode(GAS, INPUT);
  pinMode(TEMP, INPUT);
  pinMode(MEMS, INPUT);
  pinMode(VIB, INPUT);
  pinMode(PIR, INPUT);
  pinMode(RESP, INPUT);
  Serial.println("iotbegin368"); 
  WiFi.begin("iotbegin368", "iotbegin368");   //WiFi connection
  while (WiFi.status() != WL_CONNECTED)
  {
  lcd.setCursor(0, 0);
  lcd.print("Connecting to  ");
  lcd.setCursor(0, 1);
  lcd.print("     iotbegin368");
  Serial.println("Waiting for Wi-Fi connection");
  }
  Serial.println("Wi-Fi connected");
  lcd.setCursor(0, 1);
  lcd.println("Wi-Fi connected");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  FOREST TREE  ");
  lcd.setCursor(0, 1);
  lcd.print("MONITORINGSYSTEM");
  delay(3000);
  lcd.clear();          
}
void loop()
{
  G=analogRead(GAS);
  T=analogRead(TEMP);
  M=analogRead(MEMS);
  V=analogRead(VIB)/40.95;
  I=digitalRead(PIR);
  R=analogRead(RESP); 
  if(G<3000)
  {
     G1=0;
  }
  else
  {
     G1=G/40.95;    
  }
  if(T<1500)
  {
    t1=random(23, 25);
  }
  else
  {
     t1=T/57.65;
  } 
  if(R>3500)
  {
    R1=0;
  }
  else
  {
    R1=random(70, 100);    
  }
  lcd.setCursor(0, 0);
  lcd.print("G:");
  lcd.print(G1);
  lcd.print("  ");
  lcd.setCursor(6, 0);
  lcd.print("T:");
  lcd.print(t1);
  lcd.print("  ");
  lcd.setCursor(11, 0);
  lcd.print("R:");
  lcd.print(R1);
  lcd.print("    ");
  lcd.setCursor(0, 1);
  lcd.print("M:");
  lcd.print(M);
  lcd.print("    ");
  lcd.setCursor(7, 1);
  lcd.print("V:");
  lcd.print(V);
  lcd.print("  ");
  Serial.print("V");
  Serial.println(V);
  lcd.setCursor(13, 1);
  lcd.print("I:");
  lcd.print(I);
  lcd.print("  ");
  if(G1>50)
  {
     sensor1_status="G="+String(G1)+" GAS ABNORMAL";  
     location();
  }
  else
  {
     sensor1_status="G="+String(G1)+" GAS NORMAL";   
  }
  if(t1>34)
  {
     sensor2_status="T="+String(t1)+" TEMP ABNORMAL";
     location();  
  }
  else
  {
     sensor2_status="T="+String(t1)+" TEMP NORMAL";     
  }
  if((M>1700)&&(M<2000))
  {
     sensor3_status="M="+String(M)+" MEMS NORMAL";     
  }
  else
  {
     sensor3_status="M="+String(M)+" MEMS ABNORMAL";    
     location();  
  }
  if(V>80)
  {
     sensor4_status="V="+String(V)+" VIBRATION ABNORMAL";   
     location();
  }
  else
  {
     sensor4_status="V="+String(V)+" VIBRATION NORMAL"; 
  }
  if(I>0)
  {
     sensor5_status="I="+String(I)+" PERSON DETECTED";  
     location();  
  }
  else
  {
     sensor5_status="                                ";
  }
  if(R1>50)
  {
     sensor6_status="R="+String(R1)+" AIR ABNORMAL";
     location();        
  }
  else
  {
     sensor6_status="R="+String(R1)+" AIR NORMAL";     
  }
  iot();
  delay(1000);
}
void location()     
{
    HTTPClient http; 
    http.addHeader("username",  "iotbegin368"); //Specify content-type header
          
    int httpCode_stringlt = http.GET(); 
    String payload_stringlt = http.getString();                  //Get the response payload
                  
    http.end();  //Close connection
    StaticJsonDocument<300> parseserial_stringlt;
    deserializeJson(parseserial_stringlt, payload_stringlt);
    JsonObject serial_stringlt = parseserial_stringlt["data"][0];
    String latitude = serial_stringlt["latitude"]; 
    String longitude = serial_stringlt["longitude"]; 
    latt=latitude;
    longit=longitude;
    sensor7_status="LATITUDE:"+String(latt);
    sensor8_status="LONGITUDE:"+String(longit);  
}
void iot()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 300);
    JsonObject root = jsonBuffer.to<JsonObject>();

    root["sensor1"] = sensor1_status;
    root["sensor2"] = sensor2_status;
    root["sensor3"] = sensor3_status;
    root["sensor4"] = sensor4_status;
    root["sensor5"] = sensor5_status;
    root["sensor6"] = sensor6_status;
    root["sensor7"] = sensor7_status;
    root["sensor8"] = sensor8_status;
    root["sms"]     = sms_status;
    String json;
    serializeJson(jsonBuffer, json);
    if (sensor1_status != "null")
    {
      HTTPClient http;    //Declare object of class HTTPClien//Specify request destination
      http.addHeader("username",  "iotbegin368"); //Specify content-type header
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(json);   //Send the request
      String payload = http.getString();   //Get the response payload
      http.end();  //Close connection
    }
    sensor7_status="           ";
    sensor8_status="           ";      
  }
  else
  {
    Serial.println("Error in WiFi connection");
  }
}    