const int sensorPin = 2; // Pin connected to the sensor
//volatile unsigned long count = 0; // Variable to store count of rotations
unsigned long previousMicros = 0; // Variable to store previous time
float rpm = 0; // Variable to store RPM value
float rps = 0;
float lastRPM = 0; // Last RPM value for smoothing
const int numSamples = 10; // Number of samples for smoothing
float rpmSamples[numSamples]; // Array to store RPM samples for smoothing
int sampleIndex = 0; // Index for RPM sample array
float count;

int numMagnets = 16; // Number of magnets on the wheel
float wheelDiameter = 50.8; // Diameter of the wheel in cm
int displayInterval = 1; // Display results interval in seconds

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(sensorPin, INPUT_PULLUP); // Set sensor pin as input with pullup resistor
  attachInterrupt(digitalPinToInterrupt(sensorPin), countPulse, FALLING); // Attach interrupt to count pulses
}

void loop() {
  unsigned long currentMicros = micros(); // Get current time
  unsigned long elapsedTime = currentMicros - previousMicros; // Calculate time elapsed
  
  if (elapsedTime >= displayInterval * 500000) { // Calculate RPM every displayInterval seconds
    //rps = ((float)count) / ((float)numMagnets / 2); // Convert count to revolutions per second considering the number of magnets
    rps = count;
    rps = rps / (numMagnets / 2);
    rpm = rps * 60.0; // Convert revolutions per second to revolutions per minute
    
    // Smoothing RPM values
   /* rpmSamples[sampleIndex] = rpm;
    sampleIndex = (sampleIndex + 1) % numSamples;
    float sum = 0;
    for (int i = 0; i < numSamples; i++) {
      sum += rpmSamples[i];
    }
    rpm = sum / numSamples;
    */
    Serial.print("RPM: ");
    Serial.println(rpm, 4); // Print RPM
    Serial.print("RPS: ");
    Serial.println(rps, 4); // Print RPS
    Serial.print("count: ");
    Serial.println(count, 4); // Print RPM
    Serial.print("Speed (cm/s): ");
    Serial.println(rps * wheelDiameter * PI); // Calculate speed in cm/s using wheel circumference
    Serial.print("Speed (km/h): ");
    Serial.println((rps * wheelDiameter * PI) * 0.036); // Convert speed to km/h
    
    count = 0; // Reset count
    previousMicros = currentMicros; // Update previous time
  }
}


void countPulse() {
  count++; // Increment count when a pulse is detected
}
