#include <Wire.h>

#define ArrieD 4   //pin moteur droit : arriere
#define AvantD 7    //pin moteur droit : avant
#define ArrieG 8    //pin moteur gauche : arriere
#define AvantG 11    //pin moteur gauche : avant
#define pwmArrieD 5   //pin hacheur moteur droit : arriere
#define pwmAvantD 6    //pin hacheur moteur droit : avant
#define pwmArrieG 9    //pin hacheur moteur gauche : arriere
#define pwmAvantG 10    //pin hacheur moteur gauche : avant
#define porti2c 0x12
int dataRecevied1(127);
int vitesseD(0);
int last1(127);
int dataRecevied2(127);
int vitesseG(0);
int last2(127);
int moteurD(0);
int moteurG(0);
boolean data(false);
boolean done(true);

void setup() {
  pinMode(ArrieD, OUTPUT);
  pinMode(AvantD, OUTPUT);
  pinMode(ArrieG, OUTPUT);
  pinMode(AvantG, OUTPUT);
  pinMode(pwmArrieD, OUTPUT);
  pinMode(pwmAvantD, OUTPUT);
  pinMode(pwmArrieG, OUTPUT);
  pinMode(pwmAvantG, OUTPUT);
  Wire.begin(porti2c);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendEvent);
  Serial.begin(9600);       
}

void loop() {
  delay(1);
  if(done==false && (last2 != dataRecevied2 || last1 != dataRecevied1))
  {
    done=true;
    last1 = dataRecevied1;
    last2 = dataRecevied2;
    vitesseD = (dataRecevied1 - 127)*2;
    vitesseG = (dataRecevied2 - 127)*2;

    if(vitesseD >= 0)
    {
      digitalWrite(ArrieD, LOW);
      digitalWrite(AvantD, HIGH);
      moteurD = pwmAvantD;
    }
    else
    {
      digitalWrite(AvantD, LOW);
      digitalWrite(ArrieD, HIGH);
      moteurD = pwmArrieD;
      vitesseD -= vitesseD*2;
    }

    if(vitesseG >= 0)
    {
      digitalWrite(ArrieG, LOW);
      digitalWrite(AvantG, HIGH);
      moteurG = pwmAvantG;
    }
    else
    {
      digitalWrite(AvantG, LOW);
      digitalWrite(ArrieG, HIGH);
      moteurG = pwmArrieG;
      vitesseG -= vitesseG*2;
    }


    for(int i(0),o(0),count(0); ((i<vitesseD || o<vitesseG) && count != 11); count += 1)
    {
      delay(9);
      if(i < vitesseD)
      {
        i += vitesseD / 10;
      }
      else
      {
        i = vitesseD;
      }
      if(o < vitesseG)
      {
        o += vitesseG / 10;
      }
      else
      {
        o = vitesseG;
      }
      analogWrite(moteurD, i);
      analogWrite(moteurG, o);
    }
    Wire.write(0);
  }
}


void receiveEvent(int howMany)
{
  if(data==false)
  {
    dataRecevied1 = Wire.read();
    data = true;
  }
  else
  {
    dataRecevied2 = Wire.read();
    data = false;
    done = false;
  }
}

void sendEvent()
{
  if(data == true)
  {
    Wire.write(dataRecevied1);
  }
  else
  {
    Wire.write(dataRecevied2);
  }
}
