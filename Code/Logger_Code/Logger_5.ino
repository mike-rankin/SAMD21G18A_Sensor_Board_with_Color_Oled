//Setup
// File, Preferences, Additional Board Manager URLs: https://adafruit.github.io/arduino-board-index/package_adafruit_index.json
// Tools Boards Manager: Search for and install the Adafruit Feather M0
// Under Board select the Feather M0

//For the SSD_1331 Oled Display use this library: https://github.com/kodera2t/ultra_color_demo/tree/master/TO_BE_INSTALLED_IN_YOUR_LIB_folder

//To Do:
//Fix Screen Black on multiple Back press while on Main Menu Screen
//Fix Navigation debounce
//Set RTC Time
//When logging show min/max and current Sensor Data




//

#include <SSD_13XX.h>
#include <SD.h> 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_MPL115A2.h>
#include "ClosedCube_HDC1080.h"

//Oled
#define __CS1  8    //OLED_CS
#define __DC 	 10   //OLED_DC
#define __RST  9    //OLED_RES

//MicroSD
Sd2Card card;
SdVolume volume;
SdFile root;
#define SD_CS         4
#define SD_CARD_DET   12
File myFile;          //Data object you will write your sesnor data to
float tempC;
float humidity;
float pressure;

//Spash_Screen
//#include "_images/SAMD_Logger.c"

//Navigation Switch
#define Left_Button    3
#define Down_Button    2
#define Up_Button      5
#define Right_Button   11
#define In_Button      1

//Back Button
#define Back_Button_Interrupt   0
bool dont_exit;      //used to break out of While loop

int NumberOfLogs = 0;  

int Back_Button_State  = 0;
int SD_CARD_DET_State  = 0;
int Left_Button_State  = 0;
int Down_Button_State  = 0;
int Up_Button_State    = 0;
int Right_Button_State = 0;
int In_Button_State    = 0;

//Leds
#define RED_LED    13
#define GREEN_LED  6
#define BLUE_LED   7

#include "RTClib.h"
RTC_DS1307 rtc;

Adafruit_MPL115A2 mpl115a2;
ClosedCube_HDC1080 hdc1080;
SSD_13XX tft = SSD_13XX(__CS1, __DC,__RST);

/*******************************************************************************
                    Global variables
*******************************************************************************/ 
const int I2C_address = 0x68;  //RTC I2C write address 
byte    Second;     // Store second value
byte    Minute;     // Store minute value
byte    Hour;       // Store hour value
byte    Day;        // Store day value
byte    Date;       // Store date value
byte    Month;      // Store month value
byte    Year;       // Store year value


void setup(void) 
{
  Serial.begin(9600);
   //while (!Serial) 
   //{
   // ; // wait for serial port to connect. Needed for native USB port only
   //}
   
  tft.begin();

 // tft.drawImage(0, 0, &SAMD_Logger);
  delay(1000);

  hdc1080.begin(0x40);
  mpl115a2.begin();

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  digitalWrite(RED_LED, HIGH);    // turn all LEDs off by making the voltage HIGH
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);

  pinMode(Back_Button_Interrupt, INPUT); 
  attachInterrupt(digitalPinToInterrupt(Back_Button_Interrupt), Exit, RISING);

  tft.clearScreen();

  //Initialize Card
   while (!SD.begin(SD_CS)) 
   {
    Serial.println("initialization failed!");
    Blink_Red_Led();  //Red Led on power-up = No MicroSD
    tft.setTextScale(1);
    tft.setCursor(15,25); //over,down
    tft.setTextColor(RED);
    tft.print("Insert SD Card!");
   }
   Serial.println("initialization done."); 
   Blink_Green_Led();
}

int highlight_option=0; //from 0 to 3 for a 4 options menu

void loop() 
{ 
  //Back_Button_State = digitalRead(Back_Button);   // Back Button
  SD_CARD_DET_State = digitalRead(SD_CARD_DET);   // MicroSD Detect Switch
  Left_Button_State = digitalRead(Left_Button);   // Left Button
  Down_Button_State = digitalRead(Down_Button);   // Down Button
  Up_Button_State = digitalRead(Up_Button);       // Up Button
  Right_Button_State = digitalRead(Right_Button); // Right Button
  In_Button_State = digitalRead(In_Button);       // Right Button
  
  //check witch option should be highlighted
  if(Down_Button_State)      //if pushed down
  {
    highlight_option=highlight_option + 10;  //add 10 to the current value
    tft.clearScreen();
  }
  else if (Up_Button_State)  //but if pushed up
  {
    highlight_option=highlight_option-10; // subtract 10
    tft.clearScreen(); 
  }
  //check current option overflow. 3+1=4 would be invalid, same as 1-1=0
  if (highlight_option < 0) highlight_option = 30; //this has the effect of the cursor dissapearing from up to the last option.
  else if (highlight_option > 30) highlight_option = 0;

  for (int i=0;i<4;i++) 
  { // loop where i goes from 0 to 3 each iteration
    if(highlight_option == i) //if current option is highlight option
    { 

    } 
    else 
    {

    }

    tft.setTextColor(BLUE,BLACK);  //use this intead of tft.clearScreen();
    tft.setTextScale(1);
    
    tft.setCursor(12,0); //over,down
    tft.print("SD Card Logger");
    
    tft.setCursor(10,20); 
    tft.print("View Sensors");

    tft.setCursor(10,30); 
    tft.print("Start Logging");

    tft.setCursor(10,40); 
    tft.print("View Log");
    
    tft.setCursor(10,50); 
    tft.print("Delete Log");

    tft.setTextColor(RED);
    tft.setTextScale(1);
    tft.setCursor(5,highlight_option+20); //+20 is the start * location
    tft.print("*");
    delay(2);
  }

  // Check IN button
  if (In_Button_State);
  execute_menu(highlight_option); //execute current highlight option      
}


//should define some execute_menu function to execute any option based on input number range 0 to 3, being 0 first menu option and 3 last
void execute_menu (int option)
{
  if (highlight_option==0 && In_Button_State==1)       
  {
    tft.clearScreen();
    dont_exit=true;
    while(dont_exit)
    { 
      Display_Temp_Humid();
      DisplayTime();
      Display_Pressure();
      delay(10);
    }
  }

  
  if (highlight_option==10 && In_Button_State==1)  
  {
    tft.clearScreen();
    dont_exit=true;
    while(dont_exit)
    {  
     Log_To_SD_Card();
     delay(10);
    }
  }
  

  if (highlight_option==20 && In_Button_State==1)
  {
    tft.clearScreen();
    dont_exit=true;
    while(dont_exit)
    {   
     View_Log();
     delay(500);
     tft.clearScreen();
    }
  }
  
  if (highlight_option==30 && In_Button_State==1)
  {
    Delete_Log_On_SD_Card();
  } 
} 

void Exit()
{
 tft.clearScreen();
 dont_exit=false;
 tft.clearScreen(); 
}


void DisplayTime()
{
  char tempchar [7];
  byte i = 0;
  Wire.beginTransmission(I2C_address);
  Wire.write(0);
  Wire.endTransmission();
  
  Wire.requestFrom(I2C_address, 7);
  
  tft.setCursor(15,0);
  tft.setTextColor(BLUE,BLACK);
  tft.setTextScale(1);

  DateTime now = rtc.now();
  uint8_t hour = now.hour();

  if (now.hour() > 12)
  {
   hour = hour-12;
  }
  tft.print(hour);
  tft.print(":");// hour display
 
   if(now.minute()<10)
  {
   tft.print(0); //min  add 0
  } 
  tft.print(now.minute());
  tft.print(":");// min  display
  
  if(now.second()<10)
  {
   tft.print(0); //sec  add 0
  } 
  tft.print(now.second());  // sec  display


  if (now.hour() > 12)
  {
   tft.setTextScale(1);
   tft.print("pm ");
  }

  if (now.hour() < 12)
  {
   tft.setTextScale(1);
   tft.print("am ");
  }
 }


void Display_Temp_Humid(void)
 {

  tft.setCursor(15,25);
  tft.setTextColor(BLUE,BLACK);
  tft.setTextScale(1);
  
  tft.print("T:");
   
  tft.setCursor(28,25); //over,down
  tft.print(hdc1080.readTemperature());
  tft.print(" C ");
  delay(50);

  
  //Humidity
  tft.setCursor(15,35); //over,down
  tft.print("H:");
    
  tft.setCursor(28,35); //over,down
  tft.print(hdc1080.readHumidity());
  tft.print(" % ");
   delay(50);
 }

 
 void Log_To_SD_Card(void)
 {
   tft.setCursor(10,15);
   tft.setTextColor(BLUE,BLACK); 
   tft.setTextScale(1);
   tft.print("Logging Data...");

   tft.setCursor(10,25);
   tft.print("Log # ");
   tft.print(NumberOfLogs  );
   delay(3000);
   
   tempC = hdc1080.readTemperature();  
   humidity = hdc1080.readHumidity();
   pressure = mpl115a2.getPressure(); 

  myFile = SD.open("LOG.csv", FILE_WRITE);

  if (myFile) // if the file opened okay, write to it:
  {
    Serial.print("T:");
    Serial.println(tempC);    //Shows Temp that will be written in Serial
    myFile.print("T:");
    myFile.println(tempC);
    myFile.println(" ");
    Serial.print("H:");
    Serial.println(humidity); //Shows Humidity that will be written in Serial
    myFile.print("H:");
    myFile.println(humidity);
    myFile.println(" ");
    Serial.print("P:");
    Serial.println(pressure);    //Shows Temp that will be written in Serial
    myFile.print("P:");
    myFile.println(pressure);
    myFile.println(" ");
    
    NumberOfLogs++;
    
    myFile.close();  // close the file:
    Blink_Blue_Led();
  } 
   else 
   {
     // if the file didn't open, print an error:
     Serial.println("error opening LOG.csv");
   }
 }


void View_Log(void)
{
  int x,y;
  x=1,y=0;
  int dat;
  float SD_Card_Data;

   Serial.println("Contents of LOG.csv:");
   tft.clearScreen();
   tft.setCursor(0,0); 
   tft.setTextColor(BLUE);
   tft.print("Number of Logs :");
   tft.print(NumberOfLogs);
 
   myFile = SD.open("LOG.csv");
   if (myFile) 
   {
    //read from the file until there's nothing else in it:
    while (myFile.available())
    {
      Serial.write(myFile.read());  //Shows Read Temp in Serial  
    }  
       myFile.close();
   
  } else 
  {
    // if the file didn't open, print an error:
    Serial.println("error opening LOG.csv");
  }
}


void Delete_Log_On_SD_Card()
{
  SD.remove("LOG.csv");
  Serial.println("deleted LOG.csv");
  tft.clearScreen();
  tft.setCursor(0,0);
  tft.setTextColor(BLUE);
  tft.print("LOG.csv deleted!");
  NumberOfLogs=0;
  delay(1000);
} 
 
 
void Blink_Red_Led(void)
 {
  digitalWrite(RED_LED, HIGH);    // turn the LED off by making the voltage HIGH
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);  

  digitalWrite(RED_LED, LOW);  //RED LED ON
  delay(1000);
  digitalWrite(RED_LED, HIGH);  //RED LED OFF
  delay(100);
 }

 void Blink_Green_Led(void)
 {
  digitalWrite(RED_LED, HIGH);    // turn the LED off by making the voltage HIGH
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);  

  digitalWrite(GREEN_LED, LOW);  //RED LED ON
  delay(100);
  digitalWrite(GREEN_LED, HIGH);  //RED LED OFF
  delay(100);
 }

 void Blink_Blue_Led(void)
 {
  digitalWrite(RED_LED, HIGH);    // turn the LED off by making the voltage HIGH
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(100);  

  digitalWrite(BLUE_LED, LOW);  //RED LED ON
  delay(100);
  digitalWrite(BLUE_LED, HIGH);  //RED LED OFF
  delay(100);
 } 

 void Display_Pressure(void)
 {
  float pressureKPA = 0;

  tft.setCursor(15,45);
  tft.setTextColor(BLUE,BLACK);
  tft.setTextScale(1);
  
  tft.print("P:");
   
  tft.setCursor(28,45); //over,down
  tft.print(pressureKPA = mpl115a2.getPressure());
  tft.print(" KPa ");
  delay(50);
 }
 
