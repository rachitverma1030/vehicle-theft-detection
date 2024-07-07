//For GSM to receive & send messages to vehicle’s owner 

void writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
}

String readStringFromEEPROM(int addrOffset)
{
int newStrLen = EEPROM.read(addrOffset);
char data[newStrLen + 1];
for (int i = 0; i < newStrLen; i++)
{
data[i] = EEPROM.read(addrOffset + 1 + i);
}
data[newStrLen] = '\0';
return String(data);
}

boolean initGsm(){
  
  boolean ok = false;  
  // set timeout

  Serial.setTimeout(2000);
  Serial.println("AT\r\n");
  ok = Serial.find("OK");
  delay(100);
  
  if (ok == false){
    return false;
  }
  
  ok = false;
  
  Serial.println("ATE0\r\n");
  ok = Serial.find("OK");
  delay(100);
  if (ok == false){return false;}
  ok = false;
  
  //Serial.println("AT+CNMI=2,2,0,0,0");
  //ok = Serial.find("OK");
  //if (ok == false){return false;}
  //ok = false;
    
  Serial.println("AT+CMGF=1\r\n");
  ok = Serial.find("OK");
  delay(100);
  if (ok == false){
    return false;
  }
  
  
  if (ok == false){return false;}
  else{return true;} 
}

bool setupGsm(){
  bool ok = false;
  Serial.println("AT+CNMI=2,2,0,0,0");
  ok = Serial.find("OK");
  if (ok == false){return false;}
  delay(2000);
  Serial.println("AT+CLIP=1\r\n");
  ok = Serial.find("OK");
  return ok;
}
//////////////////////
boolean sendSms(String t, String m){
 boolean ok = false;
 String mobile = t;
 String sms = m;
 
  Serial.setTimeout(2000);
  delay(2000);
  Serial.println("AT\r\n");
  ok = Serial.find("OK");
  delay(100);
  if (ok == false){return false;}
  
  
  Serial.println("AT+CMGF=1\r\n");
  ok = Serial.find("OK");
  delay(100);  
  if (ok == false){return false;}
  
  Serial.print("AT+CMGS=");
  Serial.print(char(34));
  Serial.print(mobile);
    Serial.print(char(34));
  Serial.print("\r\n");
  delay(3000);
  ok = Serial.find(">");
  
  Serial.print(sms);
  Serial.print("\r\n");
  Serial.print(char(26));
  
  Serial.setTimeout(40000);
  ok = Serial.find("+CMG");
  return ok;
  
  
  
}
