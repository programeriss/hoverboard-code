#include <SoftwareSerial9.h>

#define IN1 33 //Uzkurimo rele
//#define IN2 9 //laisvas...
//#define IN2 10 //laisvas...
//#define IN2 11 //laisvas...
//#define IN2 12 //laisvas...
#define TX_blade 8 //MOSI Blade
#define RX_blade 39 //MISO Blade
#define TX_L 5 //MOSI Left wheel
#define RX_L 37 //MISO Left wheel
#define TX_R 3 //MOSI Right Wheel
#define RX_R 31 //MISO Right Wheel
#define GND1 22
#define GND2 24
#define GND3 26
#define GND4 28
#define GND5 30
#define GND6 32
#define GND7 34
#define GND8 36
#define GND9 38
#define GND10 40
#define VCC6 41
#define LEDPIN 13

SoftwareSerial9 mySerial_wheel_L(RX_L,TX_L);
SoftwareSerial9 mySerial_wheel_R(RX_R,TX_R);
SoftwareSerial9 mySerial_blade(RX_blade,TX_blade);

bool powerOn = false;
int x = 0;
int sp = 0;

void setup() {
  Serial.begin(9600);
  mySerial_wheel_L.begin(26315);
  mySerial_wheel_R.begin(26315);
  mySerial_blade.begin(26315);

  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);

  pinMode(GND1, OUTPUT);
  pinMode(GND2, OUTPUT);
  pinMode(GND3, OUTPUT);
  pinMode(GND4, OUTPUT);
  pinMode(GND5, OUTPUT);
  pinMode(GND6, OUTPUT);
  pinMode(GND7, OUTPUT);
  pinMode(GND8, OUTPUT);
  pinMode(GND9, OUTPUT);
  pinMode(GND10, OUTPUT);
  digitalWrite(GND1, LOW);
  digitalWrite(GND2, LOW);
  digitalWrite(GND3, LOW);
  digitalWrite(GND4, LOW);
  digitalWrite(GND5, LOW);
  digitalWrite(GND6, LOW);
  digitalWrite(GND7, LOW);
  digitalWrite(GND8, LOW);
  digitalWrite(GND9, LOW);
  digitalWrite(GND10, LOW);
  pinMode(VCC6, OUTPUT);
  digitalWrite(VCC6, HIGH);
}

void loop() {

  if (powerOn == false) {
    PowerON();
  }

  //test
  x++;
  if (x > 145) {
    x = 0;
  }
  if (x > 75) {
    sp = 250;
  }else{
    sp = 150;
  }

  //Go blade
  goBlade(sp, 85);
  
  //Go wheel
  goWheel(sp, sp, 85);
}

//Suktis bleidui komandos siuntimas!
void goBlade(int sp, int onHoverboard) {
      mySerial_blade.write9(256);
      mySerial_blade.write9(sp & 0xFF);
      mySerial_blade.write9((sp >> 8) & 0xFF);
      mySerial_blade.write9(sp & 0xFF);
      mySerial_blade.write9((sp >> 8) & 0xFF);
      mySerial_blade.write9(onHoverboard);
      delayMicroseconds(300);

    Serial.print("BLADE: ");
    Serial.print("sp: ");
    Serial.print(sp);
    Serial.print(" onHoverboard: ");
    Serial.println(onHoverboard);
}

//Suktis ratams komandu siuntimas!
void goWheel(int sp_L, int sp_R, int onHoverboard) {
    mySerial_wheel_L.write9(256);
    mySerial_wheel_L.write9(sp_L & 0xFF);
    mySerial_wheel_L.write9((sp_L >> 8) & 0xFF);
    mySerial_wheel_L.write9(sp_L & 0xFF);
    mySerial_wheel_L.write9((sp_L >> 8) & 0xFF);
    mySerial_wheel_L.write9(onHoverboard);
    mySerial_wheel_L.write9(88);
    mySerial_wheel_L.write9(88);
    delayMicroseconds(300);
    
    mySerial_wheel_R.write9(256);
    mySerial_wheel_R.write9(sp_R & 0xFF);
    mySerial_wheel_R.write9((sp_R >> 8) & 0xFF);
    mySerial_wheel_R.write9(sp_R & 0xFF);
    mySerial_wheel_R.write9((sp_R >> 8) & 0xFF);
    mySerial_wheel_R.write9(onHoverboard);
    mySerial_wheel_R.write9(88);
    mySerial_wheel_R.write9(88);
    delayMicroseconds(300);

    Serial.print("WHEEL: ");
    Serial.print("sp_L: ");
    Serial.print(sp_L);
    Serial.print(" sp_R: ");
    Serial.print(sp_R);
    Serial.print(" onHoverboard: ");
    Serial.println(onHoverboard);
}

//Energija ON i motinine
void PowerON() {
   powerOn = true;
   Serial.println("Ijungiama elektros grandine ir paleidziama motinine");
   delay(300);
   digitalWrite(IN1, HIGH);

    int sp = 0;
    int onHoverboard = 170;
  
    for (int i = 0; i < 14; i++) {
      goBlade(sp, onHoverboard);
    }
   digitalWrite(IN1, LOW);
    for (int i = 0; i < 5; i++) {
      goBlade(sp, onHoverboard);
    }
}

//Energija OFF i motinine
/*
void PowerOFF() {
   powerOn = false;
   Serial.println("Nutraukiama energijos grandine");
   digitalWrite(IN1, HIGH);
   delay(500);
   digitalWrite(IN1, LOW);
   delay(2000);
}
*/
