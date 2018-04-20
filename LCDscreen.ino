#include <math.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define ONE_WIRE_BUS 9 
OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);
// ECE 110 Honors Code:
//

int pin[] = {0,1};
int time = 0;
int tones[] = {311, 311, 311, 311, 392, 392, 392, 392, 415, 440}; 
//              {261, 277, 294, 311, 330, 349, 370, 392, 415, 440};
//              mid C  C#   D    D#   E    F    F#   G    G#   A

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7,8,9,10,11,12);
const int buzzer = 9; //buzzer in pin 9

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
  sensors.begin();
  lcd.begin(10,1);    //prep lcd screen
  lcd.clear();
  pinMode(buzzer, OUTPUT);
}
 
void loop()                     // run over and over again
{
 
 //getting the voltage reading from the temperature sensor
 int reading[] = {analogRead(pin[0]), sensors.requestTemperatures()};  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 double voltage[] = {reading[0] * 5.0, 0};
 voltage[0] /= 1024.0; 
 voltage[1] /= 1024.0;
 
 // calculate temperature in Celsius
 double temperatureC[] = {(voltage[0] - 0.5) * 100, sensors.getTempCByIndex(0)};  //converting from 10 mv per degree wit 500 mV offset
                                                                               //to degrees ((voltage - 500mV) times 100)
 //temporary print for debugging
 Serial.print("This is the outside temperature: "); Serial.print(temperatureC[0]); Serial.println(" degrees C");
 Serial.print("This is the liquid temperature: "); Serial.print(temperatureC[1]); Serial.println(" degrees C");
 
 // now convert to Fahrenheit
 double temperatureF[] = {(temperatureC[0] * 9.0 / 5.0) + 32.0, (temperatureC[1] * 9.0 / 5.0) + 32.0};

 //temporary print for debugging
 Serial.print("This is the outside temperature: "); Serial.print(temperatureF[0]); Serial.println(" degrees C");
 Serial.print("This is the liquid temperature: "); Serial.print(temperatureF[1]); Serial.println(" degrees C");

 //calculate time left (TO DO: change constant k)
 double timeLeft = NewtonsLawofCooling(temperatureF[0], temperatureF[1], 0.00015, 50);

//if only sample once then print out time left till 0 and exit loop
 for(int countDown = timeLeft; countDown >= 0; countDown--) {
   lcd.setCursor(0,0);
   if(countDown != 0){
     lcd.print(countDown); lcd.print(" secs left!");
   } else {
     lcd.print("All ready!");
     buzzerPlay();
   }
   delay(1000); //wait 1 second
 }
 exit(0);
 
// delay(20000);                                     //sample every 20 seconds
}

//calculates Newton's Law of Cooling (inverse to find time)
//@param inside: temperature of the liquid
        //outside: temperature fo the outside
        //k: the cooling constant specific to our cup
        //final: ideal final temperature
//return the time it takes for the liquid to cool to ideal temperature
double NewtonsLawofCooling(double outside, double inside, int k, double final) {
  double inputTolog = (final - outside) / (inside - outside); 
  double time = log(inputTolog) / (double)(-1*k);
  return time;
}

//buzzer noise function - right now buzzes 3 times
//Lizzie add music maybe?
void buzzerPlay() {
 for(int i = 0; i < 3; i++) { 
  tone(buzzer, tones[1]); //can switch which tone to use
  delay(1000);
  noTone(buzzer);
  delay(1000);
 }
}
