volatile int count=0;
int PWM,consigne,value1,value2,dvalue;
int n=1;
float dt,N=0,Nmr,potar;
float erreur,cmd,kp=1;
long t1,t2;
long t;

void setup()
{
  Serial.begin(921600);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  t1=millis();
}

void loop()
{
  potar=analogRead(0);
  consigne=(potar/675)*300;
  PWM=(potar/675)*255;
  digitalWrite(8, HIGH);
  digitalWrite(8, LOW);
  analogWrite(5, PWM);
  
  value1=digitalRead(3);
  dvalue=value2-value1;
  value2=value1;
  if (dvalue>1)
  {
    count=count+1;
    if(count>=16)
    {
      mesure();
    }
  }
  t=millis();
  if (t>=(n*100))
  {
    n=n+1;
    asservissement_P();
  }
}

int mesure()
{
  t2=millis();
  dt=t2-t1;
  N=(1/dt);
  Nmr=N/12;
  t1=t2;
  count=0;
  Serial.println(Nmr);
  return N;
}

void asservissement_P()
{
  erreur=consigne-Nmr;
  cmd=kp*erreur;
  if(cmd<0){cmd=0;}
  else if(cmd>255){cmd=255;}
}