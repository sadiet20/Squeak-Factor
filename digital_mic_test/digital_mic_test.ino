/*
 * Rui Santos 
 * Complete Project Details https://randomnerdtutorials.com
 * Source: https://randomnerdtutorials.com/guide-for-microphone-sound-sensor-with-arduino/
*/

int ledPin=13;
int sensorPin=7;
boolean val =0;

void setup(){
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin (9600);
}
  
void loop (){
  val =digitalRead(sensorPin);
  Serial.println (val);
  // when the sensor detects a signal above the threshold value, LED flashes
  if (val==HIGH) {
    digitalWrite(ledPin, HIGH);
  }
  else {
    digitalWrite(ledPin, LOW);
  }
}
