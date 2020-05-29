#include <Stepper.h>
#include <Servo.h>  
Servo servoNesnesi;

int motorPin1 = 4;  
int motorPin2 = 5; 
int motorPin3 = 6; 
int motorPin4 = 7;

#define ADIM 128   
Stepper stepKontrol(ADIM, motorPin1, motorPin3, motorPin2, motorPin4);  
int adimSayisi; 
const int yagmur_sensor=A2;
const int toprak_sensor=A0;
int toprak_veri;
const int k_led=9;;
const int y_led=10;
const int m_led=11;

char veri;
int durum = 0;
int acik=0;

int su_seviye=A3;


void setup() {


  attachInterrupt(0,manual_ac,FALLING);
  pinMode(yagmur_sensor,INPUT);
  pinMode(toprak_sensor,INPUT);
  pinMode(k_led,OUTPUT);
  pinMode(m_led,OUTPUT);
  pinMode(y_led,OUTPUT);
  servoNesnesi.attach(12);
  Serial.begin(9600);
  stepKontrol.setSpeed(400);
}

void loop() {
  
  bluetooth();
  yagmur_kontrol();
  delay(500);
  toprak_kontrol();
  delay(500);
  suSeviye();
  delay(500);


}

void toprak_kontrol() {
  toprak_veri=analogRead(toprak_sensor);
  Serial.print("toprak veri:");
  Serial.println(toprak_veri);

  if(toprak_veri > 900) {
    Serial.println("toprak kuru");
    digitalWrite(m_led,HIGH);
    digitalWrite(y_led,HIGH);
    digitalWrite(k_led,LOW);
    delay(5000);

  }
  if(toprak_veri <= 900 && toprak_veri > 550) { 
    Serial.println("toprak optimum");
     digitalWrite(m_led,HIGH);
     digitalWrite(y_led,LOW);
     digitalWrite(k_led,LOW);
     delay(5000);

  }
  if(toprak_veri <= 550) {
    Serial.println("toprak nemli");
    digitalWrite(m_led,HIGH);
    digitalWrite(y_led,LOW);
    digitalWrite(k_led,HIGH);
    delay(5000);
  }
 
}

int yagmur_kontrol() {
  
  int yagmur=analogRead(yagmur_sensor);

   if(yagmur < 900) {
    Serial.println("*********");
    Serial.println(yagmur);
    Serial.println("yagmur yagiyo");
    if(durum==0) {
      cati_ac();
      durum=1;
    }
    
    
  }
  else {
    Serial.println(yagmur);
    Serial.println("yagmur yagmiyo");
    if(durum==1){
      cati_kapat();
      durum=0; 
    }
   
  }
  delay(1000);
  
}

void manual_ac(){
  Serial.println("manual cati hareketi");
  int rDeger=random(0,255);
  int gDeger=random(0,255);
  int bDeger=random(0,255);
  analogWrite(k_led,rDeger);
  analogWrite(m_led,gDeger);
  analogWrite(y_led,bDeger);
  delay(1000);

    
}

void cati_ac(){

  stepKontrol.setSpeed(200); //Motorun dönüş hızı belirtiliyor
 
  adimSayisi = 7500; // Saat yönünde atılacak adim sayısı
 
  stepKontrol.step(adimSayisi);

}

void cati_kapat(){

   stepKontrol.setSpeed(200);
 
  adimSayisi = -7500; // Motorun ters yönde atacağı adım belirleniyor
 
  stepKontrol.step(adimSayisi);
   
}


void bluetooth() {
  
     char veri=Serial.read();
         if(veri== '1') {
         
            stepKontrol.setSpeed(200);
         
            adimSayisi = -7500; // Motorun ters yönde atacağı adım belirleniyor
         
            stepKontrol.step(adimSayisi);
         
 
     }
     if(veri=='0') {
        stepKontrol.setSpeed(200); //Motorun dönüş hızı belirtiliyor
 
         adimSayisi = 7500; // Saat yönünde atılacak adim sayısı
       
        stepKontrol.step(adimSayisi);

     }
     
 }

 void suSeviye() {
  int seviye=analogRead(su_seviye);
  Serial.print("su seviye :");
  Serial.println(seviye);
  
  if(seviye <= 300) {
    Serial.print("su seviyesi iyi :");
    Serial.print(seviye);
    barajKapaklariKapat();
  }

  if (seviye > 500) {
     //Servo acik
    Serial.print("su seviyesi fazla, havuz boşaltılıyor :"); 
    Serial.print(seviye);
    barajKapaklariAc();
  }
 }

 void barajKapaklariAc(){
  servoNesnesi.write(90);  /* Motorun mili 270. dereceye donuyor */
 }
void barajKapaklariKapat(){
  servoNesnesi.write(270);  /* Motorun mili 90. dereceye donuyor */
 }
  
