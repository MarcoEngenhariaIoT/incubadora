//Projeto incubadora PID
//Módulo LCD e teclado
//Master
#include <Wire.h>  //Inicializa o protocolo I2C
#include <LiquidCrystal.h> //Inicializa a biblioteca do display
#include <EEPROM.h>  //Inicializa a biblioteca da EEPROM

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);  //Configura o LCD

//Variáveis
int onOff = 0;
int comando = 0;
int dado = 0;
int tempSet = 0;
byte contador = 0;
int externo;
int interno;
int lampada;
int T = 16;
byte E = 0;
int X = 1;
int L1 = 2;
int L2 = 3;
int L3 = 4;
int L4 = 5;
int C1 = A0;
int C2 = A1;
int C3 = A2;
int C4 = A3;

//Setup do Arduino
void setup(){
  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  digitalWrite(L1, HIGH);
  digitalWrite(L2, HIGH);
  digitalWrite(L3, HIGH);
  digitalWrite(L4, HIGH);
  Serial.begin(1000000);
  Wire.begin(1);
  Wire.onReceive(evento);
  tempSet=EEPROM.read(0x00);  
  lcd.begin(16, 2);
  inicializaLcd();
  atualiza();
}

//Loop principal
void loop(){
  teclado();
  delay(10);
}

//Função que recebe os dados vindos o Slave
void evento(void){
  while (1 < Wire.available()){ 
    externo = int(Wire.read());
    interno = int(Wire.read());
    lampada = int(Wire.read());
    atualiza();
  }
}

//Função que atualiza o display
void atualiza(void){
  if(contador==5){
    lcd.setCursor(4, 0);
    lcd.print("   ");
    lcd.setCursor(4, 0);
    lcd.print(interno, DEC);
    
    lcd.setCursor(11, 0);
    lcd.print("   ");
    lcd.setCursor(11, 0);  
    lcd.print(externo, DEC);
    
    lcd.setCursor(4, 1);
    lcd.print("   ");
    lcd.setCursor(4, 1);
    lcd.print(tempSet, DEC);
    
    lcd.setCursor(11, 1);
    lcd.print("   ");
    lcd.setCursor(11, 1);  
    lcd.print(lampada, DEC);

    Serial.print("SetPoint: ");
    Serial.print(tempSet);
    Serial.print("°C    ");
        
    Serial.print("Externo: ");
    Serial.print(externo);
    Serial.print("°C    ");

    Serial.print("Interno: ");
    Serial.print(interno);
    Serial.print("°C    ");

    Serial.print("Lâmpada: ");
    Serial.print(lampada);
    Serial.println("°C");    
    contador=0;
  }
  contador=contador+1;
}

//Função de envio de dados para o Slave
void envia(void){
  Wire.beginTransmission(2);
  Wire.write(byte(comando));
  Wire.write(byte(dado));
  Wire.endTransmission();
  delay(10);
  Serial.print(comando);
  Serial.println(dado);
  comando=0;
  dado=0;
  T=16;
}

//Função que inicializa o display
void inicializaLcd(void){
  lcd.clear();
  lcd.setCursor(0, 0);
  //       ("0123456789ABCDEF")
  lcd.print("IN:    EX:    *C");
  lcd.setCursor(0, 1);
  lcd.print("SP:    LA:    *C");
}

//Função para leitura do teclado
void teclado(void){
  digitalWrite(L1, LOW);  //Colola a linha L1 em "0"
  X = analogRead(C1);     //Lê "C1", se estiver em "0", Tecla "1" foi pressionada
  if (X<512){
    T = 1;
    while(X<512){         //Debounce
      X = analogRead(C1); //"1"
      delay(10);
    }
  }
  X = analogRead(C2);     //"2"
  if (X<512){
    T = 2;
    while(X<512){
      X = analogRead(C2);
      delay(10);
    }
  }
  X = analogRead(C3);     //"3"
  if (X<512){
    T = 3;
    while(X<512){
      X = analogRead(C3);
      delay(10);
    }
  }
  X = analogRead(C4);    //"A"
  if (X<512){
    T = 10;
    while(X<512){
      X = analogRead(C4);
      delay(10);
    }
  }
  digitalWrite(L1, HIGH);   //Depois que todas as teclas da linha 1 foram varridas, "L1" volta a ser "1"

  digitalWrite(L2, LOW);
  X = analogRead(C1);     //"4"
  if (X<512){
    T = 4;
    while(X<512){
      X = analogRead(C1);
      delay(10);
    }
  }
  X = analogRead(C2);     //"5"
  if (X<512){
    T = 5;
    while(X<512){
      X = analogRead(C2);
      delay(10);
    }
  }
  X = analogRead(C3);     //"6"
  if (X<512){
    T = 6;
    while(X<512){
      X = analogRead(C3);
      delay(10);
    }
  }
  X = analogRead(C4);     //"B"
  if (X<512){
    T = 11;
    while(X<512){
      X = analogRead(C4);
      delay(10);
    }
  }
  digitalWrite(L2, HIGH);

  digitalWrite(L3, LOW);
  X = analogRead(C1);     //"7"
  if (X<512){
    T = 7;
    while(X<512){
      X = analogRead(C1);
      delay(10);
    }
  }
  X = analogRead(C2);     //"8"
  if (X<512){
    T = 8;
    while(X<512){
      X = analogRead(C2);
      delay(10);
    }
  }
  X = analogRead(C3);     //"9"
  if (X<512){
    T = 9;
    while(X<512){
      X = analogRead(C3);
      delay(10);
    }
  }
  X = analogRead(C4);     //"C"
  if (X<512){
    T = 12;
    while(X<512){
      X = analogRead(C4);
      delay(10);
    }
  }
  digitalWrite(L3, HIGH);

  digitalWrite(L4, LOW);
  X = analogRead(C1);     //"*"
  if (X<512){
    T = 13;
    int i=0;
    while(X<512){
      X = analogRead(C1);
      delay(10);
      i++;
      if(i==200){ //Segura dois segundos e resseta os dois arduinos
        comando=13;
        envia();
        setup();
      }
    }
  }
  X = analogRead(C2);     //"0"
  if (X<512){
    T = 0;
    while(X<512){
      X = analogRead(C2);
      delay(10);
    }
  }
  X = analogRead(C3);     //"#"
  if (X<512){
    T = 14;
    while(X<512){
      X = analogRead(C3);
      delay(10);
    }
  }
  X = analogRead(C4);     //"D"
  if (X<512){
    T = 15;
    while(X<512){
      X = analogRead(C4);
      delay(10);
    }
  }
  digitalWrite(L4, HIGH);

  if(T!=16){
    if(T==10){
      if(tempSet<100){
        tempSet=tempSet+1;
        comando=10;
        dado=tempSet;
      }
      else{
        tempSet=100;
      }
    }
  
    if(T==12){
      comando=12;
    }

    if(T==11){
      if(tempSet>20){
        tempSet=tempSet-1;
        comando=10;
        dado=tempSet;        
      }
      else{
        tempSet=20;
      }
    }

    if(T==15){
      if(onOff==0){
        onOff=1;
        comando=15;
        dado=1;
      }
      else{
        onOff=0;
        comando=15;
        dado=0;
      }
    }
  
//Salva na EEPROM o SetPoint
    EEPROM.write(0x00, byte(tempSet));
    envia();
    atualiza();
  }
}
