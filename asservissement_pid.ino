volatile int count = 0;
int PWM, consigne, value1, value2, dvalue;
int n = 1;
float dt, N = 0, Nmr, potar;
float erreur, cmd;
double kp = 1.0;

long t1, t2;
long t;

void setup() {
  Serial.begin(921600);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  t1 = millis();
}

void loop() {
  potar = analogRead(0);
  consigne = (potar / 675) * 300;
  PWM = (potar / 675) * 255;

  // Écrire HIGH sur un pin et LOW sur l'autre pour contrôler le sens du moteur
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  analogWrite(5, PWM);

  value1 = digitalRead(3);
  dvalue = value2 - value1;
  value2 = value1;
  if (dvalue != 0) {  // Appeler mesure() à chaque changement de valeur
    mesure();
  }

  t = millis();
  if (t >= (n * 100)) {
    n = n + 1;
    asservissement_PID();
  }
}

void mesure() {
  t2 = millis();
  dt = t2 - t1;
  N = (1 / dt);
  Nmr = N / 12;
  t1 = t2;
  count = count + 1;
  Serial.println(Nmr);
}

double somme_erreur = 0;
double erreur_precedente = 0;
double ki = 0.1;
double kd = 0.01;

void controlerMoteur(double vitesse) {
  analogWrite(5, vitesse);  // Contrôler la vitesse du moteur en utilisant PWM sur le pin 5
}

void asservissement_PID() {
    double erreur = consigne - Nmr;
    somme_erreur += erreur;
    double delta_erreur = erreur - erreur_precedente;
    erreur_precedente = erreur;
    double cmd = kp * erreur + ki * somme_erreur + kd * delta_erreur;
    if (cmd < 0) {
        cmd = 0;
    } else if (cmd > 255) {
        cmd = 255;
    }
    controlerMoteur(cmd);
}
