#include <SoftwareSerial.h>
#include <AntaresESP8266HTTP.h>

SoftwareSerial NodeMcu_SoftwareSerial(D1, D2);

#define ACCESSKEY "b02502c8391d6909:2440d8823e58bd26"
#define projectName "MonitoringBalancing"
#define deviceName "MonitoringBalancing"

#define WIFISSID "Feby"
#define PASSWORD "KingKongKong333"

char c;
String dataIn;
int8_t indexOfA, indexOfB, indexOfC, indexOfD, indexOfE, 
       indexOfF, indexOfG, indexOfH, indexOfI, indexOfJ, indexOfK;

String voltBTotal, voltB1, voltB2, voltB3, voltB4, 
       arusB1, arusB2, arusB3, arusB4,
       statusProses, enter;

AntaresESP8266HTTP antares(ACCESSKEY);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  
  NodeMcu_SoftwareSerial.begin(115200);

  WiFi.begin(WIFISSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
//    digitalWrite(LED_PIN, LOW);
  }

//  digitalWrite(LED_PIN, HIGH);

  antares.setDebug(true);
  antares.wifiConnection(WIFISSID, PASSWORD);

}

void loop() {
  // put your main code here, to run repeatedly:

  while(NodeMcu_SoftwareSerial.available()>0)
    {
      c = NodeMcu_SoftwareSerial.read();

      if(c=='K') {break;}
      else        {dataIn+=c;}
    }

   if(c=='K')
    {
      Parse_the_Data();

      Serial.println("Volt BTotal   = " + voltBTotal);
      Serial.println("Volt B1       = " + voltB1);
      Serial.println("Volt B2       = " + voltB2);
      Serial.println("Volt B3       = " + voltB3);
      Serial.println("Volt B4       = " + voltB4);
      Serial.println("Arus B1       = " + arusB1);
      Serial.println("Arus B2       = " + arusB2);
      Serial.println("Arus B3       = " + arusB3);
      Serial.println("Arus B4       = " + arusB4);
      Serial.println("Status        = " + statusProses);

      antares.add("Volt BTotal    = ", voltBTotal);
      antares.add("Volt B1        = ", voltB1);
      antares.add("Volt B2        = ", voltB2);
      antares.add("Volt B3        = ", voltB3);
      antares.add("Volt B4        = ", voltB4);
      antares.add("Arus B1        = ", arusB1);
      antares.add("Arus B2        = ", arusB2);
      antares.add("Arus B3        = ", arusB3);
      antares.add("Arus B4        = ", arusB4);
      antares.add("Status         = ", statusProses);

      antares.send(projectName, deviceName);

      delay(10000);
      c=0;
      dataIn="";
    }
}

void Parse_the_Data ()
{
  indexOfA = dataIn.indexOf("A");
  indexOfB = dataIn.indexOf("B");
  indexOfC = dataIn.indexOf("C");
  indexOfD = dataIn.indexOf("D");
  indexOfE = dataIn.indexOf("E");
  indexOfF = dataIn.indexOf("F");
  indexOfG = dataIn.indexOf("G");
  indexOfH = dataIn.indexOf("H");
  indexOfI = dataIn.indexOf("I");
  indexOfJ = dataIn.indexOf("J");
  indexOfK = dataIn.indexOf("K");

  voltBTotal        = dataIn.substring (indexOfA-5, indexOfA);
  voltB1            = dataIn.substring (indexOfB-4, indexOfB);
  voltB2            = dataIn.substring (indexOfC-4, indexOfC);  
  voltB3            = dataIn.substring (indexOfD-4, indexOfD);
  voltB4            = dataIn.substring (indexOfE-4, indexOfE); 
  arusB1            = dataIn.substring (indexOfF-4, indexOfF);
  arusB2            = dataIn.substring (indexOfG-4, indexOfG);
  arusB3            = dataIn.substring (indexOfH-4, indexOfH);
  arusB4            = dataIn.substring (indexOfI-4, indexOfI);
  statusProses      = dataIn.substring (indexOfJ-10, indexOfJ);
  enter             = dataIn.substring (indexOfJ+1, indexOfK);
 
}
