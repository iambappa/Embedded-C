#include <EEPROM.h>

#define   SZ_RECORD_BOOK        (48)
#define   EEPROM_SIZE           (2048)    
#define   EEPROM_BEGIN_ADDR     0x00

#define   clearPin  13 
int resetFlag = 0 ;

uint8_t serial_no ;
uint8_t rand_value ;
uint8_t p_currIndx ;
unsigned long       Sleeptime = 10e6 ;  // 10 sec. deepsleep

typedef struct{
  uint8_t next ; 
  uint8_t sr_no[ SZ_RECORD_BOOK ], rand_val[ SZ_RECORD_BOOK ];
} DATA_TBL ;

DATA_TBL  _table_ ;
DATA_TBL *ptable = &_table_;

void  DATA_print(DATA_TBL *tbl) ;

void setup() {

  randomSeed(100);
  Serial.begin(115200);
  pinMode(clearPin, INPUT) ;
  
  Serial.println("\n========= Wake-up printing EEPROM data ===========");
  esp_sleep_enable_timer_wakeup(Sleeptime);   // needed for ESP32 only
  
  if(digitalRead(clearPin) == HIGH ){
    resetFlag = 1 ;
  }

  memset(ptable, 0, sizeof(DATA_TBL)) ;   //set all "0" for reset as of now
  
  //---- at reset/wakeup read from EEPROM-------------------

  char* pChar = (char *) ptable ;   //(char*)=> typecast to type "pointer to char"
  int nBytes = sizeof(DATA_TBL) ;
  int curAddr = 0 ;
  
  EEPROM.begin(EEPROM_SIZE);
  for(int indx = 0 ; indx < nBytes ; ++indx){
    pChar[ indx ]  = EEPROM.read(curAddr);
    curAddr++ ;
  }
  EEPROM.end();
  
  DATA_print(ptable) ;        // 2nd round
  
  p_currIndx   =  ptable->next ;
  Serial.println("\n------------------Setup done------------------");
}

void loop() {

  serial_no = p_currIndx ;
  rand_value = random(50, 100);
  Serial.printf("\nCurrent Record :");
  Serial.printf("\nSerial No: %d  Random value: %d\n", serial_no, rand_value) ;
  
 // _______________ STORE DATA in RAM table_______________

    ptable-> sr_no[ p_currIndx ]         =  serial_no ;     
    ptable-> rand_val[ p_currIndx ]      =  rand_value ;     
    ptable->next   =   ptable->next  + 1 ;
    
    //DATA_print(ptable) ;

    if(resetFlag) {
      ptable->next = 0;
    }
  // ________________________________________________________

  //-------------- STORE data in EEPROM -----------------

  char* pChar = (char *) ptable ; //(char*)=> typecast to type "pointer to char"
  int nBytes = sizeof(DATA_TBL) ;
  int curAddr = 0 ;
  
  EEPROM.begin(EEPROM_SIZE);
  for(int indx = 0 ; indx < nBytes ; ++indx){
    EEPROM.write(curAddr, (byte) pChar[ indx ]); 
    curAddr++ ;
  }
  EEPROM.end();
  Serial.println("EEPROM writing done");

  DATA_print(ptable) ;
  //-----------------------------------------------
  
  if(resetFlag)           ESP.restart();        // 2nd round
   
  delay(500);
  Serial.println("\n============Going to SLEEP============");
  esp_deep_sleep_start();       // for ESP32 sleep
}

void  DATA_print(DATA_TBL *tbl){

  uint8_t       num_record = 0 ;
  uint8_t       toPrint    = 0 ;
  uint8_t       indx ;
 
  Serial.printf("\nPrinting from EEPROM");
  
  num_record  =  tbl->next ;         indx   =  0 ;
  toPrint = num_record ;
  while(toPrint)    {
    
    Serial.printf("\nSerial No: %u, Random value: %u", tbl->sr_no[ indx ], tbl->rand_val[ indx ]) ;

    indx = ( indx + 1 ) ;   
    toPrint = toPrint - 1 ;                             
  }

  Serial.printf("\nArchive: %u records", num_record) ;
}
