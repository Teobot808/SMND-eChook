#include <string.h>
#include <Arduino.h>
#include <Bounce2.h>


/** ================================== */
/** CONSTANTS                          */
/** ================================== */
/** ___________________________________________________________________________________________________ ANALOG INPUT PINS */
const int   VBATT_IN_PIN        = A0;  // Analog input pin for battery voltage
const int   AMPS_IN_PIN         = A2;  // Analog input pin for current draw
const int   THROTTLE_IN_PIN     = A3;  // Analog input pin for the throttle
const int   TEMP1_IN_PIN        = A5;  // Analog input pin for temp sensor 1
const int   TEMP2_IN_PIN        = A4;  // Analog input pin for temp sensor 2
const int   VBATT1_IN_PIN       = A7;  // Analog input pin for the lower battery voltage (battery between ground and 12V)

/** ___________________________________________________________________________________________________ DIGITAL INPUT PINS */
const int   BRAKE_IN_PIN        = 7;    // Digital input pin for launch mode button
const int   LAUNCH_BTN_IN_PIN   = 8;    // Digital input pin for launch mode button
const int   CYCLE_BTN_IN_PIN    = 12;   // Digital input pin for cycle view button

/** ___________________________________________________________________________________________________ DIGITAL INTERRUPT PINS */
const int   MOTOR_RPM_PIN       = 2;    // Digital interrupt for motor pulses
const int   WHEEL_RPM_PIN       = 3;    // Digital interrupt for wheel pulses

/** ___________________________________________________________________________________________________ DIGITAL AND PWM OUTPUT PINS */
const int   MOTOR_OUT_PIN       = 5;   // PWM output to the motor


/** ________________________________________________________________________________________ BLUETOOTH CONSTANTS */
/* BLUETOOTH SETUP PARAMETERS */
const String CAL_BT_NAME     = "eChook"; // Whatever you want to name your car's bluetooth
const String CAL_BT_PASSWORD = "1234"; // Changing password from default "1234" tends not to work yet - apologies!
const String  BT_NAME           = CAL_BT_NAME;      // Name of the bluetooth module to appear on phone
const String  BT_PASSWORD       = CAL_BT_PASSWORD;  // Pairing Password
const long    BT_BAUDRATE       = 115200;           // Baud Rate to run at. Must match Arduino's baud rate.

//Bluetooth module uses hardware serialA from Arduino, so Arduino Tx -> HC05 Rx, Ard Rx -> HC Tx. EN and Status are disconnected.


/** ________________________________________________________________________________________ CONSTANTS */
/* DATA TRANSMIT INTERVALS */
const unsigned long  SHORT_DATA_TRANSMIT_INTERVAL = 250;     // transmit interval in ms

/* CURRENT */
const int AMPSENSOR_CAL_DELAY  = 3000;    // calibration delay for current sensor (ms)




/** ___________________________________________________________________________________________________ DATA IDENTIFIERS */

// If these are altered the data will no longer be read correctly by the phone.


const char SPEED_ID            = 's';
const char MOTOR_ID            = 'm';
const char CURRENT_ID          = 'i';
const char VOLTAGE_ID          = 'v';
const char VOLTAGE_LOWER_ID    = 'w';
const char THROTTLE_INPUT_ID   = 't';
const char THROTTLE_ACTUAL_ID  = 'd';
const char TEMP1_ID            = 'a';
const char TEMP2_ID            = 'b';
const char TEMP3_ID            = 'c';
const char LAUNCH_MODE_ID      = 'L';
const char CYCLE_VIEW_ID       = 'C';
const char GEAR_RATIO_ID       = 'r';
const char BRAKE_PRESSED_ID    = 'B';


/** ================================== */
/** VARIABLES                          */
/** ================================== */
/** ___________________________________________________________________________________________________ TIMING VARIABLES */
unsigned long   lastShortDataSendTime       = 0;
unsigned long   lastLongDataSendTime        = 0;
unsigned long   lastWheelSpeedPollTime      = 0;    // poll interval for wheel speed
unsigned long   lastMotorSpeedPollTime      = 0;    // poll interval for wheel speed
int             currentSensorOffset         = 0;    //offset value for the current sensor
int             currentAvgLoopCount         = 0;    //Counter for current loop average
int       	  	loopCounter              		= 0;


/** ___________________________________________________________________________________________________ INTERRUPT VERIABLES */
/** Any variables that are being used in an Interrupt Service Routine need to be declared as volatile. This ensures
 *  that each time the variable is accessed it is the master copy in RAM rather than a cached version within the CPU.
 *  This way the main loop and the ISR variables are always in sync
 */
volatile unsigned long motorPoll      = 0;
volatile unsigned long wheelPoll      = 0;
volatile unsigned long fanPoll        = 0;




/** ___________________________________________________________________________________________________ BUTTON VERIABLES */
Bounce launchButtonDebounce = Bounce();
Bounce cycleButtonDebounce  = Bounce();
Bounce brakeButtonDebounce  = Bounce();

/** ___________________________________________________________________________________________________ Sensor Readings */

float batteryVoltageTotal   = 0;
float batteryVoltageLower   = 0;
float throttle        		  = 0;
float current       		    = 0;
float motorRPM        		  = 0;
float wheelRPM 				      = 0;
float wheelSpeed      		  = 0;
float gearRatio 			      = 0;
float tempOne       		    = 0;
float tempTwo       		    = 0;
float tempThree       		  = 0;
int   brake         		    = 0;



/** ___________________________________________________________________________________________________ Smoothing Variables */

/**
 * For some signals it is desireable to average them over a longer period than is possible using the hardware
 * components on the board. To do this we average the last few readings taken by the Arduino.
 * When a new reading is taken it is added to an array. Each new reading takes the place of the oldest reading
 * in the array, so the array always contains the last X number of readings, where X is the size of the array.
 * In our case, for a reading being updated every 250ms, an array of length 4 would average the readings over
 * the last second.
 * To implement this in code, two golbal variables are needed per filter:
 * 	> The Array - to store the last X number of readings
 *	> A Count - This dictates which position in the array a new reading goes to and loops between 0 and array length - 1
 * To make it simple to alter the length of time to average over it is good practice to define the max count
 * value and aray length as a global const too.
 */

//Current Smoothing Variables:

const int currentSmoothingSetting = 4; //current is sampled every 250ms, therefore 4 makes 1s of smoothing
int currentSmoothingArray[currentSmoothingSetting];
int currentSmoothingCount = 0;

//Speed Smoothing Variables:

const int speedSmoothingSetting = 3; //speed is sampled every 1s, therefore 3 makes 3 seconds of smoothing
int speedSmoothingArray[speedSmoothingSetting];
int speedSmoothingCount = 0;