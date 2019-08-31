#include <SoftwareSerial.h>

#define RxD    5
#define TxD    6
#define DEBUG 1

SoftwareSerial GPS(RxD, TxD);
String result;
String data[5];
String state,timegps,latitude,longitude;
char c;
void setup()
{
  Serial.begin(9600);
  GPS.begin(9600);
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);
  delay(2000);
  digitalWrite(4, LOW);
  delay(10000);
  sendData("\r\n+ATI\r\n",50,DEBUG);
  //GPS.print("\r\n+ATE1\r\n");
  //result = GPS.readString();Serial.println(result);
  sendData("\r\n+AT+CGNSPWR=1\r\n",50,DEBUG);
  sendData("\r\n+AT+ECHARGE=1\r\n",50,DEBUG);
  sendData("\r\n+AT+CGNSSEQ=RMC\r\n",100,DEBUG);
  sendData("\r\n+AT+SAPBR=3,1,CONTYPE,GPRS\r\n",1000,DEBUG);
  sendData("\r\n+AT+SAPBR=3,1,APN,internet\r\n",1000,DEBUG);
  sendData("\r\n+AT+SAPBR=1,1\r\n",1000,DEBUG);
  sendData("\r\n+AT+HTTPINIT\r\n",500,DEBUG);
  sendData("\r\n+AT+HTTPPARA=CID,1\r\n",500,DEBUG);
}

void loop()
{

  state = "0";
  timegps = "0";
  latitude = "0";
  longitude = "0";

 sendTabData("AT+CGNSINF",1000,DEBUG);
if (state !=0) {
//    Serial.println("State  :"+state);
//    Serial.println("Time  :"+timegps);
//    Serial.println("Latitude  :"+latitude);
//    Serial.println("Longitude  :"+longitude);
}
  sendData((String)"\r\n+AT+HTTPPARA=URL,http://gps.XXXXXXX.com/g/00001/"+timegps+":"+latitude+":"+longitude+"\r\n",1000,DEBUG);
  sendData("\r\n+AT+HTTPACTION=0\r\n",2000,DEBUG);
  GPS.println((char)26);
}
void sendTabData(String command , const int timeout , boolean debug){
  GPS.println(command);
  long int time = millis();
  int i = 0;
  c = 0;
  data[1] = "";
  data[2] = "";
  data[3] = "";
  data[4] = "";
  data[5] = "";
  while((time+timeout) > millis()){
    while(GPS.available()){
      c = GPS.read();
      if (c != ',') {
         data[i] +=c;
         delay(100);
      } else {
        i++;  
      }
      if (i == 5) {
        delay(100);
        goto exitL;
      }
    }
  }exitL:
  if (debug) {
    state = data[1];
    timegps = data[2];
    latitude = data[3];
    longitude =data[4];  
  }
}

void sendData(String command, const int timeout, boolean debug)
{
    String response = "";    
    GPS.println(command); 
    long int time = millis();   
    while( (time+timeout) > millis()){
      while(GPS.available()){       
        response += (char)GPS.read(); 
      }  
    }    
    if(debug){
      Serial.print(response);
    }    
    return(response);
}
