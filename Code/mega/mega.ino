#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SoftwareSerial9.h>

#define IN1 8 //Uzkurimo rele
#define IN2 9 //Gyvybes palaikymo rele
#define TX_L 5 //MOSI Left wheel
#define RX_L 4 //MISO Left wheel
#define TX_R 3 //MOSI Right Wheel
#define RX_R 2 //MISO Right Wheel
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
#define VCC1 31
#define VCC2 33
#define VCC3 35
#define VCC4 37
#define VCC5 39
#define VCC6 41
#define LEDPIN 13

RF24 radio(49, 48); // CE, CSN

SoftwareSerial9 mySerial_L(RX_L,TX_L);
SoftwareSerial9 mySerial_R(RX_R,TX_R);

const byte address[11] = "1GU123HGN9";

bool onBlade = false;
int ntn = 0;
int Pot_UpnDown_R = 0;
int Pot_Screen_R = 0;
int Pot_LeftRight_R = 0;
int Pot_UpnDown_L = 0;
int waitingUpdate = 65;
int faleTime = 0;
bool powerOn = false;

signed int sp_L = 0;
signed int sp_R = 0;
signed int new_sp_L = 0;
signed int new_sp_R = 0;
signed int sp_L2 = 0; //vienos puses sukimui
signed int sp_R2 = 0; //vienos puses sukimui
bool firstTime = true;

void setup() {
  Serial2.begin(9600);
  mySerial_L.begin(26315);
  mySerial_R.begin(26315);
  Serial.begin(9600);
  
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.startListening();

  pinMode(IN1, OUTPUT);
  digitalWrite(IN1, LOW);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN2, LOW);

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
  pinMode(VCC1, OUTPUT);
  pinMode(VCC2, OUTPUT);
  pinMode(VCC3, OUTPUT);
  pinMode(VCC4, OUTPUT);
  pinMode(VCC5, OUTPUT);
  pinMode(VCC6, OUTPUT);
  digitalWrite(VCC1, HIGH);
  digitalWrite(VCC2, HIGH);
  digitalWrite(VCC3, HIGH);
  digitalWrite(VCC4, HIGH);
  digitalWrite(VCC5, HIGH);
  digitalWrite(VCC6, HIGH);
}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    //Serial.println(text);
    
    if (isDigit(text[0]) or isDigit(text[1])) { //Jeigu pirmasis yra skaičius, paleisti vykdyti ;-) Jei purvas grazins kitoki ASCII lentelėje reiksme :)
        String tempStr;
        tempStr = strtok(text, "|");
        Pot_UpnDown_R = tempStr.toInt();
        tempStr = strtok(NULL, "|");
        Pot_Screen_R = tempStr.toInt() / 10;
        tempStr = strtok(NULL, "|");
        Pot_LeftRight_R = tempStr.toInt();
        tempStr = strtok(NULL, "|");
        Pot_UpnDown_L = tempStr.toInt() / 10;
    
        if (Pot_UpnDown_R < -100) { Pot_UpnDown_R = -100; }
        if (Pot_UpnDown_R > 100) { Pot_UpnDown_R = 100; }
        if (Pot_Screen_R < 0) { Pot_Screen_R = 0; }
        if (Pot_Screen_R > 100) { Pot_Screen_R = 100; }
        if (Pot_LeftRight_R < -100) { Pot_LeftRight_R = -100; }
        if (Pot_LeftRight_R > 100) { Pot_LeftRight_R = 100; }
        if (Pot_UpnDown_L < 0) { Pot_UpnDown_L = 0; }
        if (Pot_UpnDown_L > 100) { Pot_UpnDown_L = 100; }
    
        //Serial.print(Pot_UpnDown_L); Serial.print("|"); Serial.print(Pot_UpnDown_R); Serial.print("|"); Serial.print(Pot_LeftRight_R); Serial.print("|"); Serial.println(Pot_Screen_R);
    
        faleTime = 0;
        if (!powerOn) {
          PowerON();
        }
    }
  }

  if (powerOn) {
    faleTime++;
    if (faleTime >= 15) { //apie 30 yra 1sekunde
      PowerOFF();
      Reset();
    }

    update_new_sp();
    updateSpeed();
    
    //Serial.print("Left wheel: ");
    //Serial.println(sp_L);
    //Serial.print("Right wheel: ");
    //Serial.println(sp_R);

    if (Pot_UpnDown_L < 8 && sp_L == 0 && sp_R == 0) {
      ntn += 1;

      if (ntn > 3)
        stopWheel();
    }else{
      ntn = 0;
      if (sp_L > 0 && sp_R > 0 || sp_L < 0 && sp_R < 0) {
        goWheel();
        firstTime = true;
          //Serial.print("goWheel: ");
          //Serial.print(sp_L);
          //Serial.print(" | ");
          //Serial.println(sp_R);
      }else{
        if (firstTime) {
          if (abs(abs(sp_L)-abs(sp_R)) < 350) {
            firstTime = false;
          }
        }else{
          goWheelImpulse(); 
          //Serial.print("goWheelImpulse: ");
          //Serial.print(sp_L2);
          //Serial.print(" | ");
          //Serial.println(sp_R2);
        }
      }
    }
     
   goBlade();
      
  }else{
    stopWheel();
  }
}

//greicio atnaujinimai, sukimuisi
void update_new_sp() {
  int newPot_UpnDown_L = Pot_UpnDown_L / 5.2;
  if (Pot_UpnDown_R >= 60 || Pot_UpnDown_R <= -60) {
      new_sp_L = Pot_UpnDown_R * newPot_UpnDown_L * -1;
      new_sp_R = Pot_UpnDown_R * newPot_UpnDown_L;
  }else{
    new_sp_L = 0;
    new_sp_R = 0;
  }
  
  if (Pot_LeftRight_R > 0 && Pot_LeftRight_R >= 60) {
    int newPot_LeftRight_R = 0;

    //turbo būstas
    if (newPot_LeftRight_R == 0 && Pot_LeftRight_R >= 80) {
      if (new_sp_L > 0) {
        new_sp_L = 900;
      }else{
        new_sp_L = -900;
      }
  
      if (new_sp_R > 0) {
        new_sp_R -= -900;
      }else{
        new_sp_R -= 900;
      }
      newPot_LeftRight_R = 900;
    }
    ///
    
    if (newPot_UpnDown_L >= 1) {
      newPot_LeftRight_R = Pot_LeftRight_R;
    }
    if (newPot_UpnDown_L > 3) {
      newPot_LeftRight_R = Pot_LeftRight_R * newPot_UpnDown_L;
      if (newPot_LeftRight_R > 1500) {
        newPot_LeftRight_R = 1500;
      }
      if (newPot_LeftRight_R < -1500) {
        newPot_LeftRight_R = -1500;
      }
    }

    if (new_sp_L > 0) {
      new_sp_L = newPot_LeftRight_R;
    }else{
      new_sp_L = newPot_LeftRight_R * -1;
    }

    if (new_sp_R > 0) {
      new_sp_R = newPot_LeftRight_R;
    }else{
      new_sp_R = newPot_LeftRight_R * -1;
    }
  }
  
  if (Pot_LeftRight_R < 0 && Pot_LeftRight_R <= -60) {
    int newPot_LeftRight_R = 0;

    //turbo būstas
    if (newPot_LeftRight_R == 0 && Pot_LeftRight_R <= -80) {
      if (new_sp_L > 0) {
        new_sp_L = -900;
      }else{
        new_sp_L = 900;
      }
  
      if (new_sp_R > 0) {
        new_sp_R -= 900;
      }else{
        new_sp_R -= -900;
      }
      newPot_LeftRight_R = -900;
    }
    ///
    
    if (newPot_UpnDown_L >= 1) {
      newPot_LeftRight_R = Pot_LeftRight_R;
    }
    if (newPot_UpnDown_L > 3) {
      newPot_LeftRight_R = Pot_LeftRight_R * newPot_UpnDown_L;
      if (newPot_LeftRight_R > 1500) {
        newPot_LeftRight_R = 1500;
      }
      if (newPot_LeftRight_R < -1500) {
        newPot_LeftRight_R = -1500;
      }
    }
    
    if (new_sp_L > 0) {
      new_sp_L = newPot_LeftRight_R * -1;
    }else{
      new_sp_L = newPot_LeftRight_R;
    }

    if (new_sp_R > 0) {
      new_sp_R = newPot_LeftRight_R * -1;
    }else{
      new_sp_R = newPot_LeftRight_R;
    }
  }
  
  if (new_sp_L > 2100) {
    new_sp_L = 2100;
  }else if (new_sp_L < -2100) {
    new_sp_L = -2100;
  }
  if (new_sp_R > 2100) {
    new_sp_R = 2100;
  }else if (new_sp_R < -2100) {
    new_sp_R = -2100;
  }

  //Serial.print(new_sp_L);
  //Serial.print(" | ");
  //Serial.println(new_sp_R);
}

//Greicio atnaujinimas, kiek leisti suktis!
void updateSpeed() {
  if (new_sp_L != sp_L) {
    if (new_sp_L > sp_L) {
      sp_L = sp_L + 100;
    }else if (new_sp_L < sp_L) {
      sp_L = sp_L - 100;
    }
  }
  
  if (new_sp_R != sp_R) {
    if (new_sp_R > sp_R) {
      sp_R = sp_R + 100;
    }else if (new_sp_R < sp_R) {
      sp_R = sp_R - 100;
    }
  }

  //Serial.print(sp_L);
  //Serial.print(" | ");
  //Serial.println(sp_R);
  delay(30);
}

//Suktis ratams komandu siuntimas!
void goWheel() {
    mySerial_L.write9(256);
    mySerial_L.write9(sp_L & 0xFF);
    mySerial_L.write9((sp_L >> 8) & 0xFF);
    mySerial_L.write9(sp_L & 0xFF);
    mySerial_L.write9((sp_L >> 8) & 0xFF);
    mySerial_L.write9(85);
    mySerial_L.write9(88);
    mySerial_L.write9(88);
    delayMicroseconds(300);
    
    mySerial_R.write9(256);
    mySerial_R.write9(sp_R & 0xFF);
    mySerial_R.write9((sp_R >> 8) & 0xFF);
    mySerial_R.write9(sp_R & 0xFF);
    mySerial_R.write9((sp_R >> 8) & 0xFF);
    mySerial_R.write9(85);
    mySerial_R.write9(88);
    mySerial_R.write9(88);
    delayMicroseconds(300);

    //Serial.print("goWheel OK! >> ");
}

struct info {
   int L = 0;
   int R = 0;
};

//gauti greiti, kuri reikia prideti, kai sukasi i viena puse
struct info getSpeed() {
    struct info rez;

    int greitis = 100; //Riboms greicio max reguliavimui...
    
    //pakeistas kodas
    if (new_sp_L != sp_L2) {
      if (new_sp_L > sp_L2) {
        if (sp_L2 > 100) {
          rez.L = 300; // gali reikti pliusuoti, kad pataikyti i igreitejima
        }
        sp_L2 += greitis;
      }else if (new_sp_L < sp_L2) {
        if (sp_L2 < -100) {
          rez.L = 300 * -1; // gali reikti pliusuoti, kad pataikyti i igreitejima
        }
        sp_L2 -= greitis;
      }
    }

    if (new_sp_R != sp_R2) {
      if (new_sp_R > sp_R2) {
        if (sp_R2 > 100) {
          rez.R = 300; // gali reikti pliusuoti, kad pataikyti i igreitejima
        }
        sp_R2 += greitis;
      }else if (new_sp_R < sp_R2) {
        if (sp_R2 < -100) {
          rez.R = 300 * -1; // gali reikti pliusuoti, kad pataikyti i igreitejima
        }
        sp_R2 -= greitis;
      }
    }
    ///
  
  return rez;
}

//impulsinis sukimas, naudojamas i viena puse, ratams sukti
void goWheelImpulse() {
    struct info rez = getSpeed();
  
    mySerial_L.write9(256);
    mySerial_L.write9(rez.L & 0xFF);
    mySerial_L.write9((rez.L >> 8) & 0xFF);
    mySerial_L.write9(rez.L & 0xFF);
    mySerial_L.write9((rez.L >> 8) & 0xFF);
    mySerial_L.write9(85);
    mySerial_L.write9(88);
    mySerial_L.write9(88);
    delayMicroseconds(300);
    
    mySerial_R.write9(256);
    mySerial_R.write9(rez.R & 0xFF);
    mySerial_R.write9((rez.R >> 8) & 0xFF);
    mySerial_R.write9(rez.R & 0xFF);
    mySerial_R.write9((rez.R >> 8) & 0xFF);
    mySerial_R.write9(85);
    mySerial_R.write9(88);
    mySerial_R.write9(88);
    delayMicroseconds(300);

    //Serial.print("goWheelImpulse >> ");
    //Serial.print(rez.L);
    //Serial.print(" | ");
    //Serial.print(rez.R);
    //Serial.print(" >> ");
}

//Suktis bleidui komandu siuntimas!
void goBlade() {
    int speedBlade = Pot_Screen_R;
    
    if (speedBlade > 105) {
      speedBlade = 105;
    }else if (speedBlade < -105) {
      speedBlade = -105;
    }

    if (!onBlade) {
      speedBlade = 0;
    }
    if (Pot_Screen_R == 0) {
      onBlade = true;
    }

  if (speedBlade == 100) {
      Serial2.write('1');
      Serial2.write('0');
      Serial2.write('0');
      Serial2.write('x');

      //Serial.println("goBlade >> 100x");
  }else{
    int speedBlade_t = speedBlade / 10;
    Serial2.write('0');
    Serial2.write(speedBlade_t + '0');
    int speedBlade_tt = speedBlade - (speedBlade_t * 10);
    Serial2.write((speedBlade_tt) + '0');
    Serial2.write('x');

    //Serial.print("goBlade >> 0");
    //Serial.print(speedBlade_t);
    //Serial.print(speedBlade_tt);
    //Serial.println("x");
  }
}

//nulipta, komandos siuntimas!
void stopWheel() {
    mySerial_L.write9(256);
    mySerial_L.write9(sp_L & 0xFF);
    mySerial_L.write9((sp_L >> 8) & 0xFF);
    mySerial_L.write9(sp_L & 0xFF);
    mySerial_L.write9((sp_L >> 8) & 0xFF);
    mySerial_L.write9(170);
    mySerial_L.write9(88);
    mySerial_L.write9(88);
    delayMicroseconds(300);
    
    mySerial_R.write9(256);
    mySerial_R.write9(sp_R & 0xFF);
    mySerial_R.write9((sp_R >> 8) & 0xFF);
    mySerial_R.write9(sp_R & 0xFF);
    mySerial_R.write9((sp_R >> 8) & 0xFF);
    mySerial_R.write9(170);
    mySerial_R.write9(88);
    mySerial_R.write9(88);
    delayMicroseconds(300);
  
  if (!powerOn) {
      Serial2.write('0');
      Serial2.write('0');
      Serial2.write('0');
      Serial2.write('x');
      //Serial.println("goBlade >> 000x");
  }
    //Serial.println("nulipta");
}

//Atstatyti i pirmine padeti, (paruosti startavimui)
void Reset() {
   sp_L = 0;
   sp_R = 0;
   sp_L2 = 0;
   sp_R2 = 0;
   new_sp_L = 0;
   new_sp_R = 0;
   onBlade = false;
}

//Energija ON i motinine
void PowerON() {
   powerOn = true;
   //Serial.println("Ijungiama elektros grandine ir paleidziama motinine");
   digitalWrite(IN2, HIGH);
   delay(300);
   digitalWrite(IN1, HIGH);
   delay(1000);
   digitalWrite(IN1, LOW);
   delay(200);
}

//Energija OFF i motinine
void PowerOFF() {
   powerOn = false;
   //Serial.println("Nutraukiama energijos grandine");
   digitalWrite(IN2, LOW);
   delay(2000);
}

