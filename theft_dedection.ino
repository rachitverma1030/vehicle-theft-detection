//For storing and display mobile number on LED & off/on of the motor 

#include <LiquidCrystal.h>
#include <EEPROM.h>


LiquidCrystal lcd(7,6,5,4,3,2);
String senderMobile = "";
String userMobile = "";
String smsString = "Vechile Theft Detection Found"; 
bool callMode = false;
bool gsmFound = false;
bool theft = false;
bool theftOk = false;
bool up = false;
bool setupOk = false;
#define callModeLed 10
#define setNumber  A2
#define theftMode A0
#define motor 9
#define motorSense A1


void setup() {
delay(200);
pinMode(setNumber,INPUT_PULLUP);
pinMode(theftMode,INPUT_PULLUP);
pinMode(motor,OUTPUT);
pinMode(motorSense,INPUT_PULLUP);

digitalWrite(motor,HIGH);
pinMode(callModeLed,OUTPUT);
digitalWrite(callModeLed,LOW);  
Serial.begin(9600);
lcd.begin(16,2);
lcd.clear();
lcd.home();
  Serial.println("Vechile Theft ");
  lcd.setCursor(0,0);
  lcd.print("Wait For Ready...");
  delay(5000);

  lcd.setCursor(0,1);
  lcd.print("Wait For Gsm     ");
  delay(1000);
  lcd.setCursor(0,1);

while(gsmFound == false){
  gsmFound = initGsm();
  delay(1000);  
}
  lcd.clear();
  lcd.home();
  lcd.print("Gsm Found");
  delay(2000);

  while(setupOk == false){
    setupOk = setupGsm();
    delay(1000);
  }
  
  lcd.clear();
  lcd.home();
  lcd.print("Gsm Setup Ok");
  delay(2000);

  userMobile = readStringFromEEPROM(0);
  lcd.setCursor(0,1);
  lcd.print(userMobile);
  delay(2000);

if (digitalRead(setNumber) == false){
  callMode = true;
  digitalWrite(callModeLed,HIGH);
  lcd.clear();
  lcd.home();
  lcd.print("Please Call");
  delay(2000);

}

  
}

void loop() {
//if call mode is true then user can set mobile number
delay(100);
if (callMode == true){
  //bool ok = Serial.find("+CLIP:");
  bool ok = false;
  
  while(ok == false){
   ok = Serial.find("+91"); 
  }
   
  String userMobile = "";
  Serial.println(ok);
  delay(1000);
  for (int i = 0;i <10; i ++){
    char c= Serial.read();
    userMobile = userMobile + c;
    delay(10);
  }
  userMobile = "+91"+ userMobile;
  Serial.println(userMobile);
  writeStringToEEPROM(0,userMobile);
  lcd.setCursor(0,1);
  lcd.print(userMobile);
  while(true){
    digitalWrite(callModeLed,!digitalRead(callModeLed));
    delay(100);
  }
 
}
  
// if theft mode enabled
if (digitalRead(theftMode) == false){
  theft = true;
  lcd.clear();
  lcd.home();
  lcd.print("Theft Mode");
  up = false;
}else{
  theft = false;
  if (up == false){
    up = true;
    lcd.clear();
  lcd.home();
  lcd.print("Normal Mode");
  }
  
}



// if theft mode is true & motor is on then send sms to predefine number
if ((theft == true) && (digitalRead(motorSense) == false)){
  //lcd.clear();
  //lcd.home();
  //lcd.print("Theft Mode");
  //Serial.println("theft mode");
  bool smsOk = false;
  while (smsOk == false){
   smsOk = sendSms(userMobile,smsString);
  }
  lcd.setCursor(0,1);
  lcd.print("Message Sent");
  
  //message sent flash led 5 times
  for (int i =0; i < 10; i ++){
    digitalWrite(callModeLed,!digitalRead(callModeLed));
    delay(500);
  }
    lcd.setCursor(0,1);
    digitalWrite(callModeLed,LOW);
checkAgain:
    theftOk = false;    
// now check user message, wait for +91
    while(theftOk == false){
   theftOk = Serial.find("+91"); 
  }

  senderMobile = "";
  // now receive mobile number

  delay(1000);
  for (int i = 0;i <10; i ++){
    char c= Serial.read();
    senderMobile = senderMobile + c;
    delay(10);
  }

  senderMobile = "+91" + senderMobile;

  if (senderMobile == userMobile){
    lcd.print("matched         ");
  } else{
    lcd.print("not matched     ");
    delay(2000);
    lcd.setCursor(0,1);
    lcd.print("                ");
    goto checkAgain;
  }

  digitalWrite(motor,LOW);
  while(true){
    delay(1000);
    digitalWrite(callModeLed,!digitalRead(callModeLed));
  }
  
  
}



}
