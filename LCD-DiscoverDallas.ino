/*
   DS18B20 Pinout (Left to Right, pins down, flat side toward you)
 - Left   = Ground
 - Center = Signal (Pin 10):  (with 3.3K to 4.7K resistor to +5 or 3.3 )
 - Right  = +5 or +3.3 V   
 */
/*-----( Import needed libraries )-----*/
// Get 1-wire Library here: http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <OneWire.h>

//Get DallasTemperature Library here:  http://milesburton.com/Main_Page?title=Dallas_Temperature_Control_Library
#include <DallasTemperature.h>
// Wire (I2C) Library
#include <Wire.h>
// LCD Library
#include <LCD.h>
#include <LiquidCrystal.h>
//#include <LiquidCrystal_I2C.h>  // F Malpartida's NewLiquidCrystal library
//Download: https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
// Move original LiquidCrystal library elsewhere, copy this in it's place


/*-----( Declare Constants and Pin Numbers )-----*/
#define  BOARDLED 13

#define ONE_WIRE_BUS 3
#define I2C_ADDR    0x20  // Define I2C Address for the PCF8574A 
//---(Following are the PCF8574 pin assignments to LCD connections )----
#define BACKLIGHT_PIN  7
#define En_pin  52
#define Rw_pin  51
#define Rs_pin  50
#define D4_pin  45
#define D5_pin  44
#define D6_pin  43
#define D7_pin  42
LiquidCrystal lcd(En_pin, Rw_pin, Rs_pin, D4_pin, D5_pin, D6_pin, D7_pin);

#define  LED_OFF  0
#define  LED_ON  1

/*-----( Declare objects )-----*/
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass address of our oneWire instance to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// 28 FFEB 3263 1402 87
DeviceAddress Probe1 = {
  0x28, 0xFF, 0xEB, 0x32, 0x63, 0x14, 0x02, 0x87 };
// 28 FFDD 0863 1402 CC
DeviceAddress Probe2 = {
  0x28, 0xFF, 0xDD, 0x08, 0x63, 0x14, 0x02, 0xCC }; 
// Stores Minima & Maxima
float Mini1;
float Mini2;
float Maxi1;
float Maxi2;

void setup()   /****** SETUP: RUNS ONCE ******/
{
  Serial.begin(9600);
  int cpt=0;
  int nbsensors;
  DeviceAddress insideThermometer;
  pinMode(BOARDLED, OUTPUT);
  //------- Initialize the Temperature measurement library--------------
  sensors.begin();
  // set the resolution to 10 bit (Can be 9 to 12 bits .. lower is faster)
  //---------------- Initialize the lcd ------------------  
  lcd.begin (20,4);  // 20 characters, 4 lines
  lcd.clear();  // Reset the display  
  lcd.home();

  // Print our characters on the LCD
  // NOTE: Line number and character number start at 0 not 1
  delay(2500);

  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  nbsensors=sensors.getDeviceCount();
  Serial.println(" devices.");
  for(int cpt=0;cpt<nbsensors;cpt++){
    if((cpt%4)==0)
      delay(4000);
    if (!sensors.getAddress(insideThermometer, cpt)){
      Serial.println("Unable to find address for Device 0");
    }else{
      lcd.setCursor(0,(cpt%4));
      Serial.print("Device #");
      Serial.print(cpt);
      Serial.print(":");
      printAddress(insideThermometer);
      Serial.println();
    }
  }
  if(nbsensors==0)
    Serial.println("Unable to find address for insideThermometer");

}//--(end setup )---


void loop()   /****** LOOP: RUNS CONSTANTLY ******/
{
    digitalWrite(BOARDLED,HIGH);
    delay(2000);
    digitalWrite(BOARDLED,LOW);
    delay(2000);
}//--(end main loop )---

/*-----( Declare User-written Functions )-----*/
float displayTemperature(DeviceAddress deviceAddress)
{

  float tempC = sensors.getTempC(deviceAddress);

  if (tempC == -127.00) // Measurement failed or no device found
  {
    lcd.print("Temperature Error");
  } 
  else
  {
    lcd.print("C=");
    lcd.print(tempC);
  }
  return tempC;
}// End printTemperature
void displayMiniMaxi(float mini,float maxi){
  lcd.print("Min");
  lcd.print(mini);
  lcd.print(" Max");
  lcd.print(maxi);
}
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16){
      Serial.print("0");
      lcd.print("0");
    }
    Serial.print(deviceAddress[i], HEX);
    lcd.print(deviceAddress[i], HEX);
  }
}



