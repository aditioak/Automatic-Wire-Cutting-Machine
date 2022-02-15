//------------------------------- libraries ----------------------------------

#include <LiquidCrystal.h>
#include <Servo.h>
 
//------------------------------- lcd ----------------------------------
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
//------------------------------- stepper ----------------------------------
#define stepPin 12
#define dirPin 13
 
//------------------------------- servo ----------------------------------
//Servo snippers;
#define servo 10
//#define openAngle 180
//#define closedAngle 0
 
//------------------------------- input ----------------------------------
 
#define leftButton 8
#define rightButton A0
#define upButton 9
#define downButton 11
 
//------------------------------- user settings ----------------------------------
unsigned int wireLength = 0;
unsigned int wireQuantity = 0;
 
//------------------------------- system settings ----------------------------------
int state = 0;
int incrementSpeed = 10;
int previousWireLength = 0;
int previousWireQuantity = 0;
float mmPerStep = 0.18096;
 
 
void setup() {
  Serial.begin(9600);
 
 
  lcd.begin(16, 2); //LCD columns and rows

  pinMode(servo,OUTPUT); 
 
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
 
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
 
  //snippers.attach(servo);
 
   
  delay(1000);
}
 
void loop() {
  if (!digitalRead(rightButton)){
    if(state == 5){
      state = 0;
    }
    else{
      state += 1;
    }
    delay(200);
    lcd.clear();
  }
  if (!digitalRead(leftButton) && state > 0 && state < 4){
    state -=1;
    delay(200);
    lcd.clear();
  }
 
 
  switch (state){
    case 0:
      homeScreen();
      break;
    case 1:
       chooseWireLength();
       break;
    case 2:
      chooseWireQuantity();
      break;
    case 3:
      confirm();
      break;
    case 4:
      currentlyCutting();
      break;
    case 5:
      finishedCutting();
      break;
  }
 
}
 
 
void homeScreen(){
  lcd.setCursor(0, 0);
  lcd.print("WIRE NEW");
  lcd.setCursor(11, 1);
  lcd.print("NEXT>");
  delay(100);
}
 
 
void chooseWireLength(){
  wireLength = changeValue(wireLength);
 
  //clear LCD if required
  if(previousWireLength != wireLength){
    lcd.clear();
    previousWireLength = wireLength;
  }
 
  //Display information on LCD
  lcd.setCursor(0, 0);
  lcd.print("LENGTH:" + (String)wireLength + "mm");
  displayNavigation();
}
 
void chooseWireQuantity(){
  wireQuantity = changeValue(wireQuantity);
 
  //clear LCD if required
  if(previousWireQuantity != wireQuantity){
    lcd.clear();
    previousWireQuantity = wireQuantity;
  }
 
  //Display information on LCD
  lcd.setCursor(0, 0);
  lcd.print("QUANTITY:" + (String)wireQuantity);
  displayNavigation();
}
 
void confirm(){
  lcd.setCursor(0, 0);
  lcd.print((String)wireLength + "mm x " + (String)wireQuantity + "pcs");
  lcd.setCursor(0, 1);
  lcd.print("<BACK");
  lcd.setCursor(10, 1);
  lcd.print("START>");
  delay(100);
}
 
void currentlyCutting(){
  lcd.setCursor(0, 0);
  lcd.print((String)0 + "/" + (String)wireQuantity);
  lcd.setCursor(0, 1);
  lcd.print("???s");
  int stepsToTake = (int)wireLength/mmPerStep;
  for(int i = 0; i < wireQuantity; i++){
    unsigned long timeForOneCycle = millis();
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x < stepsToTake; x++) {
      digitalWrite(stepPin,HIGH);
      delayMicroseconds(500);
      digitalWrite(stepPin,LOW);
      delayMicroseconds(500);
    }
   
    lcd.setCursor(0, 0);
    lcd.print((String)(i+1) + "/" + (String)wireQuantity);

  digitalWrite(servo,HIGH);
  //delayMicroseconds(1000);
  delayMicroseconds(800);
  Serial.println("Check");
  digitalWrite(servo,LOW);
  delay(20);
  
    //snippers.write(openAngle);
    //Serial.println("Yes");
    //delay(600);
    //snippers.write(closedAngle);
    //Serial.println("Works");
    delay(600);
   
    lcd.setCursor(0, 1);
 
    unsigned long timeRemaining = ((millis() - timeForOneCycle)*(wireQuantity - (i+1)))/1000;
    lcd.print((String)timeRemaining + "s    ");
   
  }
  wireLength = 0;
  wireQuantity = 0;
  state = 5;
}
 
void finishedCutting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CUTTING COMPLETE");
  lcd.setCursor(11, 1);
  lcd.print("NEXT>");
  delay(100);
}
 
 
int changeValue(int currentValue){
  if (!digitalRead(upButton)) {
    delay(100);
    currentValue += incrementSpeed;
    
  }
  if (!digitalRead(downButton)) {
    if(currentValue - incrementSpeed >= 0){
      delay(100);
      currentValue -= incrementSpeed;
    
    }
    else{
      currentValue = 0;
    }
  }
  if (!digitalRead(downButton) && !digitalRead(upButton)){
    incrementSpeed = 10;
  }
  return currentValue;
}
 
void displayNavigation(){
  lcd.setCursor(0, 1);
  lcd.print("<BACK");
  lcd.setCursor(11, 1);
  lcd.print("NEXT>");
  delay(100);
}
