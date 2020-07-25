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
  timer2.begin(DELAY);
  timer3.begin(DELAY);
  timer4.begin(DELAY);
  timer5.begin(DELAY);
  timer6.begin(DELAY);
  timer7.begin(DELAY);
  timer8.begin(DELAY);
  timer9.begin(DELAY);
  timer10.begin(DELAY);
  timer11.begin(DELAY);
  timer12.begin(DELAY);
}

void loop(){
  //-----------------------------------------------------------------------------
  // sprawdzaj sygnały z pilota
    if (mySwitch.available()) //wejdz do warunku jezeli jest sygnał z pilota
    {
      long int value = 0;
      value = mySwitch.getReceivedValue();  //odbierz i zapisz sygnał
      
      if(value == KOD_1 && timer1.available() ){            //PRZYCISK 1
        zmienStan(1);
        timer1.restart();
        Serial.println("odebrano KOD 1");
      }else if(value == KOD_2 && timer2.available() ){      //PRZYCISK 2
        zmienStan(2);
        timer2.restart();
        Serial.println("odebrano KOD 2");
      }else if(value == KOD_3 && timer3.available() ){      //PRZYCISK 3
        zmienStan(3);
        timer3.restart();
        Serial.println("odebrano KOD 3");
      }else if(value == KOD_4 && timer4.available() ){      //PRZYCISK 4
        zmienStan(4);
        timer4.restart();
        Serial.println("odebrano KOD 4");
      }else if(value == KOD_5 && timer5.available() ){      //PRZYCISK 5
        zmienStan(5);
        timer5.restart();
        Serial.println("odebrano KOD 5");
      }else if(value == KOD_6 && timer6.available() ){      //PRZYCISK 6
        zmienStan(6);
        timer6.restart();
        Serial.println("odebrano KOD 6");
      }else if(value == KOD_7 && timer7.available() ){      //PRZYCISK 7
        zmienStan(7);
        timer7.restart();
        Serial.println("odebrano KOD 7");
      }else if(value == KOD_8 && timer8.available() ){      //PRZYCISK 8
        zmienStan(8);
        timer8.restart();
        Serial.println("odebrano KOD 8");
      }else if(value == KOD_9 && timer9.available() ){      //PRZYCISK 9
        zmienStan(9);
        timer9.restart();
        Serial.println("odebrano KOD 9");
      }else if(value == KOD_10 && timer10.available() ){      //PRZYCISK 10
        zmienStan(10);
        timer10.restart();
        Serial.println("odebrano KOD 10");
      }else if(value == KOD_11 && timer11.available() ){      //PRZYCISK 11
        zmienStan(11);
        timer11.restart();
        Serial.println("odebrano KOD 11");
      }else if(value == KOD_12 && timer12.available() ){      //PRZYCISK 12
        zmienStan(12);
        timer12.restart();
        Serial.println("odebrano KOD 12");
      }else{
        mySwitch.resetAvailable();
        Serial.println("odebrano kod nieznany");
      }
      Serial.println("Stany rolet:");
      for(int i=1;i<14;i++){
        Serial.println(stan_rolety[i]);
      }
    }
    //-----------------------------------------------------------------------------
    // sprawdzaj sygnały z przycisków
    if (!digitalRead(PRZYCISK_1) && timer1.available())   //PRZYCISKI DO ROLETY 1
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
    //-----------------------------------
    if (!digitalRead(PRZYCISK_3) && timer2.available())   //PRZYCISKI DO ROLETY 2
    {
        while(!digitalRead(PRZYCISK_3)){
            if(abs(zmierzPrad(A2)-512)<PROG_PRADOWY){
                roleta(2,1);
            }else{
                roleta(2,0);
            }
        }
        timer2.restart();
    }else if (!digitalRead(PRZYCISK_4) && timer2.available())
    {
        while(!digitalRead(PRZYCISK_4)){
            if(abs(zmierzPrad(A2)-512)<PROG_PRADOWY){
                roleta(2,2);
            }else{
                roleta(2,0);
            }
        }
        timer2.restart();
    }
  //-----------------------------------
    if (!digitalRead(PRZYCISK_5) && timer3.available())   //PRZYCISKI DO ROLETY 3
    {
        while(!digitalRead(PRZYCISK_5)){
            if(abs(zmierzPrad(A3)-512)<PROG_PRADOWY){
                roleta(3,1);
            }else{
                roleta(3,0);
            }
        }
        timer3.restart();
    }else if (!digitalRead(PRZYCISK_6) && timer3.available())
    {
        while(!digitalRead(PRZYCISK_6)){
            if(abs(zmierzPrad(A3)-512)<PROG_PRADOWY){
                roleta(3,2);
            }else{
                roleta(3,0);
            }
        }
        timer3.restart();
    }
  //-----------------------------------
    if (!digitalRead(PRZYCISK_7) && timer4.available())   //PRZYCISKI DO ROLETY 4
    {
        while(!digitalRead(PRZYCISK_7)){
            if(abs(zmierzPrad(A4)-512)<PROG_PRADOWY){
                roleta(4,1);
            }else{
                roleta(4,0);
            }
        }
        timer4.restart();
    }else if (!digitalRead(PRZYCISK_8) && timer4.available())
    {
        while(!digitalRead(PRZYCISK_8)){
            if(abs(zmierzPrad(A4)-512)<PROG_PRADOWY){
                roleta(4,2);
            }else{
                roleta(4,0);
            }
        }
        timer4.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_9) && timer5.available())   //PRZYCISKI DO ROLETY 5
    {
        while(!digitalRead(PRZYCISK_9)){
            if(abs(zmierzPrad(A5)-512)<PROG_PRADOWY){
                roleta(5,1);
            }else{
                roleta(5,0);
            }
        }
        timer5.restart();
    }else if (!digitalRead(PRZYCISK_10) && timer5.available())
    {
        while(!digitalRead(PRZYCISK_10)){
            if(abs(zmierzPrad(A5)-512)<PROG_PRADOWY){
                roleta(5,2);
            }else{
                roleta(5,0);
            }
        }
        timer5.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_11) && timer6.available())   //PRZYCISKI DO ROLETY 6
    {
        while(!digitalRead(PRZYCISK_11)){
            if(abs(zmierzPrad(A6)-512)<PROG_PRADOWY){
                roleta(6,1);
            }else{
                roleta(6,0);
            }
        }
        timer6.restart();
    }else if (!digitalRead(PRZYCISK_12) && timer6.available())
    {
        while(!digitalRead(PRZYCISK_12)){
            if(abs(zmierzPrad(A6)-512)<PROG_PRADOWY){
                roleta(6,2);
            }else{
                roleta(6,0);
            }
        }
        timer6.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_13) && timer7.available())   //PRZYCISKI DO ROLETY 7
    {
        while(!digitalRead(PRZYCISK_13)){
            if(abs(zmierzPrad(A5)-512)<PROG_PRADOWY){
                roleta(7,1);
            }else{
                roleta(7,0);
            }
        }
        timer7.restart();
    }else if (!digitalRead(PRZYCISK_14) && timer7.available())
    {
        while(!digitalRead(PRZYCISK_14)){
            if(abs(zmierzPrad(A7)-512)<PROG_PRADOWY){
                roleta(7,2);
            }else{
                roleta(7,0);
            }
        }
        timer7.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_15) && timer8.available())   //PRZYCISKI DO ROLETY 8
    {
        while(!digitalRead(PRZYCISK_15)){
            if(abs(zmierzPrad(A8)-512)<PROG_PRADOWY){
                roleta(8,1);
            }else{
                roleta(8,0);
            }
        }
        timer8.restart();
    }else if (!digitalRead(PRZYCISK_16) && timer8.available())
    {
        while(!digitalRead(PRZYCISK_16)){
            if(abs(zmierzPrad(A8)-512)<PROG_PRADOWY){
                roleta(8,2);
            }else{
                roleta(8,0);
            }
        }
        timer8.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_17) && timer9.available())   //PRZYCISKI DO ROLETY 9
    {
        while(!digitalRead(PRZYCISK_17)){
            if(abs(zmierzPrad(A9)-512)<PROG_PRADOWY){
                roleta(9,1);
            }else{
                roleta(9,0);
            }
        }
        timer9.restart();
    }else if (!digitalRead(PRZYCISK_18) && timer9.available())
    {
        while(!digitalRead(PRZYCISK_18)){
            if(abs(zmierzPrad(A9)-512)<PROG_PRADOWY){
                roleta(9,2);
            }else{
                roleta(9,0);
            }
        }
        timer9.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_19) && timer10.available())   //PRZYCISKI DO ROLETY 10
    {
        while(!digitalRead(PRZYCISK_19)){
            if(abs(zmierzPrad(A10)-512)<PROG_PRADOWY){
                roleta(10,1);
            }else{
                roleta(10,0);
            }
        }
        timer10.restart();
    }else if (!digitalRead(PRZYCISK_20) && timer10.available())
    {
        while(!digitalRead(PRZYCISK_20)){
            if(abs(zmierzPrad(A10)-512)<PROG_PRADOWY){
                roleta(10,2);
            }else{
                roleta(10,0);
            }
        }
        timer10.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_21) && timer11.available())   //PRZYCISKI DO ROLETY 11
    {
        while(!digitalRead(PRZYCISK_21)){
            if(abs(zmierzPrad(A11)-512)<PROG_PRADOWY){
                roleta(11,1);
            }else{
                roleta(11,0);
            }
        }
        timer11.restart();
    }else if (!digitalRead(PRZYCISK_22) && timer11.available())
    {
        while(!digitalRead(PRZYCISK_22)){
            if(abs(zmierzPrad(A11)-512)<PROG_PRADOWY){
                roleta(11,2);
            }else{
                roleta(11,0);
            }
        }
        timer11.restart();
    }
    //-----------------------------------
    if (!digitalRead(PRZYCISK_23) && timer12.available())   //PRZYCISKI DO ROLETY 12
    {
        while(!digitalRead(PRZYCISK_23)){
            if(abs(zmierzPrad(A12)-512)<PROG_PRADOWY){
                roleta(12,1);
            }else{
                roleta(12,0);
            }
        }
        timer12.restart();
    }else if (!digitalRead(PRZYCISK_24) && timer12.available())
    {
        while(!digitalRead(PRZYCISK_24)){
            if(abs(zmierzPrad(A12)-512)<PROG_PRADOWY){
                roleta(12,2);
            }else{
                roleta(12,0);
            }
        }
        timer12.restart();
    }
  //-----------------------------------------------------------------------------
  // steruj roletami wg sygnałów z pilota

//roleta 1
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
//--------------------------------------------
//roleta 2
if (stan_rolety[2]==0)
{
    roleta(2,0);
}else if (stan_rolety[2]==1)
{
    if(abs(zmierzPrad(A2)-512)<PROG_PRADOWY){
        roleta(2,1);
    }else{
        roleta(2,0);
    }
}else if (stan_rolety[2]==2)
{
    roleta(2,0);
}else if (stan_rolety[2]==3)
{
    if(abs(zmierzPrad(A2)-512)<PROG_PRADOWY){
        roleta(2,2);
    }else{
        roleta(2,0);
    }
}
//--------------------------------------------
//roleta 3
if (stan_rolety[3]==0)
{
    roleta(3,0);
}else if (stan_rolety[3]==1)
{
    if(abs(zmierzPrad(A3)-512)<PROG_PRADOWY){
        roleta(3,1);
    }else{
        roleta(3,0);
    }
}else if (stan_rolety[3]==2)
{
    roleta(3,0);
}else if (stan_rolety[3]==3)
{
    if(abs(zmierzPrad(A3)-512)<PROG_PRADOWY){
        roleta(3,2);
    }else{
        roleta(3,0);
    }
}
//--------------------------------------------
//roleta 4
if (stan_rolety[4]==0)
{
    roleta(4,0);
}else if (stan_rolety[4]==1)
{
    if(abs(zmierzPrad(A4)-512)<PROG_PRADOWY){
        roleta(4,1);
    }else{
        roleta(4,0);
    }
}else if (stan_rolety[4]==2)
{
    roleta(4,0);
}else if (stan_rolety[4]==3)
{
    if(abs(zmierzPrad(A4)-512)<PROG_PRADOWY){
        roleta(4,2);
    }else{
        roleta(4,0);
    }
}
//--------------------------------------------
//roleta 5
if (stan_rolety[5]==0)
{
    roleta(5,0);
}else if (stan_rolety[5]==1)
{
    if(abs(zmierzPrad(A5)-512)<PROG_PRADOWY){
        roleta(5,1);
    }else{
        roleta(5,0);
    }
}else if (stan_rolety[5]==2)
{
    roleta(5,0);
}else if (stan_rolety[5]==3)
{
    if(abs(zmierzPrad(A5)-512)<PROG_PRADOWY){
        roleta(5,2);
    }else{
        roleta(5,0);
    }
}
//--------------------------------------------
//roleta 6
if (stan_rolety[6]==0)
{
    roleta(6,0);
}else if (stan_rolety[6]==1)
{
    if(abs(zmierzPrad(A6)-512)<PROG_PRADOWY){
        roleta(6,1);
    }else{
        roleta(6,0);
    }
}else if (stan_rolety[6]==2)
{
    roleta(6,0);
}else if (stan_rolety[6]==3)
{
    if(abs(zmierzPrad(A6)-512)<PROG_PRADOWY){
        roleta(6,2);
    }else{
        roleta(6,0);
    }
}
//--------------------------------------------
//roleta 7
if (stan_rolety[7]==0)
{
    roleta(7,0);
}else if (stan_rolety[7]==1)
{
    if(abs(zmierzPrad(A7)-512)<PROG_PRADOWY){
        roleta(7,1);
    }else{
        roleta(7,0);
    }
}else if (stan_rolety[7]==2)
{
    roleta(7,0);
}else if (stan_rolety[7]==3)
{
    if(abs(zmierzPrad(A7)-512)<PROG_PRADOWY){
        roleta(7,2);
    }else{
        roleta(7,0);
    }
}
//--------------------------------------------
//roleta 8
if (stan_rolety[8]==0)
{
    roleta(8,0);
}else if (stan_rolety[8]==1)
{
    if(abs(zmierzPrad(A8)-512)<PROG_PRADOWY){
        roleta(8,1);
    }else{
        roleta(8,0);
    }
}else if (stan_rolety[8]==2)
{
    roleta(8,0);
}else if (stan_rolety[8]==3)
{
    if(abs(zmierzPrad(A8)-512)<PROG_PRADOWY){
        roleta(8,2);
    }else{
        roleta(8,0);
    }
}
//--------------------------------------------
//roleta 9
if (stan_rolety[9]==0)
{
    roleta(9,0);
}else if (stan_rolety[9]==1)
{
    if(abs(zmierzPrad(A9)-512)<PROG_PRADOWY){
        roleta(9,1);
    }else{
        roleta(9,0);
    }
}else if (stan_rolety[9]==2)
{
    roleta(9,0);
}else if (stan_rolety[9]==3)
{
    if(abs(zmierzPrad(A9)-512)<PROG_PRADOWY){
        roleta(9,2);
    }else{
        roleta(9,0);
    }
}
//--------------------------------------------
//roleta 10
if (stan_rolety[10]==0)
{
    roleta(10,0);
}else if (stan_rolety[10]==1)
{
    if(abs(zmierzPrad(A10)-512)<PROG_PRADOWY){
        roleta(10,1);
    }else{
        roleta(10,0);
    }
}else if (stan_rolety[10]==2)
{
    roleta(10,0);
}else if (stan_rolety[10]==3)
{
    if(abs(zmierzPrad(A10)-512)<PROG_PRADOWY){
        roleta(10,2);
    }else{
        roleta(10,0);
    }
}
//--------------------------------------------
//roleta 11
if (stan_rolety[11]==0)
{
    roleta(11,0);
}else if (stan_rolety[11]==1)
{
    if(abs(zmierzPrad(A11)-512)<PROG_PRADOWY){
        roleta(11,1);
    }else{
        roleta(11,0);
    }
}else if (stan_rolety[11]==2)
{
    roleta(11,0);
}else if (stan_rolety[11]==3)
{
    if(abs(zmierzPrad(A11)-512)<PROG_PRADOWY){
        roleta(11,2);
    }else{
        roleta(11,0);
    }
}
//--------------------------------------------
//roleta 12
if (stan_rolety[12]==0)
{
    roleta(12,0);
}else if (stan_rolety[12]==1)
{
    if(abs(zmierzPrad(A12)-512)<PROG_PRADOWY){
        roleta(12,1);
    }else{
        roleta(12,0);
    }
}else if (stan_rolety[12]==2)
{
    roleta(12,0);
}else if (stan_rolety[12]==3)
{
    if(abs(zmierzPrad(A12)-512)<PROG_PRADOWY){
        roleta(12,2);
    }else{
        roleta(12,0);
    }
}
//--------------------------------------------
//roleta 13
if (stan_rolety[13]==0)
{
    roleta(13,0);
}else if (stan_rolety[13]==1)
{
    if(abs(zmierzPrad(A13)-512)<PROG_PRADOWY){
        roleta(13,1);
    }else{
        roleta(13,0);
    }
}else if (stan_rolety[13]==2)
{
    roleta(13,0);
}else if (stan_rolety[13]==3)
{
    if(abs(zmierzPrad(A13)-512)<PROG_PRADOWY){
        roleta(13,2);
    }else{
        roleta(13,0);
    }
}
//-----------------------------------------------------------------------------
Serial.print("...");
delay(10);
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
