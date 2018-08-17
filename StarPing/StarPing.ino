

#define TRIGGER_PIN_1 9
#define TRIGGER_PIN_2 8


#define ECHO_PIN_1 6
#define ECHO_PIN_2 5

/**
 * Setup the Sketch
 */
//--------------------------------------------------------------
void setup() 
{
  Serial.begin(9600);
  pinMode(TRIGGER_PIN_1,OUTPUT);
  pinMode(ECHO_PIN_1,INPUT);
  pinMode(TRIGGER_PIN_2,OUTPUT);
  pinMode(ECHO_PIN_2,INPUT);
}

/**
 * Function for the Ping Sensor Simply input the pins 
 */
//--------------------------------------------------------------
long pingSensor(int triggerPin, int echoPin) {
  long distance, duration;

  digitalWrite(triggerPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin,LOW);
  duration = pulseIn(echoPin,HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}

/**
 * Main Loop runs every 100th of a second
 */
//--------------------------------------------------------------
void loop() {
  int val1 = pingSensor(TRIGGER_PIN_1,ECHO_PIN_1);
  int val2 = pingSensor(TRIGGER_PIN_2,ECHO_PIN_2);
  Serial.print(val1);
  Serial.print(",");
  Serial.println(val2);
  delay(90);
}
