#include <SoftwareSerial.h>

SoftwareSerial Arduino_SoftwareSerial(10, 11);

float voltBTotal, voltB1, voltB2, voltB3, voltB4, arusB1, arusB2, arusB3, arusB4;
String statusProses;

#include "ACS712.h"
#include <EEPROM.h>
#define sensorInput4 A9
#define sensorInput3 A10
#define sensorInput2 A11
#define sensorInput1 A12
ACS712 sensor1(ACS712_05B, sensorInput1);
ACS712 sensor2(ACS712_05B, sensorInput2);
ACS712 sensor3(ACS712_05B, sensorInput3);
ACS712 sensor4(ACS712_05B, sensorInput4);


float Arus1 = 0.0;
float Arus2 = 0.0;
float Arus3 = 0.0;
float Arus4 = 0.0;

int zero1 = 0;
int zero2 = 0;
int zero3 = 0;
int zero4 = 0;


//power charge cell relay
int power4 = 45; //relay cell 4 awal 39
int power3 = 43; //relay cell 3 awal 41
int power2 = 41; //relay cell 2 awal 43
int power1 = 39; //relay cell 1 awal 45

int relayshuntA = 31; //relay 1
int relayshuntB = 33; //relay 1
int relayshuntC = 35; //relay 1
int relayshuntD = 37; //relay 1

//batre shunt resistor discharge cell 1
const float BAT_LOW1 = 2.9;
const float BAT_FULL1 = 4.1;
const float BAT_HIGH1 = 4.5; //4.5 default

//batre shunt resistor discharge cell 2
const float BAT_LOW2 = 2.9;
const float BAT_FULL2 = 4.1;
const float BAT_HIGH2 = 4.5; //4.5 default

//batre shunt resistor discharge cell 2
const float BAT_LOW3 = 2.9;
const float BAT_FULL3 = 4.1;
const float BAT_HIGH3 = 4.5; //4.5 default

//batre shunt resistor discharge cell 3
const float BAT_LOW4 = 2.9;
const float BAT_FULL4 = 4.1;
const float BAT_HIGH4 = 4.5; //4.5 default


//sensor tegangan 0-25V
int analogPin1 = A1; //cell 4
int analogPin2 = A2; //cell 2
int analogPin3 = A3; //cell 3
int analogPin4 = A4; //cell 1

float R1 = 30000.0; //30k //resistor sensor tegangan
float R2 = 7500.0; //7500 ohm //resistor sensor tegangan

//cel 1
float Vmodul1 = 0.00;
float hasil1 = 0.00;
int value1 = 0;

//cell 2
float Vmodul2 = 0.00;
float hasil2 = 0.00;
int value2 = 0;

//cell 3
float Vmodul3 = 0.00;
float hasil3 = 0.00;
int value3 = 0;

//cell 4
float Vmodul4 = 0.00;
float hasil4 = 0.00;
int value4 = 0;

//cell Total Volt
float hasilT = 0.00;

int i = 0;
int step1 = 0; //discharge
int step2 = 0; //charge
int step3 = 0; //finish

//arus shunt Resistor cell 1
int sensorValue_Shunt_Resistor1 = A8;
float voltage1 = 0.00;
float current1 = 0.00;
int arus1 = 0;
float mA1 = 0.00;

//arus shunt Resistor cell 2
int sensorValue_Shunt_Resistor2 = A7;
float voltage2 = 0.00;
float current2 = 0.00;
int arus2 = 0;
float mA2 = 0.00;

//arus shunt Resistor cell 3
int sensorValue_Shunt_Resistor3 = A6;
float voltage3 = 0.00;
float current3 = 0.00;
int arus3 = 0;
float mA3 = 0.00;

//arus shunt Resistor cell 4
int sensorValue_Shunt_Resistor4 = A5;
float voltage4 = 0.00;
float current4 = 0.00;
int arus4 = 0;
float mA4 = 0.00;

int nocheck = 0;

void(*mulai_reset)(void)=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(relayshuntA, OUTPUT);
  pinMode(relayshuntB, OUTPUT);
  pinMode(relayshuntC, OUTPUT);
  pinMode(relayshuntD, OUTPUT);
  pinMode(power1, OUTPUT);
  pinMode(power2, OUTPUT);
  pinMode(power3, OUTPUT);
  pinMode(power4, OUTPUT);
   
  digitalWrite(relayshuntA, LOW);
  digitalWrite(relayshuntB, LOW);
  digitalWrite(relayshuntC, LOW);
  digitalWrite(relayshuntD, LOW);

  pinMode(sensorValue_Shunt_Resistor1, INPUT);
  pinMode(sensorValue_Shunt_Resistor2, INPUT);
  pinMode(sensorValue_Shunt_Resistor3, INPUT);
  pinMode(sensorValue_Shunt_Resistor4, INPUT);
  
  digitalWrite(power1, LOW);
  digitalWrite(power2, LOW);
  digitalWrite(power3, LOW);
  digitalWrite(power4, LOW);
  
  pinMode(analogPin1, INPUT);
  pinMode(analogPin2, INPUT);
  pinMode(analogPin3, INPUT);
  pinMode(analogPin4, INPUT);

  int zero1 = sensor1.calibrate();
  int zero2 = sensor2.calibrate();
  int zero3 = sensor3.calibrate();
  int zero4 = sensor4.calibrate();  
  
  Serial.begin(115200);

  Arduino_SoftwareSerial.begin(115200);
}

void data_volt_cell(){
  digitalWrite(relayshuntA, LOW);
  digitalWrite(relayshuntB, LOW);
  digitalWrite(relayshuntC, LOW);
  digitalWrite(relayshuntD, LOW);  
  digitalWrite(power1, LOW);
  digitalWrite(power2, LOW);
  digitalWrite(power3, LOW);
  digitalWrite(power4, LOW); 

  delay(3000);
  
  //volt cell 1
  value1 = analogRead(analogPin1);
  Vmodul1 = (value1 * 5.0) / 1024.0;
  hasil1 = Vmodul1 * ((R1+R2)/R2);

  //volt cell 2
  value2 = analogRead(analogPin2);
  Vmodul2 = (value2 * 5.0) / 1024.0;
  hasil2 = Vmodul2 * ((R1+R2)/R2);
  
  //volt cell 3
  value3 = analogRead(analogPin3);
  Vmodul3 = (value3 * 5.0) / 1024.0;
  hasil3 = Vmodul3 * ((R1+R2)/R2);
  
  //volt cell 4
  value4 = analogRead(analogPin4);
  Vmodul4 = (value4 * 5.0) / 1024.0;
  hasil4 = Vmodul4 * ((R1+R2)/R2);

  //volt cell total
  hasilT = hasil1 + hasil2 + hasil3 + hasil4;

}

void data_arus_cell_discharge(){
  //arus discharge cell 1
  arus1 = analogRead(sensorValue_Shunt_Resistor1);
  voltage1 = arus1 *(5.00 / 1024.0);
  current1 = voltage1/Resistor1;

  mA1 = current1 * 1000.0;

  //arus discharge cell 2
  arus2 = analogRead(sensorValue_Shunt_Resistor2);
  voltage2 = arus2 *(5.00 / 1024.0);
  current2 = voltage2/Resistor2;

  mA2 = current2 * 1000.0;

  //arus discharge cell 1
  arus3 = analogRead(sensorValue_Shunt_Resistor3);
  voltage3 = arus3 *(5.00 / 1024.0);
  current3 = voltage3/Resistor1;

  mA3 = current1 * 1000.0;

  //arus discharge cell 4
  arus4 = analogRead(sensorValue_Shunt_Resistor4);
  voltage4 = arus4 *(5.00 / 1024.0);
  current4 = voltage4/Resistor4;

  mA4 = current4 * 1000.0;
}

void loop() {
  
  float I1 = sensor1.getCurrentDC();
  float I2 = sensor2.getCurrentDC();
  float I3 = sensor3.getCurrentDC();
  float I4 = sensor4.getCurrentDC();


  for (i = 0; i < 10; i++)
  {
    data_volt_cell();
          step1 == 1; 
//    Serial.println("Check Cell......");
    Serial.print("Hasil pengukuran B1 = ");
    Serial.print(hasil1, 2);
    Serial.print("volt");
    Serial.print("Hasil pengukuran B2 = ");
    Serial.print(hasil2, 2);
    Serial.print("volt");
    Serial.print(",  Hasil pengukuran B3 = ");
    Serial.print(hasil3,2);
    Serial.print("volt");
    Serial.print("Hasil pengukuran B4 = ");
    Serial.print(hasil4,2);
    Serial.print("volt");
    Serial.print(",  Arus Pengisian B2 = ");
    Serial.print(I2,2);
    Serial.print("A");
    Serial.print(",  Arus Pengisian B3 = ");
    Serial.print(I3,2);
    Serial.print("A");
    Serial.print(",  Arus Pengisian B4 = ");
    Serial.println(I4,2);
        
        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Checking  ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K");
    
    delay(1000);

  if(hasil1 >= BAT_FULL1 && hasil1 <= BAT_HIGH1 && hasil2 >= BAT_FULL2 && hasil2 <= BAT_HIGH2 && hasil3 >= BAT_FULL3 && hasil3 <= BAT_HIGH3 && hasil4 >= BAT_FULL4 && hasil4 <= BAT_HIGH4)
  {
    step3 == 1;
  }

for (step1 = 1; step1 < 2;)
  {
    data_volt_cell();
    data_arus_cell_discharge();

    if (hasil1 > BAT_LOW1 && hasil2 > BAT_LOW2 && hasil3 > BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);      // Menyalakan hubungan batre ke beban agar batre mengalami discharge
        digitalWrite(relayshuntB, HIGH);      
        digitalWrite(relayshuntC, HIGH);      
        digitalWrite(relayshuntD, HIGH);       
       
        Serial.print("B Total = ");
        Serial.print(hasilT,2);
        Serial.print("V");
        Serial.print(",  Arus Pengurangan B1 = ");
        Serial.print(mA1,2);
        Serial.print(" mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Arus Pengurangan B2 = ");
        Serial.print(mA2,2);
        Serial.print(" mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Arus Pengurangan B3 = ");
        Serial.print(mA3,2);
        Serial.print(" mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Arus Pengurangan B4 = ");
        Serial.print(mA4,2);
        Serial.print(" mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 

        delay(5000);      
    }

//////////////////////////////////////////LOGIKA CELL 1/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (hasil1 < BAT_LOW1 && hasil2 > BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);       // Hanya batre 1 yang tidak mengalami discharge, baterai lain terhubung ke shunt.
        digitalWrite(relayshuntB, HIGH);       
        digitalWrite(relayshuntC, HIGH);       
        digitalWrite(relayshuntD, HIGH);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);      
    }

    else if (hasil1 < BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);       // Baterai 1 dan 2 tidak terdischarge
        digitalWrite(relayshuntB, LOW);       
        digitalWrite(relayshuntC, HIGH);       
        digitalWrite(relayshuntD, HIGH);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);  
    }

    else if (hasil1 < BAT_LOW1 && hasil2 > BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);       //Baterai 1 dan 3 tidak terdischarge
        digitalWrite(relayshuntB, HIGH);    
        digitalWrite(relayshuntC, LOW);     
        digitalWrite(relayshuntD, HIGH);    


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);  
    }

    else if (hasil1 < BAT_LOW1 && hasil2 > BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);       // Baterai 1 dan 4 tidak terdischarge
        digitalWrite(relayshuntB, HIGH);       
        digitalWrite(relayshuntC, HIGH);     
        digitalWrite(relayshuntD, LOW);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");            Arduino_SoftwareSerial.print("K");
        
        delay(5000);  
    }
//////////////CELL 2//////////////////////////////////////////////////
    else if (hasil1 > BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);       // Baterai 2 tidak terdischarge
        digitalWrite(relayshuntB, LOW);       
        digitalWrite(relayshuntC, HIGH);      
        digitalWrite(relayshuntD, HIGH);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);  
    }

    else if (hasil1 > BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);       // Baterai 2 dan 3 tidak terdischarge
        digitalWrite(relayshuntB, LOW);       
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, HIGH);     


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);  
    }

    else if (hasil1 > BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);     //Baterai 2 dan 4 tidak terdischarge
        digitalWrite(relayshuntB, LOW);       
        digitalWrite(relayshuntC, HIGH);      
        digitalWrite(relayshuntD, LOW);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");            Arduino_SoftwareSerial.print("K");
        
        delay(5000);  
    }
///////////////////////////////CELL 3///////////////////////////////////////////////////////////////////////
    else if (hasil1 > BAT_LOW1 && hasil2 > BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);     // Baterai 3 tidak terdischarge
        digitalWrite(relayshuntB, HIGH);     
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, HIGH);     


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);  
    }

    else if (hasil1 > BAT_LOW1 && hasil2 > BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);     // Baterai 3 dan 4 tidak terdischarge
        digitalWrite(relayshuntB, HIGH);       
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, LOW);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);  
    }
/////////////////////////////CELL 4////////////////////////////////////////////////////////////////
    else if (hasil1 > BAT_LOW1 && hasil2 > BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);     // Baterai 4 tidak terdischarge
        digitalWrite(relayshuntB, HIGH);       
        digitalWrite(relayshuntC, HIGH);      
        digitalWrite(relayshuntD, LOW);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K");
        
        delay(5000);  
    }

//////////////////////////////////////////LOGIKA CELL 1, CELL 2, CELL 3/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (hasil1 < BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 > BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);       // Baterai 1,2,3 tidak di discharge
        digitalWrite(relayshuntB, LOW);       
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, HIGH);     


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Capacity B4 = ");
        Serial.print(mA4,2);
        Serial.print("mA");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);      
    }

//////////////////////////////////////////LOGIKA CELL 1, CELL 2, CELL 4/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (hasil1 < BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);      //Baterai 1,2,4 tidak terdischarge
        digitalWrite(relayshuntB, LOW);      
        digitalWrite(relayshuntC, HIGH);     
        digitalWrite(relayshuntD, LOW);      

        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Capacity B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);      
    }

//////////////////////////////////////////LOGIKA CELL 1, CELL 3, CELL 4/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (hasil1 < BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 > BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);       // Baterai 1,3,4 tidak terdischarge
        digitalWrite(relayshuntB, HIGH);       
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, LOW);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Capacity B2 = ");
        Serial.print(mA2,2);
        Serial.print("mA");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");            Arduino_SoftwareSerial.print("K");
        
        delay(5000);      
    }

//////////////////////////////////////////LOGIKA CELL 2, CELL 3, CELL 4/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    else if (hasil1 > BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, HIGH);     // Baterai 2,3,4 tidak terdischarge
        digitalWrite(relayshuntB, LOW);      
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, LOW);      


        Serial.print("B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Capacity B1 = ");
        Serial.print(mA1,2);
        Serial.print("mA");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(mA3,2);
        Serial.print("mA");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K"); 
        
        delay(5000);      
    }

////////////////////////LOGIKA MENUJU CHARGE///////////////////////////////////////////////////////////////    
    else if (hasil1 < BAT_LOW1 && hasil2 < BAT_LOW2 &&  hasil3 < BAT_LOW3 && hasil4 < BAT_LOW4)
    {
        digitalWrite(relayshuntA, LOW);      // Seluruh baterai tidak mengalami discharge
        digitalWrite(relayshuntB, LOW);      
        digitalWrite(relayshuntC, LOW);      
        digitalWrite(relayshuntD, LOW);      

        Serial.print(",  B Capacity Total = ");
        Serial.print(Capacity1 + Capacity2 + Capacity3 + Capacity4,2);
        Serial.print("mAH");
        Serial.print(",  Waiting B1 = ");
        Serial.print(",  B1 = ");
        Serial.print(hasil1,2);
        Serial.print("V");
        Serial.print(",  Waiting B2 = ");
        Serial.print(",  B2 = ");
        Serial.print(hasil2,2);
        Serial.print("V");
        Serial.print(",  Waiting B3 = ");
        Serial.print(",  B3 = ");
        Serial.print(hasil3,2);
        Serial.print("V");
        Serial.print(",  Waiting B4 = ");
        Serial.print(", B4 = ");
        Serial.print(hasil4,2);
        Serial.println("V");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = current1;
        arusB2 = current2;
        arusB3 = current3;
        arusB4 = current4;
        statusProses = "Balancing ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K");
        
        delay(5000);
        step2 == 1;
        break;        
    }

  }

for (step2 = 1; step2 < 2;)
  {
      data_volt_cell();
      digitalWrite(relayshuntA, LOW);
      digitalWrite(relayshuntB, LOW);
      digitalWrite(relayshuntC, LOW);
      digitalWrite(relayshuntD, LOW);
      if (hasil1 < BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, LOW);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, LOW);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, LOW);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, LOW);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, LOW);
        digitalWrite(power3, LOW);
        digitalWrite(power4, HIGH);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, LOW);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, LOW);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 < BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, HIGH);
        digitalWrite(power2, LOW);
        digitalWrite(power3, LOW);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 < BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, HIGH);
        digitalWrite(power3, LOW);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 < BAT_FULL3 && hasil4 >= BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, LOW);
        digitalWrite(power3, HIGH);
        digitalWrite(power4, LOW);
      }
      else if (hasil1 >= BAT_FULL1 && hasil2 >= BAT_FULL2 && hasil3 >= BAT_FULL3 && hasil4 < BAT_FULL4)
      {
        digitalWrite(power1, LOW);
        digitalWrite(power2, LOW);
        digitalWrite(power3, LOW);
        digitalWrite(power4, HIGH);
      }
      else if ( hasil1 >= BAT_HIGH1 || hasil2 >= BAT_HIGH2 || hasil3 >= BAT_HIGH3 || hasil4 >= BAT_HIGH4)
      {
        step2 == 0;
        step1 == 1;
        break;
      }
      float i1 = sensor1.getCurrentDC();
      float i2 = sensor2.getCurrentDC();
      float i3 = sensor1.getCurrentDC();
      float i4 = sensor2.getCurrentDC();
      float I1 = i1 - zero1;
      float I2 = i2 - zero2;
      float I3 = i3 - zero3;
      float I4 = i4 - zero4;
      Serial.print("Hasil pengukuran B1 = ");
      Serial.print(hasil1,2);
      Serial.print("Hasil pengukuran B2 = ");
      Serial.print(hasil2,2);
      Serial.print("volt");
      Serial.print(",  Arus Pengisian B2 = ");
      Serial.print(I2,2);
      Serial.print("A");
      Serial.print(",  Hasil pengukuran B3 = ");
      Serial.print(hasil3,2);
      Serial.print("volt");
      Serial.print(",  Arus Pengisian B3 = ");
      Serial.print(I3,2);
      Serial.print("A");
      Serial.print("Hasil pengukuran B4 = ");
      Serial.print(hasil4,2);
      Serial.print("volt");
      Serial.print(",  Arus Pengisian B4 = ");
      Serial.print(I4,2);
      Serial.print("A");
      Serial.println(",  Charge Batre");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = I1;
        arusB2 = I2;
        arusB3 = I3;
        arusB4 = I4;
        statusProses = "Charging  ";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K");
      
        delay(5000);
                  

        if(hasil1 >= BAT_FULL1 && hasil1 < BAT_HIGH1 && hasil2 >= BAT_FULL2 &&  hasil2 < BAT_HIGH2 && hasil3 >= BAT_FULL3 && hasil3 < BAT_HIGH3 && hasil4 >= BAT_FULL4 && hasil4 < BAT_HIGH4) 
        { // Memeriksa keadaan baterai dalam keadaan batas aman
            step1 == 3;
            step2 == 3;
            step3 == 1;
            break;
        }
  }

  for (step3 = 1; step3 < 2;)
  {
      data_volt_cell();
      digitalWrite(relayshuntA, LOW);
      digitalWrite(relayshuntB, LOW);
      digitalWrite(relayshuntC, LOW);
      digitalWrite(relayshuntD, LOW);
      digitalWrite(power1, LOW);
      digitalWrite(power2, LOW);
      digitalWrite(power3, LOW);
      digitalWrite(power4, LOW);
      if ( hasil1 >= BAT_HIGH1 || hasil2 >= BAT_HIGH2 || hasil3 >= BAT_HIGH3 || hasil4 >= BAT_HIGH4)
      {
        step3 == 0;
        step2 == 0;
        step1 == 1;
        break;
      }
      float i1 = sensor1.getCurrentDC();
      float i2 = sensor2.getCurrentDC();
      float i3 = sensor1.getCurrentDC();
      float i4 = sensor2.getCurrentDC();
      float I1 = i1 - zero1;
      float I2 = i2 - zero2;
      float I3 = i3 - zero3;
      float I4 = i4 - zero4;

      Serial.print("Hasil pengukuran B1 = ");
      Serial.print(hasil1,2);
      Serial.print("volt");
      Serial.print("Hasil pengukuran B2 = ");
      Serial.print(hasil2,2);
      Serial.print("volt");
      Serial.print(", Hasil pengukuran B3 = ");
      Serial.print(hasil3,2);
      Serial.print("volt");
      Serial.print(", Hasil pengukuran B4 = ");
      Serial.print(hasil4,2);
      Serial.print("volt");
      Serial.println(",  FULL Batre");

        voltBTotal = hasilT;
        voltB1 = hasil1;
        voltB2 = hasil2;
        voltB3 = hasil3;
        voltB4 = hasil4;
        arusB1 = I1;
        arusB2 = I2;
        arusB3 = I3;
        arusB4 = I4;
        statusProses = "FULL Batre";
      
        Arduino_SoftwareSerial.print(voltBTotal, 2);  Arduino_SoftwareSerial.print("A");
        Arduino_SoftwareSerial.print(voltB1, 2);      Arduino_SoftwareSerial.print("B");
        Arduino_SoftwareSerial.print(voltB2, 2);      Arduino_SoftwareSerial.print("C");
        Arduino_SoftwareSerial.print(voltB3, 2);      Arduino_SoftwareSerial.print("D");
        Arduino_SoftwareSerial.print(voltB4, 2);      Arduino_SoftwareSerial.print("E");
        Arduino_SoftwareSerial.print(arusB1, 2);      Arduino_SoftwareSerial.print("F");
        Arduino_SoftwareSerial.print(arusB2, 2);      Arduino_SoftwareSerial.print("G");
        Arduino_SoftwareSerial.print(arusB3, 2);      Arduino_SoftwareSerial.print("H");
        Arduino_SoftwareSerial.print(arusB4, 2);      Arduino_SoftwareSerial.print("I");
        Arduino_SoftwareSerial.print(statusProses);   Arduino_SoftwareSerial.print("J");
        Arduino_SoftwareSerial.print("\n");           Arduino_SoftwareSerial.print("K");
        
      delay(5000);
      step1 == 3;
      step2 == 3;
      step3 == 3;
      nocheck == 0;
      break;
  }
  

}
