
#define ModePin 13
int runMode = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.println("------- Entering setup --------");
  Serial.begin(115200);

  pinMode(ModePin, INPUT);

    if(digitalRead(ModePin) == HIGH){
    delay (3000);
    if(digitalRead(ModePin) == HIGH){
      runMode = 2;    // for OTA update: keep ModePin pressed for >3 sec.
    } else  { 
      runMode = 1;
    }
   }
   Serial.println("-------- Setup done ----------");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  
  //----- if else ladder -------------
  
  if (runMode == 0){
    Serial.println("Run mode : NORMAL");
  }
  else if (runMode == 1){
    Serial.println("Run mode : CONFIG.");
  }
  else {
    Serial.println("Run mode : OTA update");
  }
  
  
  /*
  //------- switch ------------
  
  switch (runMode)  {
    case 0:
      Serial.println("Run mode : NORMAL");
      break  ;
    case 1:
      Serial.println("Run mode : CONFIG");
      break ;
    case 2:
      Serial.println("Run mode : OTA update");
  }
  */
  

  
  //-------- conditional operator -----------
  /*
    runMode == 0? Serial.println("Run mode : NORMAL"): (runMode == 1? Serial.println("Run mode : CONFIG"): Serial.println("Run mode : OTA update"));
  */
  
  delay(1000);
}
