//-----------HEATER----------//3
#include <OneWire.h>
#include <DallasTemperature.h>
const int TEMERATURE_SENSOR_PIN = 16;
const int HEATER_PIN = 17;
OneWire oneWire(TEMERATURE_SENSOR_PIN);
DallasTemperature tempSensor(&oneWire);
float tempCelsius;


//-------LCD+Keypad----------------//
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,20,4);  
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {38,40,42,44};  //connect to the row pinouts of the keypad
byte colPins[COLS] = {46,48,47,49};   //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);



//----------------ultraSonic------------------//
#define trigPin 14
#define echoPin 15
long duration;

int distance;


#define trigPinMILK 11
#define echoPinMILK 10
long durationMILK;

int distanceMILK;




//------------VAlves------------------------------//
const int VALVE1_PIN = 4;  //  pin of relay used it in valve
const int pump_PIN = 5;  //  pnb of relay used it in pump
const int VALVE3_PIN = 7;  //  pin of relay used it in valve connected to pump


// ------------------------- DC MOTOR RELAY -----------------//

const int DcMotor_PIN = 26;  //  pin of relay used it in dc motor+

//------------PUMP FOR MIXER ----------------------------//

const int pumpMix_PIN = 10;  //  pin of relay used it in pumpMixer
const int pumpRID_PIN = 6;  //  pin of relay used it in mixer valve to get rid of water
const int pumpMix2_PIN = 24; // black pump
const int valveRID_PIN=12;



// valve for cleanning 
const int VALVE4_PIN = 36; 
const int VALVE5_PIN = 34; 
const int VALVE6_PIN = 32; 

//-----------------Flow sensor--------------//
const int flowSensorPin = 3;  // Replace with the appropriate pin number
unsigned int flowFrequency = 0;



//------------ GENERAL ----------------------------//
  int TYPE;
int STATE = 0;
unsigned long previousMillis = 0;
const long interval = 2000; // 2 seconds interval for each state, adjust as needed
int mode=0;
char key0; //for modes
 char key ; // for amount of water
      char keyy;//for types

            bool f = false;


//------------------RTC-----------------------//
#include <RTClib.h>

RTC_DS1307 rtc;

//--------------------RFID----------------------//
#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>
#include <EEPROM.h>


/* Define the DIO used for the SDA (SS) and RST (reset) pins. */
#define SDA_DIO 9
#define RESET_DIO 8
/* Create an instance of the RFID library */
RFID RC522(SDA_DIO, RESET_DIO); 


//------------MP3 & PUSH BUTTON---------------------//

#include "SerialMP3Player.h"

#define TX 28
#define RX 30
#define BUTTON_PIN 2 

SerialMP3Player mp3(RX, TX);
bool isPlaying = false;



//--------------functions---------------------//


  void chooseMode()
  {
   DateTime now = rtc.now();

  Serial.print(now.hour());
          Serial.print(':');
 Serial.println(now.minute());
    

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Choose the mode :!");
  lcd.setCursor(0,1);
  lcd.print(" 1-Feeding Mode ");
   lcd.setCursor(0,2);
  lcd.print(" 2-Cleaning Mode ");
 
   delay(2000);

      while ((key0 = keypad.getKey()) == NO_KEY) {
        // Wait for user input
      }

      if ((key0 - '0') == 1) //  mode 1
      {       
                                                                       
    
        mode = 1;
        Serial.println(" Feeding Mode");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Feeding Mode");
        delay(1000);
        
      }

            if ((key0 - '0') == 2) //  mode 2
      {       
                                                                       
    
        mode = 2;
        Serial.println(" Cleaning Mode");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Cleaning Mode");
        delay(1000);
        
      }


  } 




void countPulse() {
  flowFrequency++;
}

void togglePlayback() {
  if (isPlaying) {
    mp3.pause();
    delay(500);
  } else {
    mp3.sendCommand(CMD_SEL_DEV, 0, 1);
    mp3.play();
    delay(500);
  }
  isPlaying = !isPlaying; // Toggle the playback state
}

void buttonPressed() {
  togglePlayback();
  Serial.println(isPlaying);
}


void setup()
{Serial.begin(115200);
   Serial1.begin(115200);

//---------------MP3 & push button-------------//
    pinMode(BUTTON_PIN, INPUT_PULLUP); 

  mp3.begin(9600);
  delay(500);
  mp3.sendCommand(CMD_SEL_DEV, 0, 2);
  delay(500);
  
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonPressed, LOW);




//--------------------------RFID-----------------//
 SPI.begin(); 
  /* Initialise the RFID reader */
  RC522.init();

//--------------------RTC---------------------------//
 if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1);
  }

  // automatically sets the RTC to the date & time on PC this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));


//-----------------------LCD------------------//

  lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcom to our Sheep MilkPro !");
   delay(2000);

 // choose MODE



//-----------Heater--------------//
  tempSensor.begin();
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN , HIGH);
  //-------------UltraSonic------------//


  //--------------------Valve & pump---------------------//
  //VALVE
   pinMode(VALVE1_PIN, OUTPUT);
      digitalWrite(VALVE1_PIN , HIGH);

      pinMode(pumpRID_PIN, OUTPUT);
      digitalWrite(pumpRID_PIN , HIGH);

      pinMode(VALVE3_PIN, OUTPUT);
      digitalWrite(VALVE3_PIN , HIGH);

    pinMode(pump_PIN, OUTPUT);
    digitalWrite(pump_PIN , HIGH);
    

//------------------FOR MILK -----------------------------//

//DC motor
 pinMode(DcMotor_PIN, OUTPUT);
      digitalWrite(DcMotor_PIN , HIGH);

//------------------FOR MIXER -----------------------------//

//pumpMix
pinMode(pumpMix_PIN, OUTPUT);
      digitalWrite(pumpMix_PIN , HIGH);
pinMode(pumpMix2_PIN, OUTPUT);
      digitalWrite(pumpMix2_PIN , HIGH);

 pinMode(VALVE4_PIN, OUTPUT);
      digitalWrite(VALVE4_PIN , HIGH);

pinMode(VALVE5_PIN, OUTPUT);
      digitalWrite(VALVE5_PIN , HIGH);

pinMode(VALVE6_PIN, OUTPUT);
      digitalWrite(VALVE6_PIN , HIGH);

 pinMode(valveRID_PIN, OUTPUT);
      digitalWrite(valveRID_PIN , HIGH);


      //-------------Flow sensor--------------//

     pinMode(flowSensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), countPulse, RISING);

}



// ... (your setup function remains the same)

// ... (your includes and pin definitions remain the same)

// ... (your setup function remains the same)

void loop() {
  DateTime now = rtc.now();
    Serial.print(now.hour());
        Serial.println(now.minute());
if (   ((now.hour() == 6) || (now.hour() == 12) || (now.hour() == 15) || (now.hour() == 19)) && mode!=2  )
 {
  //at 6 AM, 12 PM, 3 PM, or 7 PM
  mode =1;
  }//end of scheduling

  if ((now.hour() == 21) && mode!=1 ) {
  //at 8 PM
  mode =2;
  }//end of scheduling
else{
  if(!f)
   chooseMode();
}



  if(mode==1){


        Serial.println("mode feeding");
  unsigned long currentMillis = millis();

  // ------- General ------- //
  int entry;
  int amountMilk;
  bool waterEntered = false; // Flag to indicate if the user entered the amount of water
    double TIME = 0.0;


  //----------------------Heater-----------------
  tempSensor.requestTemperatures();
  tempCelsius = tempSensor.getTempCByIndex(0);
  Serial.print("TEMP: ");
  Serial.println(tempCelsius);
     
  //------------ultrasonic-------------------------
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print("distance INITIAL....");
  Serial.println(distance);
  
  //-------------FILLING WATER ----------------//


  // The state machine logic to control the flow
   

  switch (STATE) {
   case 0:
      // Wait for user to select the type of sheep
      Serial.println("select appropiate type 1 or 2 :");
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("Select the type:");
      lcd.setCursor(0,2);
      lcd.print("1:1 day to 30 days");
      lcd.setCursor(0,3);
      lcd.print("2:30 days to 60 day");

      while ((keyy = keypad.getKey()) == NO_KEY) {
        // Wait for user input
      }

      if ((keyy - '0') == 1) // TYPE 1 (1 day to 30 days)
      {
        TYPE = 1;
                f = true;

        Serial.println("Selected TYPE 1");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Selected TYPE 1");
        delay(1000);
      }
      else if ((keyy - '0') == 2) // TYPE 2 (30 days to 60 days)
      {
        TYPE = 2;
                f = true;

        Serial.println("Selected TYPE 2");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Selected TYPE 2");
                delay(1000);

      }

    // Move to the next state
      STATE = 1;
      break;
   
    case 1:
      // Wait for the user to enter the amount of water only if not entered already
      if (!waterEntered) {
        Serial.println("Enter the amount of water:");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter the amount of water:");
        char key;
        while ((key = keypad.getKey()) == NO_KEY) {
          // Wait for user input
        }

        // Calculate the distance after water filling
        Serial.println(key);
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("The amount of water in L: ");
        lcd.print(key);
        
        waterEntered = true; // Set the flag to true after the user enters the amount of water
      }

      // Open the valve and wait for the water to reach the desired level
      if (distance >8) {
        digitalWrite(VALVE1_PIN, LOW); // Open valve
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" VALVE IS OPEN ");

        while (1) {
          // Wait for the water to reach the desired level
          // Use millis() for non-blocking delay
          unsigned long currentMillis = millis();
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            // Ultrasonic sensor update
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(10);
            digitalWrite(trigPin, LOW);
            duration = pulseIn(echoPin, HIGH);
            distance = duration * 0.034 / 2;
            Serial.print(" distance1: ");
            Serial.println(distance);

          }
          if(distance < 8)break;
        }
        digitalWrite(VALVE1_PIN, HIGH); // Close valve
      }
      else if (distance <= 8) {
        digitalWrite(VALVE1_PIN, HIGH); // Close valve
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" VALVE IS CLOSE ");
      }
  

      // Move to the next state
      STATE = 2;
      break;

    // ... (the rest of the code remains the same)
  
    case 2:
      // Heating water
      tempSensor.requestTemperatures();
      tempCelsius = tempSensor.getTempCByIndex(0);
      Serial.print("Current temperature: ");
      Serial.println(tempCelsius);

      // Check if the temperature is below 40°C and turn on the heater
      if (tempCelsius<40) {
        digitalWrite(HEATER_PIN, LOW); // Turn on the heater
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("The Heater ON: ");

        delay(500);
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TEMP:");
        lcd.setCursor(6, 0);
        lcd.print(tempCelsius);
      
        // Wait for the temperature to reach 40°C
        while (tempCelsius<40) {
          // Wait for temperature update
          tempSensor.requestTemperatures();
          tempCelsius = tempSensor.getTempCByIndex(0);
          Serial.print("Current temperature: ");
          Serial.println(tempCelsius);

          
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("TEMP:");
        lcd.setCursor(6, 0);
        lcd.print(tempCelsius);
          delay(1000);
          
          }
  }

        digitalWrite(HEATER_PIN, HIGH); // Turn off the heater
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("The Heater OFF: ");
      
      

      // Move to the next state
      STATE = 3;
      break;

    case 3:
    //-----------------MILK OPERATION-------------------//


     lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Milk falling");

    if(TYPE ==1){
      Serial.println(key);
      amountMilk = (key- '0') * 6;
      Serial.println("type1");
      }

      else if(TYPE ==2){
      amountMilk = (key- '0') * 5;
      Serial.println("type2");

      }
    
       TIME = ((5 * 60) / 120.0)*1000; // 60s --> 60g of milk
       Serial.print("time");
       Serial.println(TIME);

      // Turn on the milk motor
      digitalWrite(DcMotor_PIN, LOW);
      delay(20000);
      digitalWrite(DcMotor_PIN, HIGH);
  // Move to the next state
  STATE = 4;
  break;


    case 4:

    //----------------------OPEN VALVE-------------------//

        digitalWrite(VALVE3_PIN,LOW); // turn on valve connected with pump
  digitalWrite(pump_PIN, LOW); // Turn on the pump

  Serial.println("Pump on");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" PUMP IS OPEN ");
   delay(15000);
  digitalWrite(pump_PIN, HIGH); // Turn off the pump
    digitalWrite(VALVE3_PIN,HIGH); // turn off valve connected with pump

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" PUMP IS ClOSE ");
  
     // Milk operation
     
      // Move to the next state
      STATE = 5;
      break;
    
    case 5:

  // Continue with the mixer operation (pump on for 10 seconds)
   digitalWrite(pumpMix_PIN, LOW); // Turn on the mixer pump
    lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" MIXER ON ");
  Serial.println("mixer on");

  delay(8000);


  digitalWrite(pumpMix_PIN, HIGH); // Turn off the mixer pump
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(" MIXER OFF ");
  delay(600);
  digitalWrite(pumpMix2_PIN, LOW); // Turn on the mixer pump



  // OPEN THE THREE VALVES 
  //Serial.println("valves onnnnnn");
  digitalWrite(VALVE4_PIN,LOW);
  digitalWrite(VALVE5_PIN,LOW);
  digitalWrite(VALVE6_PIN,LOW);

  Serial.println("Milk distribution");

  delay(7000);


//---flow sensor-----------//
  float flowRate = (float)flowFrequency * 7.5;  // Conversion factor for YF-S201 (mL/s)

  Serial.print("Flow Rate: ");
  Serial.print(flowRate);
  Serial.println(" mL/s");
lcd.clear();
 lcd.setCursor(0, 1);
 lcd.print("Flow Rate:");
 lcd.setCursor(0, 2);
 lcd.print(flowRate);
 lcd.setCursor(1, 2);
 lcd.print(" mL/s");
  
  delay(9000);  // Delay for one second

  
    digitalWrite(pumpMix2_PIN, HIGH); // Turn on the mixer pump
  digitalWrite(VALVE4_PIN,HIGH);
  digitalWrite(VALVE5_PIN,HIGH);
  digitalWrite(VALVE6_PIN,HIGH);

delay(2000);




//   //--------------------RFID----------------------------------//
//  /* Has a card been detected? */
 lcd.clear();
 lcd.setCursor(1, 1);
 lcd.print("Scan The sheep Card!");
 delay(8000);
if (RC522.isCard()) {
  RC522.readCardSerial();
  Serial.println("Card detected:");

  String cardID = "";
  for (int i = 0; i < 5; i++) {
    cardID += String(RC522.serNum[i], DEC);
  }
  Serial.println(cardID);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sheep ID:");
  lcd.setCursor(8, 0);
  lcd.print(cardID);
  
  // Convert the card ID to an integer for use as EEPROM address
  int eepromAddr = cardID.toInt();
  
  // Prompt user to enter the weight
  Serial.print(cardID);
  Serial.print(": ");
  
  int enteredWeight = 20; // Example weight  
  lcd.setCursor(0, 1);
  lcd.print("Weight:");
  lcd.setCursor(8, 1);
  lcd.print(enteredWeight);
  
  lcd.setCursor(0, 2);
  lcd.print("Type:");
  lcd.setCursor(7, 2);
  lcd.print( TYPE);


  delay(8000); 
  
  // Store the card ID, weight, and type in the same EEPROM address
  String combinedData = cardID + ',' + String(enteredWeight) + ',' + String(TYPE);
  int dataSize = combinedData.length();
  for (int i = 0; i < dataSize; i++) {
    EEPROM.write(eepromAddr + i, combinedData[i]);
  }
  
  Serial.println("Card ID, weight, and type stored in EEPROM.");
  Serial.println();
  
}
delay(2000);


    chooseMode();

break;




  }//SWITCH MODE 1
  }

 
//--------------------Cleaning ----------------------------//
   if(mode==2){
   
 digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.print("Water distance ....");
  Serial.println(distance);


  if(distance <8){

    Serial.println("mode cleaning");
  //         lcd.clear();
  // lcd.setCursor(0,0);
  // lcd.print("Cleaning Mode");
  
        digitalWrite(pumpRID_PIN , HIGH);
                digitalWrite(valveRID_PIN , HIGH);


      
   digitalWrite(VALVE4_PIN, HIGH);  // close valve 4
    digitalWrite(VALVE5_PIN, HIGH);  // close valve 5
    digitalWrite(VALVE6_PIN, HIGH);  // close valve 6
    digitalWrite(pumpRID_PIN, HIGH); 
    digitalWrite(valveRID_PIN , HIGH);


    delay(1000);
    // flowing water
  digitalWrite(VALVE3_PIN,LOW); // turn on valve connected with pump
  digitalWrite(pump_PIN, LOW); // Turn on the pump
    digitalWrite(pumpMix_PIN, HIGH); // Turn OFF the MIXER

  delay (15000);
  digitalWrite(VALVE3_PIN,HIGH); // turn off valve connected with pump
  digitalWrite(pump_PIN, HIGH); // Turn off the pump
  delay (5000);
// MIX IT

  digitalWrite(pumpMix_PIN, LOW); // Turn ON the MIXER
  delay(5000); 
  digitalWrite(pumpMix_PIN, HIGH); // Turn OFF the MIXER

 //remove it 
   digitalWrite(valveRID_PIN, LOW); 
 digitalWrite(pumpRID_PIN, LOW);
 
delay (25000);
    digitalWrite(pumpRID_PIN, HIGH); 
        digitalWrite(valveRID_PIN, HIGH); 

    digitalWrite(VALVE3_PIN , HIGH);
     digitalWrite(pump_PIN , HIGH);
    digitalWrite(pumpMix_PIN , HIGH);

delay(1000);
      chooseMode();

  }//ENDIF


  }//end of mode 2

  }// end loop