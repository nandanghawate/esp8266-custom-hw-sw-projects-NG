#include <Servo.h>
#include <EEPROM.h>
#define boardLED 2 //D4
#define redButton 14 //D5
#define greenButton 12 //D6
#define servoOut 5 //D1

Servo Servo1;
#define servoOn 180
#define servoOff 0

bool redState = 0;
bool greenState = 0;
bool prev_redState = 0;
bool prev_greenState = 0;
bool firstBoot = 0;
int addr = 0;
byte value;
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  Serial.println("");
  Serial.println("Motor 1 Test");
  pinMode(boardLED, OUTPUT);
  pinMode(redButton, INPUT_PULLUP);
  pinMode(greenButton, INPUT_PULLUP);

  // Avoid Randomly Vibrating when turning on
  Servo1.attach(servoOut,700,2800);
  //Servo1.attach(servoOut);
  digitalWrite(boardLED, HIGH);
  Serial.println("Servo Off");
  delay(200);
  firstBoot=1;
  prev_redState=!redState;
  prev_greenState=!greenState;
  
  value = EEPROM.read(addr);

  Serial.print(addr);
  Serial.print("\t");
  Serial.print(value, DEC);
  Serial.println();
  
  Serial.print("init Stage : ");
  Servo1.attach(servoOut,700,2800);
  if(value==1){
    Servo1.write(servoOn);
    Serial.println("servo On");
  }
  else if(value==2){
    Servo1.write(servoOff);
    Serial.println("servo Off");
  }
  
}

void loop() {
  if ((digitalRead(redButton)==0)&&(redState!=prev_redState)){
    //while(digitalRead(redButton)==0);
    Serial.println("Servo On");
    Servo1.write(servoOn);
    for(int i=0;i<3;i++)
    {
      digitalWrite(boardLED, LOW);
      delay(200);
      digitalWrite(boardLED, HIGH);
      delay(200);
    }
    prev_redState=redState;
    if(firstBoot==0)greenState=!greenState;
    firstBoot=0;
    EEPROM.write(addr, 1);
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }
  }
  
  if ((digitalRead(greenButton)==0)&&(prev_greenState!=greenState)){
    //while(digitalRead(greenButton)==0);
    Serial.println("Servo Off");
    Servo1.write(servoOff);
    for(int i=0;i<3;i++)
    {
      digitalWrite(boardLED, LOW);
      delay(200);
      digitalWrite(boardLED, HIGH);
      delay(200);
    }
    prev_greenState=greenState;
    if(firstBoot==0)redState=!redState;
    firstBoot=0;
    EEPROM.write(addr, 2);
    if (EEPROM.commit()) {
      Serial.println("EEPROM successfully committed");
    } else {
      Serial.println("ERROR! EEPROM commit failed");
    }
  }
   

}
