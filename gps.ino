#include <SoftwareSerial.h>

#define RxD    5
#define TxD    6
#define DEBUG 1

SoftwareSerial GPS(RxD, TxD);

String result,payload,flag;
String data[7];
//String state,timegps,latitude,longitude,alt,sp;
char c;
int r;
void setup()
{
  Serial.begin(9600);
  GPS.begin(9600);
  delay(1000);
  pinMode(4, OUTPUT);
  //sendData("\r\n+ATI\r\n",50,DEBUG);
  //delay(1000);
  //if (flag == 0) {
  digitalWrite(4, HIGH);
  delay(2000);
  digitalWrite(4, LOW);
  delay(10000);
  //}
  //sendData("\r\n+ATI\r\n",50,DEBUG);
  //Serial.println(flag);
  sendData("\r\n+AT+CGNSPWR=1\r\n",50,DEBUG);
  sendData("\r\n+AT+ECHARGE=1\r\n",50,DEBUG);
  sendData("\r\n+AT+CGNSSEQ=RMC\r\n",100,DEBUG);
  sendData("\r\n+AT+SAPBR=3,1,CONTYPE,GPRS\r\n",1000,DEBUG);
  sendData("\r\n+AT+SAPBR=3,1,APN,internet\r\n",1000,DEBUG);
 // sendData("\r\n+AT+HTTPINIT\r\n",500,DEBUG);
  r = 0;
}

void loop()
{
  r++;
  Serial.println((String)"Step:"+r);
  if (r > 30){
    sendData("\r\n+AT+CFUN=1,1\r\n",50,DEBUG);
    delay(10000);
    sendData("\r\n+AT+CGNSPWR=1\r\n",50,DEBUG);
    sendData("\r\n+AT+ECHARGE=1\r\n",50,DEBUG);
    sendData("\r\n+AT+CGNSSEQ=RMC\r\n",100,DEBUG);
    sendData("\r\n+AT+SAPBR=3,1,CONTYPE,GPRS\r\n",1000,DEBUG);
    sendData("\r\n+AT+SAPBR=3,1,APN,internet\r\n",1000,DEBUG);
    r = 0;
  }
  sendData("\r\n+AT+HTTPINIT\r\n",500,DEBUG);
  sendData("\r\n+AT+SAPBR=1,1\r\n",1000,DEBUG);
  sendData("\r\n+AT+HTTPPARA=CID,1\r\n",500,DEBUG);
  sendTabData("\r\n+AT+CGNSINF\r\n",2000,DEBUG);
  Serial.flush();
  payload = "";
  payload = (String)"\r\n+AT+HTTPPARA=URL,http://gps.XXXXXXX.com/g/1/"+data[2]+":"+data[3]+":"+data[4]+":"+data[5]+":"+data[6]+"\r\n";
  sendData (payload,500, DEBUG);
  delay(100);
  sendData("\r\n+AT+HTTPPARA=\"BREAK\",3000",3000,DEBUG);
  sendData("\r\n+AT+HTTPACTION=0\r\n",3000,DEBUG);
  sendData("\r\n+AT+HTTPTERM\r\n",500,DEBUG);
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
  data[6] = "";
  data[7] = "";
  while((time+timeout) > millis()){
    while(GPS.available()){
      c = GPS.read();
      //Serial.print(c);
      if (c != ',') {
         data[i] +=c;
         delay(40);
      } else {
        i++;  
      }
      if (i == 7) {
        delay(40);
        goto exitL;
      }
    }
  }exitL:
  if (debug) {
//    state = data[1];
//    timegps = data[2];
//    latitude = data[3];
//    longitude = data[4];
//    alt = data[5];
//    sp = data[6];
  }
}

void sendData(String command, const int timeout, boolean debug)
{
  flag = "0";
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
    //Checking if GPS board is on.
    if (command == "\r\n+ATI\r\n" && (sizeof(response) > 5)){flag = 1;}
   // Serial.println(response);
    GPS.flush();
    return(response);
}
