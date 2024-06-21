#include <Servo.h>

Servo servoZ; //orijin X servo
Servo servoY; //orijin Y servo
Servo servoD; //nerf bullet push (115 - 15) servo


int baslangic_taret_positionY = 85;
int baslangic_taret_positionZ = 100;
int dart_push_position = 15;
int dart_back_position = 115;

int hedefin_x_kordinat_acisi = 0;
int hedefin_y_kordinat_acisi = 0;
int hedefin_ters_X_koordinat_acisi = 0;
int hedefin_ters_Y_koordinat_acisi = 0;

int taret_Z_ekseni_ust_sinir_degeri = 180; //Z ekseninde ust sınır 180 derecedir
int taret_Z_ekseni_alt_sinir_degeri = 0; //Z ekseninde alt sınır 0 derecedir
int taret_Y_ekseni_ust_sinir_degeri = 125; //Y ekseninde ust sınır 40 derecedir, 85 (default) + 40 = 125
int taret_Y_ekseni_alt_sinir_degeri = 70; //Y ekseninde alt sınır 15 derecedir, 85 (default) - 15 = 70

int eski_Z_pozisyonu = 0;
int eski_Y_pozisyonu = 0;
int current_Z_position = 0;
int current_Y_position = 0;
int new_Z_position = 0;
int new_Y_position = 0;

int Z_ekseni_pozisyon_farki = 0;
int Y_ekseni_pozisyon_farki = 0;

int in1 = 9;
int in2 = 10;
int in3 = 11;
int in4 = 12;

int hedef_Z_koordinati_piksel_degeri = 0;
int hedef_Y_koordinati_piksel_degeri = 0;

int current_Z_pixel_coordinate = 0;
int current_Y_pixel_coordinate = 0;

int konumsal_Z_piksel_farki = 0;
int konumsal_Y_piksel_farki = 0;

int old_Z_pixel_coordinate = 0;
int old_Y_pixel_coordinate = 0;

bool moveServoD = false;

void setup() {
  delay(500); //Arduino 'nun kararli hale gecmesi icin 2 saniye sistemi bekletiyoruz.
  servoZ.attach(5);
  servoY.attach(7);
  servoD.attach(8);
  servoD.write(dart_back_position);
  delay(1000);
  servoZ.write(baslangic_taret_positionZ);
  delay(1000); 
  servoY.write(baslangic_taret_positionY);
  delay(1000);
  Serial.begin(115200); // Seri iletisimin baslatilması
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}

void loop() {
  //DC motorlarin H - Bridge ile surulmeye baslanmasi
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  servoD.write(dart_back_position);
  
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int delimiter = data.indexOf(",");

    //Asenkron iletisim
    if (delimiter != -1) {
      int Z_konumsal_hata = 0;
      int Y_konumsal_hata = 0;

      //Z için mevcut konum degerini piksel olarak algilamak
      current_Z_position = servoZ.read();
      old_Z_pixel_coordinate = map(new_Z_position, 0, 180, 0, 640);

      //Y için mevcut konum degerini piksel olarak algilamak
      current_Y_position = servoY.read();
      old_Y_pixel_coordinate = map(current_Y_position, 0, 180, 0, 480);

      //Gelen koordinat degerlerinin ayristirilmasi
      int centerX = data.substring(0, delimiter).toInt();
      int centerY = data.substring(delimiter + 1).toInt();
      
      //Goruntu 'nun piksel 'den aci degerine cevrilmesi 
      hedefin_ters_X_koordinat_acisi = map(centerX, 0, 640, 0, 180); 
      hedefin_ters_Y_koordinat_acisi = map(centerY, 0, 480, 0, 180);

      //Ters goruntuyu duzeltmek icin "180 - hedef aci degeri" formulu uygulanir.
      hedefin_x_kordinat_acisi = 180 - hedefin_ters_X_koordinat_acisi;
      hedefin_y_kordinat_acisi = 180 - hedefin_ters_Y_koordinat_acisi;

      //X ekseni icin koordinatlara oran verilmesi
      if(hedefin_x_kordinat_acisi > 160 && hedefin_x_kordinat_acisi <= 180){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi - 30;
      }
      else if(hedefin_x_kordinat_acisi > 140 && hedefin_x_kordinat_acisi <= 160){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi - 25;
      }
      else if(hedefin_x_kordinat_acisi > 120 && hedefin_x_kordinat_acisi <= 140){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi - 12;
      }
      else if(hedefin_x_kordinat_acisi > 110 && hedefin_x_kordinat_acisi <= 120){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi - 8;
      }
      else if(hedefin_x_kordinat_acisi > 105 && hedefin_x_kordinat_acisi <= 110){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi - 4;
      }
      else if(hedefin_x_kordinat_acisi > 100 && hedefin_x_kordinat_acisi <= 105){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi +5;
      }
      else if(hedefin_x_kordinat_acisi >= 0  && hedefin_x_kordinat_acisi < 20){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi + 55;
      }
      else if(hedefin_x_kordinat_acisi >= 20 && hedefin_x_kordinat_acisi <= 40){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi + 42;
      }
      else if(hedefin_x_kordinat_acisi > 40 && hedefin_x_kordinat_acisi <= 60){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi + 35;
      }
      else if(hedefin_x_kordinat_acisi > 60 && hedefin_x_kordinat_acisi <= 80){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi + 18;
      }
      else if(hedefin_x_kordinat_acisi > 80 && hedefin_x_kordinat_acisi <= 90){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi + 10;
      }
      else if(hedefin_x_kordinat_acisi > 90 && hedefin_x_kordinat_acisi <= 100){
        hedefin_x_kordinat_acisi = hedefin_x_kordinat_acisi + 4;
      }
      
      //Y ekseni icin koordinatlara oran verilmesi
      if(hedefin_y_kordinat_acisi > 120 && hedefin_y_kordinat_acisi <= 124){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 4;
      }
      else if(hedefin_y_kordinat_acisi > 115 && hedefin_y_kordinat_acisi <= 120){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 3;
      }
      else if(hedefin_y_kordinat_acisi > 105 && hedefin_y_kordinat_acisi <= 110){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 6;
      }
      else if(hedefin_y_kordinat_acisi > 100 && hedefin_y_kordinat_acisi <= 105){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 10;
      }
      else if(hedefin_y_kordinat_acisi > 90 && hedefin_y_kordinat_acisi <= 95){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 20;
      }
      else if(hedefin_y_kordinat_acisi > 85 && hedefin_y_kordinat_acisi <= 90){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 25;
      }
      else if(hedefin_y_kordinat_acisi > 75 && hedefin_y_kordinat_acisi <= 85){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 20;
      }
      else if(hedefin_y_kordinat_acisi >= 70 && hedefin_y_kordinat_acisi <= 75){
        hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 28;
      }
      
      new_Z_position = hedefin_x_kordinat_acisi;
      new_Y_position = hedefin_y_kordinat_acisi; 

      current_Z_position = servoZ.read();
      current_Y_position = servoY.read();

      if((new_Z_position >= 0 && new_Z_position <= 180) && (new_Y_position >= 70 && new_Y_position <= 125)){
        //Hedef X ve Y koordinat acilarinin piksel degerleri
        hedef_Z_koordinati_piksel_degeri = map(new_Z_position, 0, 180, 0, 640);
        hedef_Y_koordinati_piksel_degeri = map(new_Y_position, 0, 180, 0, 480);
        //default Z = 100, Y = 85
        Z_ekseni_pozisyon_farki = new_Z_position - current_Z_position;
        Y_ekseni_pozisyon_farki = new_Y_position - current_Y_position;
        
        //Z Servosunu dondurmek
        if(Z_ekseni_pozisyon_farki > 0){
          for(int i = 0; i <= Z_ekseni_pozisyon_farki; i++){
            servoZ.write(current_Z_position + i);
            delay(3);
          }
        }
        else if(Z_ekseni_pozisyon_farki < 0){
          Z_ekseni_pozisyon_farki = abs(Z_ekseni_pozisyon_farki);
          for(int i = 0; i <= Z_ekseni_pozisyon_farki; i++){
            servoZ.write(current_Z_position - i);
            delay(3);
          }
        }
        else if (Z_ekseni_pozisyon_farki == 0){
          servoZ.write(current_Z_position);
        }

        current_Z_position = servoZ.read();
        //Z servosu icin konumsal hata (Kp - error)
        Z_konumsal_hata = new_Z_position - current_Z_position;
        servoZ.write(current_Z_position + Z_konumsal_hata);


        //Y Servosunu dondurmek
        if(Y_ekseni_pozisyon_farki > 0){
          for(int i = 0; i <= Y_ekseni_pozisyon_farki; i++){
            servoY.write(current_Y_position + i);
            delay(3);
          }
        }

        else if(Y_ekseni_pozisyon_farki < 0){
          Y_ekseni_pozisyon_farki = abs(Y_ekseni_pozisyon_farki);
          for(int i = 0; i <= Y_ekseni_pozisyon_farki; i++){
            servoY.write(current_Y_position - i);
            delay(3);
          }
        }
        
        else if (Y_ekseni_pozisyon_farki == 0){
          servoY.write(current_Y_position);
        }

        current_Y_position = servoY.read();
        //Y servosu icin konumsal hata (Kp - error)
        Y_konumsal_hata = new_Y_position - current_Y_position;
        servoY.write(current_Y_position + Y_konumsal_hata);

        
        //Konumsal piksel farki + veya - deger olabilir
        konumsal_Z_piksel_farki = abs(hedef_Z_koordinati_piksel_degeri - old_Z_pixel_coordinate);
        konumsal_Y_piksel_farki = abs(hedef_Y_koordinati_piksel_degeri - old_Y_pixel_coordinate);

        // 640/180 = 3.555555 , Z için 1 derece = 3.5555 pikseldir, 3.5555 x 3 derece = 10,6665
        // 480/180 = 2.666666 , Y için 1 derece ) 2.6666 pikseldir, 2.6666 x 3 derece = 8
        //ServoD 'nin calisicagi zaman icin flag ayarlanmasi ve minimal piksel oynamalarinin onune gecilmesi 
        if (konumsal_Z_piksel_farki > 25 || konumsal_Y_piksel_farki > 18){
          moveServoD = true; //Servo D flag
        }
        
        //Nerf mermisini ileri/geri ittirmek
        if (moveServoD) {
          delay(20);
          servoD.write(dart_push_position);
          delay(350);
          servoD.write(dart_back_position);
          moveServoD = false;
        }
        else{
          servoD.write(dart_back_position);
          moveServoD = false;
        }
        
      }

      else if((new_Y_position < 70 || new_Y_position > 125) && (new_Z_position <= 0 && new_Z_position >= 180)){
        hedef_Z_koordinati_piksel_degeri = map(new_Z_position, 0, 180, 0, 640);

        //Z ekseni icin hareket
        Z_ekseni_pozisyon_farki = new_Z_position - current_Z_position;

        if(Z_ekseni_pozisyon_farki > 0){
          for(int i = 0; i <= Z_ekseni_pozisyon_farki; i++){
            servoZ.write(current_Z_position + i);
            delay(3);
          }
        }

        else if(Z_ekseni_pozisyon_farki < 0){
          Z_ekseni_pozisyon_farki = abs(Z_ekseni_pozisyon_farki);
          for(int i = 0; i <= Z_ekseni_pozisyon_farki; i++){
            servoZ.write(current_Z_position - i);
            delay(3);
          }
        }

        else if(Z_ekseni_pozisyon_farki == 0){
          servoZ.write(current_Z_position);
        }
        
        //Z servosu icin konumsal hata (Kp - error)
        current_Z_position = servoZ.read();
        Z_konumsal_hata = new_Z_position - current_Z_position;
        servoZ.write(current_Z_position + Z_konumsal_hata);


        //Y ekseni icin hareket
        if(new_Y_position > 125){
          Y_ekseni_pozisyon_farki = taret_Y_ekseni_ust_sinir_degeri - current_Y_position;
          for(int i = 0; i <= Y_ekseni_pozisyon_farki; i++){
            servoY.write(current_Y_position + i);
            delay(3);
          }
          //Y servosu icin konumsal hata (Kp - error)
          current_Y_position = servoY.read();
          //Y servosu icin konumsal hata
          Y_konumsal_hata = taret_Y_ekseni_ust_sinir_degeri - current_Y_position;
          servoY.write(current_Y_position + Y_konumsal_hata);

          hedef_Y_koordinati_piksel_degeri = map(taret_Y_ekseni_ust_sinir_degeri, 0, 180, 0, 480);
        }

        else if(new_Y_position < 70){
          //Min=88  , Max = 91
          if(hedefin_y_kordinat_acisi > 50 && hedefin_y_kordinat_acisi <= 69){
            hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 30;
          }
          else if(hedefin_y_kordinat_acisi > 40 && hedefin_y_kordinat_acisi <= 50){
            hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 40;
          }
          else if(hedefin_y_kordinat_acisi > 30 && hedefin_y_kordinat_acisi <= 40){
            hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 45;
          }
          else if(hedefin_y_kordinat_acisi > 20 && hedefin_y_kordinat_acisi <= 30){
            hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 55;
          }
          else if(hedefin_y_kordinat_acisi > 10 && hedefin_y_kordinat_acisi <= 20){
            hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 62;
          }
          else if(hedefin_y_kordinat_acisi <= 10){
            hedefin_y_kordinat_acisi = hedefin_y_kordinat_acisi + 70;
          }

          new_Y_position = hedefin_y_kordinat_acisi;

          Y_ekseni_pozisyon_farki = new_Y_position - current_Y_position;
          
          //Y Servosunu dondurmek
          if(Y_ekseni_pozisyon_farki > 0){
            for(int i = 0; i <= Y_ekseni_pozisyon_farki; i++){
              servoY.write(current_Y_position + i);
              delay(3);
            }
          }

          else if(Y_ekseni_pozisyon_farki < 0){
            Y_ekseni_pozisyon_farki = abs(Y_ekseni_pozisyon_farki);
            for(int i = 0; i <= Y_ekseni_pozisyon_farki; i++){
              servoY.write(current_Y_position - i);
              delay(3);
            }
          }
        
          else if (Y_ekseni_pozisyon_farki == 0){
            servoY.write(current_Y_position);
          }

          //Y servosu icin konumsal hata (error)
          current_Y_position = servoY.read();
          //Y servosu icin konumsal hata
          Y_konumsal_hata = new_Y_position - current_Y_position;
          servoY.write(current_Y_position + Y_konumsal_hata);

          hedef_Y_koordinati_piksel_degeri = map(new_Y_position, 0, 180, 0, 480);
        }
        
        //Konumsal piksel farki + veya - deger olabilir
        konumsal_Z_piksel_farki = abs(hedef_Z_koordinati_piksel_degeri - old_Z_pixel_coordinate);
        konumsal_Y_piksel_farki = abs(hedef_Y_koordinati_piksel_degeri - old_Y_pixel_coordinate);
        
        // 640/180 = 3.555555 , Z için 1 derece = 3.5555 pikseldir, 3.5555 x 3 derece = 10,6665
        // 480/180 = 2.666666 , Y için 1 derece ) 2.6666 pikseldir, 2.6666 x 3 derece = 8
        //ServoD 'nin calisicagi zaman icin flag ayarlanmasi ve minimal piksel oynamalarinin onune gecilmesi 
        if (konumsal_Z_piksel_farki > 25 || konumsal_Y_piksel_farki > 18){
          moveServoD = true; //Servo D flag
        }

        //Nerf mermisini ileri/geri ittirmek
        if (moveServoD) {
          delay(20); //Taretin hedefe kilitlenmesi icin kisa bir sure bekletiyoruz.
          servoD.write(dart_push_position);
          delay(350);
          servoD.write(dart_back_position);
          moveServoD = false;
        }
        else{
          servoD.write(dart_back_position);
          moveServoD = false;
        }
        
      }

      
    }
  }

}