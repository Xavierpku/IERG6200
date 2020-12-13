void setup() {
  Serial.begin(9600);  /* Define baud rate for serial communication */
}

void loop() {
  int count;
  for(count = 0; count<100; count++)
  {
    Serial.println(count);
    Serial.println('This is the plaintext we send');
    delay(100);
  }
}
