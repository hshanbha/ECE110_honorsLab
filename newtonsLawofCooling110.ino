#include <math.h>
// ECE 110 Honors Code:
//

int pin[] = {0,1};

void setup()
{
  Serial.begin(9600);  //Start the serial connection with the computer
                       //to view the result open the serial monitor 
}
 
void loop()                     // run over and over again
{
 
 //getting the voltage reading from the temperature sensor
 int reading[] = {analogRead(pin[0]), analogRead(pin[1])};  
 
 // converting that reading to voltage, for 3.3v arduino use 3.3
 double voltage[] = {reading[0] * 5.0, reading[1] * 5.0};
 voltage[0] /= 1024.0; 
 voltage[1] /= 1024.0;
 
 // print out the voltage - for DEBUGGING maybe
 // Serial.print(voltage[0]); Serial.println(" volts");
 
 // calculate temperature in Celsius
 double temperatureC[] = {(voltage[0] - 0.5) * 100, (voltage[1] - 0.5) * 100};  //converting from 10 mv per degree wit 500 mV offset
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
 
 //TO DO:
        //print out the time left to a console
        //decide if to continue sampling and updating time how?
 
 delay(20000);                                     //sample every 20 seconds
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

