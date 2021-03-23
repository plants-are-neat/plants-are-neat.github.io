// Data pins D5 D6 D7 D8
#define latchIN D2
#define recOUT D1
String msg = "";
void setup() {
  Serial.begin(9600);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  pinMode(D8, INPUT);
  pinMode(latchIN, INPUT);
  pinMode(recOUT, OUTPUT);
  digitalWrite(recOUT, LOW);
  delay(1000);

}

void loop() {
  if(checkLatch()) 
  {
    String s = getData();
    if (s[0] != '|') {
      msg.concat(s);
      Serial.print(s);
      Serial.println("This ran2");
      //Serial.println(msg);
    } 
    else {
      // Push message to console over Wi-Fi
      Serial.println(msg);
      Serial.println("complete");
      msg = "";
    }
    resetData();
    delay(10);
  }
}

bool checkLatch() {
  return ((digitalRead(latchIN) == HIGH) ? true : false);
}

String getData() {
  int num = 0;
  num += ((digitalRead(D5) == HIGH) ? 1 : 0) +
    ((digitalRead(D6) == HIGH) ? 2 : 0) +
    ((digitalRead(D7) == HIGH) ? 4 : 0) +
    ((digitalRead(D8) == HIGH) ? 8 : 0);
  if (num == 14) return String(",");
  if (num == 15) return String("|");
  return String(num);
}

void resetData() {
  digitalWrite(recOUT, HIGH);
  delay(10);
  digitalWrite(recOUT, LOW);
}
