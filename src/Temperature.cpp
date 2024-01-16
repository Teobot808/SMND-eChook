//  Teplota

#include <Temperature.hpp>
#include <math.h>
#include <Arduino.h>
#include <constants.hpp>


//Board and Sensor Specific Calibrations
const float CAL_THERM_A = 0.001871300068; //Steinhart-Hart constants - See documentation for calibration method
const float CAL_THERM_B = 0.00009436080271;
const float CAL_THERM_C = 0.0000007954800125;
bool DEBUG_MODE = 0;

float Temperature::thermistorADCToCelcius(int rawADC)
{
  // Constants for this calculation:

  // Steinhart-Hart Coefficients, see comment above
  // These coefficients are for the MF52AT NTC 10k thermistor, however due to thermistor tolerances each thermistor should be calibrated individually.
 
 
  const float A = CAL_THERM_A;
  const float B = CAL_THERM_B;
  const float C = CAL_THERM_C;

  // Value of resistor forming potential divider with Thermistor in ohms.
  const int FIXED_RESISTOR_VALUE = 10000; //10k

  /* Calculations:
  The formula is: Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]^3} where A, B and C are the coefficients above and R is the resistance across the thermistor.

  First step is to calculate the resistance of the thermistor using the potential divider equation V_out = (R1 + R2)/(R1 * R2)*V_in
   As R2 is the only unknown variable we can re-write this as: R2 = R1((V_in/V_out)-1).  R2 = (R1 V2)/(V1-V2)
  As the ADC values are our readings of the voltage, we can substitute V_in with 1024 and V_out with the reading taken from the ADC, which is passed into this function as rawADC
 This makes the calculation:   */

  float thermistorResistance = ((float)FIXED_RESISTOR_VALUE * (float)rawADC)/ (float)((float)1023 - (float)rawADC);

  // Next, you'll notice that the log natural (ln) of this resistance needs to be calculated 4 times in the Steinhart-Hart equation. This is a complex and long calculation for the arduino.
  // As such it is efficient to do it once and save the result for use later:

  double lnResistance = log(thermistorResistance);

  // Now plug it all into the equation:

  double temperature = 1 / (A + (B * lnResistance) + (C * lnResistance * lnResistance * lnResistance));



  // We now have the temperature in Kelvin. To convert it into Celcius we need to subtract 273.15

  temperature = temperature - 273.15;

  if (DEBUG_MODE)
  {
    Serial.print("\n\rThemistore Resistance = ");
    Serial.println(thermistorResistance);
    Serial.print("Temperature = ");
    Serial.println(temperature);
  }



  // Now return the Celcius Value:

  return (temperature);

}

float Temperature::readTempOne()
{
  float temp = Temperature::thermistorADCToCelcius(analogRead(TEMP1_IN_PIN)); //use the thermistor function to turn the ADC reading into a temperature

  return (temp); //return Temperature.
}

float Temperature::readTempTwo()
{
  float temp = Temperature::thermistorADCToCelcius(analogRead(TEMP2_IN_PIN));

  return (temp);
}