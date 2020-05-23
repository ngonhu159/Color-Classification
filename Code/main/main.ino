#include "function.h"
#include <Servo.h>

#define servoPin A0
#define out_Infrared 4        // Output cua cam bien hong ngoai

int counter_Red = 0;          // bien dem so luong san pham mau do - de hien thi LCD
int counter_Green = 0;        // bien dem so luong san pham mau xanh la - de hien thi LCD
int counter_Blue = 0;         // bien dem so luong san pham mau xanh duong - de hien thi LCD
int tempColor = 0;
int check_Infrared = 0;

int product_Code[255];        // mang chua so luong san pham theo mau. vi du: | 1 | 3 | 2 | 2 | .... tuong ung | Do | Xanh duong | Xanh la | Xanh la |
int number = 0;               // number la bien dung de dem tong so luong mau

Servo myservo;

void setup() {
  Init();
  myservo.attach(servoPin);
  myservo.write(0);

  pinMode(out_Infrared, INPUT);
}

void loop() {
  read_ColorSensor();             // doc mau san pham
  tempColor = detectColor();      // mau san pham duoc doc se duoc gan vao tempcolor tuong ung: 1 - mau do; 2 - mau xanh la; 3 - mau xanh duong; 0 - khong co mau
  switch(tempColor){
    case 1:
      counter_Red++;
      number++;                   
      product_Code[number-1] = tempColor;  
      break;
    case 2:
      counter_Green++;
      number++;
      product_Code[number-1] = tempColor;  
      break;
    case 3:
      counter_Blue++;
      number++;
      product_Code[number-1] = tempColor;   
      break;
    default:
      break;
  }
  
  if (!digitalRead(out_Infrared)){
    while (!digitalRead(out_Infrared));
    if (number>0){                                    // kiem tra neu con san pham nao tren bang truyen (number > 0) 
      switch(product_Code[0]){                        // kiem tra xem gia tri dau tien cua mang la bao nhieu (1 hay 2 hay 3) -> be goc servo khi co tin hieu tu cam bien hong ngoai         
        case 1:
          myservo.write(30);                          // goc servo 30 - mau do
          check_Infrared = 1;
          if (number>1){
            for(int i=0; i<number-1; i++){            // neu ban dau chi con 1 san pham tren bang truyen thi thoi, nguoc lai thi xoa bo san pham dau tien va dua cac san pham dich ra truoc trong mang
              product_Code[i] = product_Code[i+1];
            }    
            number--;                                 // tru lai 1 san pham 
          } else {
            number--;
          }
          break;
        case 2:
          myservo.write(150);
          if (number>1){
            for(int i=0; i<number-1; i++){
              product_Code[i] = product_Code[i+1];
            }    
            number--;
          } else {
            number--;
          }
          break;
        case 3:
          myservo.write(90); 
          if (number>1){
            for(int i=0; i<number-1; i++){
              product_Code[i] = product_Code[i+1];
            }    
            number--;
          } else {
            number--;
          }
          break;
      } 
    } 
   }
  Serial.println("Number: "+(String)number);
  
  dislayLCD(counter_Red, counter_Green, counter_Blue);          // hien thi ra LCD so luong san pham
  delay(500);                                                   // cho nay de delay thoi gian xac dinh san pham
}
