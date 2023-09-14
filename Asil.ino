#define line_L A2
#define line_R A4

int side_L = 0;
int front_L = 1;
int front_R = 2;
int side_R = 4;

int motorL_pwm = 3;
int motorL_dir = 12;
int motorR_pwm = 11;
int motorR_dir = 13;

int v_Line;
int v_Front;
int v_Side;

void setup()
{
  Serial.begin(9600);

  pinMode(line_L, INPUT);
  pinMode(line_R, INPUT);
  pinMode(side_L, INPUT);
  pinMode(side_R, INPUT);
  pinMode(front_L, INPUT);
  pinMode(front_R, INPUT);

  pinMode(motorL_pwm, OUTPUT);
  pinMode(motorL_dir, OUTPUT);
  pinMode(motorR_pwm, OUTPUT);
  pinMode(motorR_dir, OUTPUT);

  v_Line = 0;
  v_Front = 0;
  v_Side = 0;

  delay(2000);
  goBack(200, 200, 1500);
  delay(1000);
  goForward(200, 200, 1500);
  delay(1000);
}

void loop()
{
start:

  // ---------- Line ----------

  v_Line = lineSensors();

  if (v_Line == 3)
  {
    Serial.println("Linea - ambos");
    goto start;
  }
  else if (v_Line == 1)
  {
    Serial.println("Linea - izquierda");
    goto start;
  }
  else if (v_Line == 2)
  {
    Serial.println("Linea - derecha");
    goto start;
  }

  // ---------- Front ----------

  v_Front = frontSensors();
  if (v_Front == 3)
  {
    Serial.println("Enfrente - ambos");
    goto start;
  }
  else if (v_Front == 1)
  {
    Serial.println("Enfrente - izquierda");
    goto start;
  }
  else if (v_Front == 2)
  {
    Serial.println("Enfrente - derecha");
    goto start;
  }

  // ---------- Side ----------

  v_Side = sideSensors();
  if (v_Side == 1)
  {
    Serial.println("Lado - izquierda");
    goto start;
  }
  else if (v_Side == 2)
  {
    Serial.println("Lado - derecha");
    goto start;
  }
  else
  {
    Serial.println("Ninguno");
    goto start;
  }
} // <---loop end

// ----------------- Sensors Functions ----------------- .

int lineSensors()
{
  int lineL = digitalRead(line_L);
  int lineR = digitalRead(line_R) * 2;

  int addition_Line = lineL + lineR;

  return addition_Line;
}

int frontSensors()
{
  int frontL = digitalRead(front_L);
  int frontR = digitalRead(front_R) * 2;

  int addition_Front = frontL + frontR;

  return addition_Front;
}

int sideSensors()
{
  int sideL = digitalRead(side_L);
  int sideR = digitalRead(side_R) * 2;

  int addition_Side = sideL + sideR;

  return addition_Side;
}

// ----------------- Stop Functions ----------------- .

void stopi(bool pause, int miliseconds)
{
  digitalWrite(motorL_1, LOW);
  digitalWrite(motorL_2, LOW);

  digitalWrite(motorL_1, LOW);
  digitalWrite(motorR_2, LOW);

  if (pause == true)
  {
    delay(miliseconds);
  }
}

// ----------------- Motors Functions ----------------- .

void goForward(int powerL_f, int powerR_f, int workTime_f)
{
  digitalWrite(motorL_dir, LOW);
  analogWrite(motorL_pwm, powerL_f);

  digitalWrite(motorR_dir, LOW);
  analogWrite(motorR_pwm, powerR_f);

  delay(workTime_f);
  stopi(false, 0);
}

void goBack(int powerL_b, int powerR_b, int workTime_b)
{
  digitalWrite(motorL_dir, HIGH);
  analogWrite(motorL_pwm, powerL_b);

  digitalWrite(motorR_dir, HIGH);
  analogWrite(motorR_pwm, powerR_b);

  delay(workTime_b);
  stopi(false, 0);
}

void goRight(bool curve_r, int power_r, int workTime_r)
{
  if (curve_r == true)
  {
    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, power_r);

    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, round(power_r / 2));

    delay(workTime_r);
    stopi(false, 0);
  }
  else if (curve_r == false)
  {
    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, power_r);

    digitalWrite(motorR_dir, HIGH);
    analogWrite(motorR_pwm, power_r);

    delay(workTime_r);
    stopi(false, 0);
  }
}

void goLeft(bool curve_l, int power_l, int workTime_l)
{
  if (curve_l == true)
  {
    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, power_l);

    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, round(power_l / 2));

    delay(workTime_l);
    stopi(false, 0);
  }
  else if (curve_l == false)
  {
    digitalWrite(motorL_dir, HIGH);
    analogWrite(motorL_pwm, power_l);

    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, power_l);

    delay(workTime_l);
    stopi(false, 0);
  }
}

// ----------------- Test Functions ----------------- .

void motorsTest1(int workTimes, int pause_t)
{
  Serial.println("Test: Adelante y Atras");
  Serial.println("adelante");
  goForward(200, 200, workTimes);
  stopi(true, pause_t);
  Serial.println("atras");
  goBack(200, 200, workTimes);
  stopi(true, pause_t);
}

void motorsTest2(int workTimes, int pause_t)
{
  Serial.println("Test: giro total Derecha e Izquierda");
  Serial.println("giro derecha");
  goRight(false, 150, workTimes);
  stopi(true, pause_t);
  Serial.println("giro izquierda");
  goLeft(false, 150, workTimes);
  stopi(true, pause_t);
}

void motorsTest3(int workTimes, int pause_t)
{
  Serial.println("Test: curva a la Derecha e Izquierda");
  Serial.println("curva a la derecha");
  goRight(true, 150, workTimes);
  stopi(true, pause_t);
  Serial.println("curva a la izquierda");
  goLeft(true, 150, workTimes);
  stopi(true, pause_t);
}
