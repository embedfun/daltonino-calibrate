// base http://circuitdigest.com/microcontroller-projects/arduino-color-sensor-tcs3200
//  20161125  PM  modifier ports SPI pour "nouveau" MEGA (pourquoi ça marchait avant comme UNO ???)
// ne pas tenter une conversion RGB, juste reconnaitre selon periode

int OutPut = 9; //input for pulseIn
const int s0 = 2, s1 = 3, s2 = 4, s3 = 5;

int rowR, rowG, rowB, rowL;
//char valRal[] = "0000";
#include <SD.h>
#include <SPI.h>
const int pinSD = 53; // Mega SS=53
File myFile;
void setup()
{
  Serial.begin(9600);
  Serial.println("Daltonino /Calibrate ");


  Serial.print("InitSD");
  pinMode(pinSD, OUTPUT);
  if (!SD.begin(pinSD)) {
    Serial.println("initfailed!");
    return;
  }
  Serial.println("open...");
  myFile = SD.open("valRal.csv", FILE_WRITE);
  // pins mode
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(OutPut, INPUT);
  // frequency scaling
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH); //2% = plus grands écarts donc intéressant)
}

int  getChannel(int ps2, int ps3) {
  unsigned int demiPeriode = 0;
  digitalWrite(s2, ps2);
  digitalWrite(s3, ps3); //setting for  color sensor
  demiPeriode = pulseIn(OutPut, LOW);//reading demi période
  delay(50);
  return demiPeriode;
}

void loop()
{

  rowR = getChannel(LOW, LOW);
  rowG = getChannel(HIGH, HIGH);
  rowB = getChannel(LOW, HIGH);
  //rowL = getChannel(HIGH, LOW);
  Serial.print("R:");
  Serial.print(rowR);
  Serial.print(" G:");
  Serial.print(rowG);
  Serial.print(" B:");
  Serial.println(rowB);

  // luminosité
  //rowL = getChannel(HIGH, LOW);
  if (Serial.available() > 0) {
    String cle = Serial.readStringUntil('\n');
    myFile.print(rowR);
    myFile.print(";");
    myFile.print(rowG);
    myFile.print(";");
    myFile.print(rowB);
    myFile.print(";");
    myFile.println(cle);
    myFile.flush();
    Serial.println("Enr. ok");
  }
  Serial.println("====================");
  delay(1000);
}


