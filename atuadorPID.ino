//Projeto incubadora PID
//Atuador
//Sensores
//PID
//Slave

#include <EEPROM.h>   //Biblioteca da memória EEPROM
#include <Wire.h>     //Biblioteca do protocolo I2C
float lampada = 0, externo = 0, interno = 0;    //Variáveis dos sensores de temperatura
int setPoint = EEPROM.read(1);   //Variável do set point armazenado na eeprom         
int pwm = 0, beep = 100;  //variável do estado inicial do PWM e valor padrão do beep

float xf1=0;  //Variáveis do filtro IIR
float xf2=0;
float yf1=0;
float yf2=0;

int LM2=3;    // LM35
int atuador = 3;      // Saída PWM
int M=EEPROM.read(0);
int comando=0;
int dado=0;
float y=0,x1=0,x2=0,y2=0,n=0,yy;
float T=0.1;
int manual=0;
float x;
float a0,a1,a2,b0,b1,b2; 
float Kp=1,Kd=0,Ki=0.05,y19,z,y1; 

//Setup do Arduino
void setup() {
  Serial.begin(1000000); //inicializa a Serial
  Wire.begin(2);  //Inicializa o I2C ou TWO Wire
  Wire.onReceive(evento); //Sempre que haver uma interrupção de recebimento de dado, é chamada essa função
  pinMode(3, OUTPUT); //Configura o pino como saída
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);  //Seta o pino em nível alto, ou beep de inicialização do sistema
  delay(200); //Beep de inicialização
  digitalWrite(2, LOW);  //Seta o pino em nível baixo
  configura_PID();    //Configura o PID
}

//Loop principal
void loop() {
    recebe();
    temperatura();
    PID();
}

//Inicializa filtro, é necessário que sempre seja incializado toda vez que muda de estado o PID
void iniciaFiltro(void){
  //condições iniciais no cabeçalho e em todas as partes onde houver transição para o filtro
  xf1=0;
  xf2=0;
  yf1=0;
  yf2=0;
}

//Função do filtro discreto IIR
float filtroIIR(float x){
  y=0.0674552*(x+2*xf1+xf2)-0.412*yf2+1.14298*yf1;
  xf2=xf1;
  yf2=yf1;
  xf1=x;
  yf1=y;
  return y;
}

void recebe(void){
  if(comando==15&&dado==1){ //Recebe o estado que deve atuar o PID em malha fechada "0"
    M=0;
    EEPROM.write(0,M);
    digitalWrite(2, HIGH); //beep curto
    delay(200);
    digitalWrite(2, LOW);
  }
  if(comando==15&&dado==0){ //Recebe o estado que deve atuar o PID no modo desligado "3"
    M=3;
    EEPROM.write(0,M);
    digitalWrite(2, HIGH); //beep longo
    delay(500);
    digitalWrite(2, LOW);    //Serial.println(M);
  }
  if(comando==12){    //Recebe o estado que deve atuar o PID em malha aberta "1"
    M=1;
    EEPROM.write(0,M);
    digitalWrite(2, HIGH);  //beep curto
    delay(200);
    digitalWrite(2, LOW);
  }
  if(comando==10){  //Seta Set Point
    setPoint=int(dado);
    EEPROM.write(1,setPoint); //Salva SetPoint na EEPROM
    digitalWrite(2, HIGH);    //Beep 
    delay(100);
    digitalWrite(2, LOW);
  }
  if(comando==13){  //Resset, reinicializa o sistema
    setup();
  }
}

//Função PID
void PID(void){
  //Função PID malha fechada
  if(M==0){
    iniciaFiltro(); //Filtro IIR
    while (M!=1 && M!=3){
      temperatura(); 
      x=analogRead(A3);
      x=x*0.488758553;
      x=filtroIIR(x);
      Serial.println(x);
      if(x>=120){
       analogWrite(atuador,0); 
      }else{
      x=setPoint-x;
      y=(b2*x2+b1*x1+b0*x-a2*y2)/a0;
      if (n>0){
        x2=x1;
        y2=y1;
      }
      if (n==2){
        n=0;
      }
      x1=x;
      y1=y;
      if(y>=500){y=500;}
      if(y<0){y=0;}   
      yy=((y*255)/500);
      analogWrite(atuador, yy);
      Serial.println(yy);
      n++;
      delay(100);
    }
    }
  }

  //função do PID em malha aberta
  else if(M==1){
    iniciaFiltro(); //Filtro IIR
    while(M!=0 && M!=3){
      x1=0;y1=0;x2=0;y2=0;n=0;
      temperatura();
      delay(10);
      x=analogRead(A3);
      x=x*0.488758553;
      x=filtroIIR(x);
      Serial.println(x);
      if(x>=120){
        analogWrite(atuador, 0);
        Serial.println(0);
      }
      else{  
        yy=255*(setPoint-20)/100; 
        analogWrite(atuador, yy);
        Serial.println(yy); 
    }
    delay(100); 
    }
  }

  //Função para desligar o PID
  else if(M==3){
    iniciaFiltro();
    Serial.println(x);
    while(M!=0 && M!=1){
      temperatura();
      x1=0;y1=0;x2=0;y2=0;n=0;
      analogWrite(atuador, 0);
      Serial.println(0);
      delay(100); 
    }
  }
}

//Função que configura o PID
void configura_PID(void){
  a0=2*T;
  a2=-2*T;
  b0=4*Kd+(Ki*T*T)+2*Kp*T;
  b1=2*Ki*T*T-8*Kd;
  b2=4*Kd+Ki*T*T-2*Kp*T;
}

//Função de leitura das temperaturas e envio para o Master mostgrar no display
void temperatura(void){
  lampada = (float(analogRead(3))*500/(1023));
  interno = (float(analogRead(2))*500/(1023));
  externo = (float(analogRead(1))*500/(1023));
  envia();
}

//Função de nvio de dados para o Master I2C
void envia(void){
  Wire.beginTransmission(1);
  Wire.write(byte(externo));
  Wire.write(byte(interno));
  Wire.write(byte(lampada));
  Wire.endTransmission();
  delay(10);
}

//Sempre que recebe um comando I2C vindo do Master, gera uma interrupção que é tratada nessa função
void evento(void){
  while(1<Wire.available()){
    comando=int(Wire.read());
    dado=int(Wire.read());  
    recebe();
  }
}
