#include <RCSwitch.h>
#include <Timers.h>

#define KOD_1 5034200
#define KOD_2 5034196
#define KOD_3 5034204
#define KOD_4 5034194
#define KOD_5 5034202
#define KOD_6 5034198
#define KOD_7 5034206
#define KOD_8 5034193
#define KOD_9 5034201
#define KOD_10 5034197
#define KOD_11 5034205
#define KOD_12 5034195

#define PRZYCISK_1 3
#define PRZYCISK_2 4
#define PRZYCISK_3 5
#define PRZYCISK_4 6
#define PRZYCISK_5 7
#define PRZYCISK_6 8
#define PRZYCISK_7 9
#define PRZYCISK_8 10
#define PRZYCISK_9 11
#define PRZYCISK_10 12
#define PRZYCISK_11 13
#define PRZYCISK_12 14
#define PRZYCISK_13 15
#define PRZYCISK_14 16
#define PRZYCISK_15 17
#define PRZYCISK_16 18
#define PRZYCISK_17 19
#define PRZYCISK_18 20
#define PRZYCISK_19 21
#define PRZYCISK_20 48
#define PRZYCISK_21 49
#define PRZYCISK_22 50
#define PRZYCISK_23 51
#define PRZYCISK_24 52

#define CZAS_1 500
#define CZAS_2 500
#define CZAS_3 500
#define CZAS_4 500
#define CZAS_5 500
#define CZAS_6 500
#define CZAS_7 500
#define CZAS_8 500
#define CZAS_9 500
#define CZAS_10 500
#define CZAS_11 500
#define CZAS_12 500
#define CZAS_13 500
#define CZAS_14 500

#define DELAY 500
#define PROG_PRADOWY 100
#define RawOffset 512
#define ILOSC_PROBEK 10
#define DELAY_POMIAR_PRADU 1 

byte stan_rolety[14]={};//0 - stoj; 1 -jedz w dol; 2 - stoj; 3 - jedz w gore

RCSwitch mySwitch = RCSwitch();

Timer timer1;
Timer timer2;
Timer timer3;
Timer timer4;
Timer timer5;
Timer timer6;
Timer timer7;
Timer timer8;
Timer timer9;
Timer timer10;
Timer timer11;
Timer timer12;
Timer timer13;

void roleta(byte nr,byte komenda);
void zmienStan(byte nr);
int prad(byte analogIn);
int Max(int input[ILOSC_PROBEK]);
int zmierzPrad(int nr);

void setup() {
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2mySwitch
  Serial.begin(9600);

  for (byte i = 22; i < 48; i++)
  {
      pinMode(i,OUTPUT);
      digitalWrite(i,HIGH);
  }

  for (byte i = PRZYCISK_1; i <= PRZYCISK_14; i++)
  {
      pinMode(i,INPUT_PULLUP);
  }

  interrupts();
  timer1.begin(DELAY);
}

void loop(){
    if (mySwitch.available())
    {
      long int value = 0;
      value = mySwitch.getReceivedValue();
      
      if(value == KOD_1 && timer1.available() ){
        zmienStan(1);
        timer1.restart();
      }else{
        mySwitch.resetAvailable();
      }
    }
    if (!digitalRead(PRZYCISK_1) && timer1.available())
    {
        while(!digitalRead(PRZYCISK_1)){
            if(abs(zmierzPrad(A1)-512)<PROG_PRADOWY){
                roleta(1,1);
            }else{
                roleta(1,0);
            }
        }
        timer1.restart();
    }else if (!digitalRead(PRZYCISK_2) && timer1.available())
    {
        while(!digitalRead(PRZYCISK_2)){
            if(abs(zmierzPrad(A1)-512)<PROG_PRADOWY){
                roleta(1,2);
            }else{
                roleta(1,0);
            }
        }
        timer1.restart();
    }

if (stan_rolety[1]==0)
{
    roleta(1,0);
}else if (stan_rolety[1]==1)
{
    if(abs(zmierzPrad(A1)-512)<PROG_PRADOWY){
        roleta(1,1);
    }else{
        roleta(1,0);
    }
}else if (stan_rolety[1]==2)
{
    roleta(1,0);
}else if (stan_rolety[1]==3)
{
    if(abs(zmierzPrad(A1)-512)<PROG_PRADOWY){
        roleta(1,2);
    }else{
        roleta(1,0);
    }
}

}

void roleta(byte nr,byte komenda){
    switch (komenda)
    {
    case 0://stoj
        digitalWrite(20+2*nr,HIGH);
        digitalWrite(21+2*nr,HIGH);
        break;
    
    case 1://jedz w dol
        digitalWrite(20+2*nr,LOW);
        digitalWrite(21+2*nr,HIGH);
        break;

    case 2://jedz w gore
        digitalWrite(20+2*nr,HIGH);
        digitalWrite(21+2*nr,LOW);
        break;
    }
}

void zmienStan(byte nr){
    switch (stan_rolety[nr]){
        case 0:
            stan_rolety[nr]++;
            break;
        case 1:
            stan_rolety[nr]++;
            break;
        case 2:
            stan_rolety[nr]++;
            break;
        case 3:
            stan_rolety[nr]=0;
            break;
        }
}

// Funkcja do pomiary natężenia prądu:
int prad(byte analogIn){
    //int mVperAmp = 100; // use 100 for 20A Module and 66 for 30A Module
    int RawValue= 0;
    //double Voltage = 0;
    //double Amps = 0;

    RawValue = analogRead(analogIn);
    //Voltage = ((RawValue - RawOffset) / 1024.0) * 5000; // Gets you mV
    //Amps = Voltage / mVperAmp;
    //Serial.print("Amps: ");Serial.print(Amps);Serial.print("Voltage: ");Serial.print(Voltage);Serial.print("RawValue: ");Serial.print(RawValue);
    return RawValue;
}

//Funkcja do szukania największej próbki
int Max(int input[ILOSC_PROBEK]){
    int res=input[0];
        for (byte i = 1; i < ILOSC_PROBEK; i++)
        {
            if(input[i]>res){
                res=input[i];
            }
        }
    return res;
}
int zmierzPrad(int nr){
    int pomiar[ILOSC_PROBEK]={};
        for (byte j = 0; j < ILOSC_PROBEK; j++)
        {
            pomiar[j]=prad(nr);
            delay(DELAY_POMIAR_PRADU);
        }
        int wynik=Max(pomiar);
    return abs(wynik);
}