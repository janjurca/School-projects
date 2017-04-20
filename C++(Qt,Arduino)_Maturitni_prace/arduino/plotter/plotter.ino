#include <Servo.h>

Servo servo; 
int x_motor_1 = 6;
int x_motor_2 = 7;
int x_motor_3 = 8;
int x_motor_4 = 9;

int y_motor_1 = 2;
int y_motor_2 = 3;
int y_motor_3 = 4;
int y_motor_4 = 5;

int servo_pin = 11;
int x_motor_stav = 0;
int y_motor_stav = 0;

int prijato_int;  
int rychlost = 1;
int stav;
int pocitadlo;
int tlacitko_x = 10;
int tlacitko_y= 12;

long int x_max = 38500;
long int y_max = 43000;

long int x_aktualne = 0;
long int y_aktualne = 0;

void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  Serial.setTimeout(10);
  servo.attach(servo_pin);
  servo.write(40);
  pinMode(x_motor_1, OUTPUT);
  pinMode(x_motor_2, OUTPUT);
  pinMode(x_motor_3, OUTPUT);
  pinMode(x_motor_4, OUTPUT);
  pinMode(y_motor_1, OUTPUT);
  pinMode(y_motor_2, OUTPUT);
  pinMode(y_motor_3, OUTPUT);
  pinMode(y_motor_4, OUTPUT);
  pinMode(tlacitko_x, INPUT);
  pinMode(tlacitko_y, INPUT); 
  jdi_domu();

}

void loop() {
       

        // send data only when you receive data:
        while (Serial.available() > 0) {
                          
                prijato_int = Serial.parseInt();
                
                if(prijato_int == 600) // na prijeti servo dat
                {
                  stav = 600;
                  Serial.write("c");                  
                }
                else if(prijato_int == 601) // na prijeti x dat
                {
                  stav = 601;
                  Serial.write("c");                  
                }
                else if(prijato_int == 602) // na prijeti y dat
                {
                  stav = 602;
                  Serial.write("c");                  
                }
                else{                
                if(stav == 600){
                  int vyska;
                  if( prijato_int > 0) vyska = 40;
                  if( prijato_int == 0) vyska = 1;
                    servo.write(vyska);
                    Serial.write("c");
                    stav = 0;                   
                }
                if(stav == 601){
                    x_motor(prijato_int);
                    Serial.write("c");
                    stav = 0;
                }
                if(stav == 602){
                    y_motor(prijato_int);
                    Serial.write("c");
                    stav = 0;                   
                }       
        }
        }
}

void pohni_servem(int hodnota){
  servo.write(hodnota);  
}
 
void x_motor (int pocet_kroku){
  if(pocet_kroku > 0){
    for(int i = 0; i < pocet_kroku; i++){
      if(x_aktualne > x_max) break;
      x_aktualne++;
      if(x_motor_stav > 7)x_motor_stav = 0;
      
      //Serial.write("ok");
      if(x_motor_stav ==0){
      digitalWrite(x_motor_1, HIGH);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, HIGH);      
      delay(rychlost);
      }
      if(x_motor_stav ==1){
      digitalWrite(x_motor_1, HIGH);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==2){
      digitalWrite(x_motor_1, HIGH);
      digitalWrite(x_motor_2, HIGH);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==3){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, HIGH);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==4){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, HIGH);
      digitalWrite(x_motor_3, HIGH);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==5){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, HIGH);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==6){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, HIGH);
      digitalWrite(x_motor_4, HIGH);      
      delay(rychlost);
      }
      if(x_motor_stav ==7){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, HIGH);      
      delay(rychlost);
      }
      x_motor_stav++; 
    } 
      
  }
  if(pocet_kroku < 0 ){
    for(int i = 0; i > pocet_kroku; i--){
      x_aktualne--;
      if(x_motor_stav > 7)x_motor_stav = 0;
      if(digitalRead(tlacitko_x) == HIGH) {
        //Serial.write("k");
        break;
      }
      if(x_motor_stav ==0){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, HIGH);      
      delay(rychlost); 
      }
      if(x_motor_stav ==1){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, HIGH);
      digitalWrite(x_motor_4, HIGH);      
      delay(rychlost);
      }
      if(x_motor_stav ==2){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, HIGH);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost); 
      }
      if(x_motor_stav ==3){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, HIGH);
      digitalWrite(x_motor_3, HIGH);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);   
      }
      if(x_motor_stav ==4){
      digitalWrite(x_motor_1, LOW);
      digitalWrite(x_motor_2, HIGH);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==5){
      digitalWrite(x_motor_1, HIGH);
      digitalWrite(x_motor_2, HIGH);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==6){
      digitalWrite(x_motor_1, HIGH);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, LOW);      
      delay(rychlost);
      }
      if(x_motor_stav ==7){
      digitalWrite(x_motor_1, HIGH);
      digitalWrite(x_motor_2, LOW);
      digitalWrite(x_motor_3, LOW);
      digitalWrite(x_motor_4, HIGH);      
      delay(rychlost);
      }
      x_motor_stav++;   
    }    
  }  
}

void y_motor (int pocet_kroku){
  if(pocet_kroku > 0){
    for(int i = 0; i < pocet_kroku; i++){
      if(y_aktualne > y_max) break;
      y_aktualne++;
      if(y_motor_stav > 7)y_motor_stav = 0;
      //Serial.write("ok");
      if(y_motor_stav == 0){
      digitalWrite(y_motor_1, HIGH);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, HIGH);      
      delay(rychlost);
      }
      if(y_motor_stav == 1){
      digitalWrite(y_motor_1, HIGH);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 2){
      digitalWrite(y_motor_1, HIGH);
      digitalWrite(y_motor_2, HIGH);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 3){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, HIGH);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 4){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, HIGH);
      digitalWrite(y_motor_3, HIGH);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 5){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, HIGH);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 6){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, HIGH);
      digitalWrite(y_motor_4, HIGH);      
      delay(rychlost);
      }
      if(y_motor_stav == 7){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, HIGH);      
      delay(rychlost);
      }
      y_motor_stav++;
    }    
  }
  if(pocet_kroku < 0){
    for(int i = 0; i > pocet_kroku; i--){
      y_aktualne--;
      if(y_motor_stav > 7)y_motor_stav = 0;
      if(digitalRead(tlacitko_y) == HIGH) {
        //Serial.write("k");
        break;
      }
      
      if(y_motor_stav == 0){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, HIGH);      
      delay(rychlost); 
      }
      if(y_motor_stav == 1){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, HIGH);
      digitalWrite(y_motor_4, HIGH);      
      delay(rychlost);
      }
      if(y_motor_stav == 2){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, HIGH);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost); 
      }
      if(y_motor_stav == 3){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, HIGH);
      digitalWrite(y_motor_3, HIGH);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);   
      }
      if(y_motor_stav == 4){
      digitalWrite(y_motor_1, LOW);
      digitalWrite(y_motor_2, HIGH);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 5){
      digitalWrite(y_motor_1, HIGH);
      digitalWrite(y_motor_2, HIGH);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 6){
      digitalWrite(y_motor_1, HIGH);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, LOW);      
      delay(rychlost);
      }
      if(y_motor_stav == 7){
      digitalWrite(y_motor_1, HIGH);
      digitalWrite(y_motor_2, LOW);
      digitalWrite(y_motor_3, LOW);
      digitalWrite(y_motor_4, HIGH);      
      delay(rychlost); 
      }  
      y_motor_stav++;
       
    }    
  }  
}

void jdi_domu(){
  while(digitalRead(tlacitko_x) == LOW){
      x_motor(-1);
//      Serial.println("a");
  }
  while(digitalRead(tlacitko_y) == LOW){
      y_motor(-1);
//      Serial.println("a");
  }
  
  digitalWrite(x_motor_1, LOW);
  digitalWrite(x_motor_2, LOW);
  digitalWrite(x_motor_3, LOW);
  digitalWrite(x_motor_4, LOW);
  
  digitalWrite(y_motor_1, LOW);
  digitalWrite(y_motor_2, LOW);
  digitalWrite(y_motor_3, LOW);
  digitalWrite(y_motor_4, LOW);
  x_aktualne = 0;
  y_aktualne = 0;
  
}

