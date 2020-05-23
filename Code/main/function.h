#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


#define outGY 5

#define led_RedPin  9
#define led_BluePin  10
#define led_GreenPin  8

#define redMax    55
#define redMin    45
#define greenMax  65
#define greenMin  55
#define blueMax   75
#define blueMin   55

/*
  #define S0 3
  #define S1 2
  #define S2 6
  #define S3 7
 */
int sPin_GY[4] = {3, 2, 6, 7};          // cac pin S cua cam bien mau GY-31
int ledPin[4] = {8,9,10,11};            // cac pin led
int ledStatus[4] = {0,0,0,0};           // trang thai cua led 0 hoac 1

int redColor_Frequency = 0;             // gia tri thoi giac xac dinh cho tung mau tuong ung
int greenColor_Frequency = 0;
int blueColor_Frequency = 0;

void Init(){
  Serial.begin(9600);
  lcd.init();         //Khởi động màn hình. Bắt đầu cho phép Arduino sử dụng màn hình
  lcd.backlight();    //Bật đèn nền
  for(int i=0; i<4; i++){
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], HIGH);

    pinMode(sPin_GY[i], OUTPUT);
  }

  // Set S0 = 1, S1 = 0 => Scale tan so 20%
  digitalWrite(sPin_GY[0], HIGH);
  digitalWrite(sPin_GY[1], LOW);

  // Set S2 = 1, S3 = 0 => Chon che do Clear(No filter)
  digitalWrite(sPin_GY[2], HIGH);
  digitalWrite(sPin_GY[3], LOW);
  
  pinMode(outGY, INPUT);
}

void read_ColorSensor(){
  // chon mode detect mau do
  digitalWrite(sPin_GY[2], LOW);
  digitalWrite(sPin_GY[3], LOW);
  redColor_Frequency = pulseIn(outGY, LOW);

  // chon mode detect mau xanh duong
  digitalWrite(sPin_GY[2], LOW);
  digitalWrite(sPin_GY[3], HIGH);
  blueColor_Frequency = pulseIn(outGY, LOW);  

  // chon mode detect mau xanh duong
  digitalWrite(sPin_GY[2], HIGH);
  digitalWrite(sPin_GY[3], HIGH);
  greenColor_Frequency = pulseIn(outGY, LOW);

  Serial.println("Red: " + (String)redColor_Frequency + " Blue: " + (String)blueColor_Frequency + " Green: " + (String)greenColor_Frequency);
}

int detectColor(){
    // kiem tra cac gia tri vua detect ben tren thuoc truong hop nao, tra ve gia tri cho ham: 1 - do; 2 - xanh la; 3 - xanh duong; 0 - khong mau
    if ((redColor_Frequency<=redMax)&&(redColor_Frequency>=redMin)){
      Serial.println("Red color");
      digitalWrite(led_RedPin, LOW);
      digitalWrite(led_BluePin, HIGH);
      digitalWrite(led_GreenPin, HIGH);
      return 1;
    } else if ((greenColor_Frequency<=greenMax)&&(greenColor_Frequency>=greenMin)){
      Serial.println("Green color");
      digitalWrite(led_RedPin, HIGH);
      digitalWrite(led_BluePin, HIGH);
      digitalWrite(led_GreenPin, LOW);
      return 2;
    } else if ((blueColor_Frequency<=blueMax)&&(blueColor_Frequency>=blueMin)){
      Serial.println("Blue color");
      digitalWrite(led_RedPin, HIGH);
      digitalWrite(led_BluePin, LOW);
      digitalWrite(led_GreenPin, HIGH);
      return 3;
    } else {
      Serial.println("No color");
      digitalWrite(led_RedPin, HIGH);
      digitalWrite(led_BluePin, HIGH);
      digitalWrite(led_GreenPin, HIGH);
      return 0;
    }
}

void dislayLCD(int red, int green, int blue){
  lcd.setCursor(4,0);                       // chon vi tri hien thi bat dau tai cot 4, dong 0
  lcd.print("Red:" + (String)red);

  lcd.setCursor(0,1);                       // chon vi tri hien thi bat dau tai cot 0, dong 1
  lcd.print("Green:" + (String)green);

  lcd.setCursor(9,1);                       // chon vi tri hien thi bat dau tai cot 9, dong 1
  lcd.print("Blue:" + (String)blue);
}
