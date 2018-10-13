#include <SoftwareSerial9.h>

#define TX 10
#define RX 9
#define LEDPIN 13

SoftwareSerial9 mySerial(RX,TX);

void setup() {
  mySerial.begin(26315);
  Serial.begin(9600);
}

char c = ' ';
signed int sp=0;
int stopBlade = 0;
int pos;

int a = 0;
int b = 0;
int d = 0;

void loop() {
  //Serial.println(c);
  if (c == 'x') {
    pos = 0;
  }else{
    if (pos == 2) {
      d = c - '0';
      pos++;
    }
    if (pos == 1) {
      b = c - '0';
      pos++;
    }
    if (pos == 0) {
      a = c - '0';
      pos++;
    }
  }
  int temp = a * 100 + b * 10 + d;
  //Serial.println(temp);
  sp = temp * 20;

  stopBlade = 0;
  if (sp > 2100) {
    sp = 2100;
  }
  if (sp < 0) {
    sp = 0;
  }
  
  Serial.print("speed ");
  Serial.println(sp);
  do {
      mySerial.write9(256);
      mySerial.write9(sp & 0xFF);
      mySerial.write9((sp >> 8) & 0xFF);
      mySerial.write9(sp & 0xFF);
      mySerial.write9((sp >> 8) & 0xFF);
      mySerial.write9(85);
      delayMicroseconds(300);
      stopBlade++;
      if (stopBlade > 50) {
        stopBlade = -30;
        sp -= 100;
        if (sp < 0) {
          sp = 0;
        }
          Serial.print("speed ");
          Serial.println(sp);
      }
  } while(!Serial.available());
  c=Serial.read();
}
