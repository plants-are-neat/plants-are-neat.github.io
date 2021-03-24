int datapins[] = {10, 11, 12, 13};
#define latch 9
#define rec 6
void setup() {
  Serial.begin(9600);
  for (int i = 10; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(latch, OUTPUT);
  pinMode(rec, INPUT);
  digitalWrite(latch, LOW);
  Serial.println("initialized");
}

void loop() {
  String msg = "1:2.3456789,10|";
  int num = 0;
  for (int i = 0; i < msg.length(); i++) {
    //Serial.println(num);
    if (msg[i] == ',') {
      num = 14;
    } else if (msg[i] == '|') {
      num = 15;
    } else if (msg[i] == '.') {
      num = 13;
    } else if (msg[i] == ':') {
      num = 12;
    } else {
      num = String(msg[i]).toInt();
    }
    Serial.println(num);
    sendBin(num);
    pulseLatch();
    resetData();
  }
  delay(5000);
}

void sendBin(int num) {
  String s = String(num, BIN);
  while (s.length() < 4) {
    s = String(0) + s;
  }
  for (int i = 0; i < 4; i++) {
    if (s[i] == '0') {
      digitalWrite(datapins[i], LOW);
    } else {
      digitalWrite(datapins[i], HIGH);
    }
  }
  delay(1);
}

void pulseLatch() {
  digitalWrite(latch, HIGH);
  delay(10);
  digitalWrite(latch, LOW);
}

void resetData() {
  while (digitalRead(rec) != HIGH) {}
  sendBin(15);
  delay(10);
}