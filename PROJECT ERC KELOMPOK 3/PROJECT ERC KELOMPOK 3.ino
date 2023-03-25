#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 13;
const int echoPin = 12;
const int suara = A0;
int led = 10;
bool realCarJarak = false;
bool realCarSuara = false;
String ada = "Parkir tersedia";
String tidakAda = "PARKIR HABIS";
int parkirTerisi = 0;
int jumlahParkir = 476;

int sisaParkir;
int duration;
int jarak;
int valueSuara;

String message (int a){
  String b;
  b = a;
  return b + " Slot lagi!";
}

void read(){
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int jarak = duration / 58;

  valueSuara = analogRead(suara);
  jarak < 20? realCarJarak = true : realCarJarak = false;
  valueSuara > 100? realCarSuara = true : realCarSuara = false;

  sisaParkir = jumlahParkir - parkirTerisi;
  if(sisaParkir != 0){
  lcd.setCursor(0, 0);
  lcd.print(ada);
  lcd.setCursor(0, 1);
  lcd.print(message (sisaParkir));

  }else{
  lcd.setCursor(0, 0);
  lcd.print(tidakAda);
  lcd.setCursor(0, 1);
  lcd.print("----------------");
  }

}

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(suara, INPUT);
  pinMode(led, OUTPUT);

  lcd.init();
  lcd.backlight();
}

void loop() {
  read();

  //Untuk mengetahui kondisi prakir pada monitor serial
  Serial.print("Ada mobil : ");
  Serial.println(realCarJarak); // 1 = ada mobil/objek, 0 = tidak ada mobil/objek
  Serial.print("Suara Mobil : ");
  Serial.println(realCarSuara); // 1 = ada suara mobil, 0 = tidak ada suara mobil
  Serial.println("==========================================="); // pemisah

                                        //PENJELASAN PER SYNTAX
  if(!realCarJarak && !realCarSuara || //gaada mobil dan gaada suara
      !realCarJarak && realCarSuara){ // gaada mobil dan ada suara
    digitalWrite(led, HIGH);          //lampu nyala
  }else if(realCarJarak && realCarSuara){//ada mobil dan ada suara maka akan masuk infinite loop
  parkirTerisi++;
  digitalWrite(led, LOW);               //lampu mati
    while(true){
      read();                           //dibaca lagi
    if(!realCarJarak){ //jika sudah gaada mobil dan suara lagi maka akan keluar infinite loop
    parkirTerisi--;
      break;
      }

    }
    }
  delay(1000);
}
