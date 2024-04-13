const int sensorPin = 2; // Pin connected to the sensor
unsigned long count = 0; // Variable to store count of rotations
unsigned long previousMillis = 0; // Variable to store previous time
float rpm = 0; // Variable to store RPM value
float rps = 0;
bool gec, gec2;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(sensorPin, INPUT_PULLUP); // Set sensor pin as input with pullup resistor
}

void loop() {
  unsigned long currentMillis = millis(); // Get current time
  unsigned long elapsedTime = currentMillis - previousMillis; // Calculate time elapsed
  
  if (digitalRead(sensorPin) == HIGH) {
    gec = 0;
    gec2 = 1;
    //Serial.println(0);
  }

  if (digitalRead(sensorPin) == LOW && gec == 0) { // Check if sensor pin is low
    count++; // Increment count when a pulse is detected
   gec = 1;
    //Serial.println(1);
   //delay(3);
  }

  

  if (elapsedTime >= 1000) { // Calculate RPM every second
   rps = (count /1);// vydelil som dvojnasobkom lebo v testovacom zapojeni magnet zapol vypinac dvakrat z dovodu jeho orientacie - kazda otacka sa ratala ako 2, nezabudnut zmenit ked sa magnet zaintegruje do systemu inak aby sa predislo nespravnym vysledkom!!!!!!!!!!!!!!!!!!!!
   //rps = count;
   /*if(rps != 0){
rps--;
   }*/
   rpm = rps * 60;
    Serial.print("RPM: ");
    Serial.println(rpm); // Print RPM
    Serial.print("RPS: ");
    Serial.println(rps); // Print RPM
    
    //Serial.println(digitalRead(sensorPin));
    //Serial.println(count);

    count = 0; // Reset count
    previousMillis = currentMillis; // Update previous time
  }
}
