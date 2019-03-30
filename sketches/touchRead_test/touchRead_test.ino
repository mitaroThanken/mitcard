
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int data = touchRead(16);
  Serial.print("16: ");
  Serial.println(data);
  data = touchRead(22);
  Serial.print("22: ");
  Serial.println(data);
  data = touchRead(23);
  Serial.print("23: ");
  Serial.println(data);
  delay(100);
}
