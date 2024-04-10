#include <Arduino.h>

#include <Servo.h>

// #define line_R A2
// #define line_I A5
#define front_R A5
int side_L = 0;
int front_L = 1;
int side_R = 4;

int servo_B = 2;
// int startPin = 10;

int motorL_pwm = 12;
int motorL_dir = 3;

int motorR_pwm = 11;
int motorR_dir = 13;

// int v_Line;
int v_Front;
int v_Side;

int proportionalUsed;
int counter;

Servo FLAG;

// ------------------------- Sensors Functions ------------------------- .

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

// ------------------------- Stop Functions ------------------------- .

void stopi(bool reverse)
{
  if (reverse == false)
  {
    digitalWrite(motorL_dir, HIGH);
    digitalWrite(motorL_pwm, LOW);

    digitalWrite(motorR_dir, HIGH);
    digitalWrite(motorR_pwm, LOW);
  }
  else if (reverse == true)
  {
    digitalWrite(motorL_dir, LOW);
    digitalWrite(motorL_pwm, LOW);

    digitalWrite(motorR_dir, LOW);
    digitalWrite(motorR_pwm, LOW);
  }
}

// ------------------------- Motors Functions ------------------------- .

void goForward(int powerL_f, int powerR_f, int workTime_f)
{
  digitalWrite(motorL_dir, LOW);
  analogWrite(motorL_pwm, powerL_f);

  digitalWrite(motorR_dir, LOW);
  analogWrite(motorR_pwm, powerR_f);

  delay(workTime_f);
  stopi(false);
}

void goBack(int powerL_b, int powerR_b, int workTime_b)
{
  digitalWrite(motorL_dir, HIGH);
  analogWrite(motorL_pwm, powerL_b);

  digitalWrite(motorR_dir, HIGH);
  analogWrite(motorR_pwm, powerR_b);

  delay(workTime_b);
  stopi(true);
}

void goFull_back()
{
  digitalWrite(motorL_dir, HIGH);
  digitalWrite(motorL_pwm, LOW);

  digitalWrite(motorR_dir, HIGH);
  analogWrite(motorR_pwm, 255);
}

void goFull_left()
{
  digitalWrite(motorL_dir, HIGH);
  analogWrite(motorL_pwm, 255);

  digitalWrite(motorR_dir, LOW);
  analogWrite(motorR_pwm, 255);
}

void goFull_right()
{
  digitalWrite(motorL_dir, HIGH);
  analogWrite(motorL_pwm, 255);

  digitalWrite(motorR_dir, HIGH);
  analogWrite(motorR_pwm, 255);
}

// ------------------------- Proportional Functions ------------------------- .

void goForward_proportional(int workTime_fp)
{
  for (int i = 5; i < 255; i = i + 50)
  {
    if (i < 200)
    {
      goForward(i, i, round(workTime_fp / 4));
    }
    else
    {
      goForward(i, i, workTime_fp);
    }
  }
}

void goRight(bool curve_r, int power_r, int workTime_r, int relation_r)
{
  if (curve_r == true)
  {
    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, power_r);

    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, round(power_r / relation_r));

    delay(workTime_r);
    stopi(false);
  }
  else if (curve_r == false)
  {
    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, power_r);

    digitalWrite(motorR_dir, HIGH);
    analogWrite(motorR_pwm, power_r);

    delay(workTime_r);
    stopi(false);
  }
}

void goLeft(bool curve_l, int power_l, int workTime_l, int relation_l)
{
  if (curve_l == true)
  {
    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, power_l);

    digitalWrite(motorL_dir, LOW);
    analogWrite(motorL_pwm, round(power_l / relation_l));

    delay(workTime_l);
    stopi(false);
  }
  else if (curve_l == false)
  {
    digitalWrite(motorL_dir, HIGH);
    analogWrite(motorL_pwm, power_l);

    digitalWrite(motorR_dir, LOW);
    analogWrite(motorR_pwm, power_l);

    delay(workTime_l);
    stopi(false);
  }
}

void setup()
{
  Serial.begin(9600);
  FLAG.attach(servo_B);

  // pinMode(startPin, INPUT);
  //   pinMode(line_L, INPUT);
  //  pinMode(line_R, INPUT);
  pinMode(side_L, INPUT);
  pinMode(side_R, INPUT);
  pinMode(front_L, INPUT);
  pinMode(front_R, INPUT);

  pinMode(motorL_pwm, OUTPUT);
  pinMode(motorL_dir, OUTPUT);
  pinMode(motorR_pwm, OUTPUT);
  pinMode(motorR_dir, OUTPUT);
  pinMode(servo_B, OUTPUT);

  // v_Line = 0;
  v_Front = 0;
  v_Side = 0;

  proportionalUsed = 0;
  counter = 0;

  FLAG.write(80);

  stopi(true);
}

void loop()
{
start:
  // if (digitalRead(startPin) == HIGH) {
  // FLAG.write(190);

  // ---------- Line ----------
  // if (analogRead(line_I) < 600)
  // {
  //   Serial.println("Linea - izq");
  // goFull_back();
  // delay(30);
  // goFull_right();
  // delay(20);
  // stopi(true);
  // goto start;
  // }

  // ---------- Front ----------

  v_Front = frontSensors();
  if (v_Front == 3)
  {
    if (proportionalUsed == 0)
    {
      Serial.println("Enfrente - ambos");
      goFull_left();
      proportionalUsed = 1;
      goto start;
    }
    else if (proportionalUsed == 1)
    {
      goFull_left();
      goto start;
    }
  }
  else if (v_Front == 1)
  {
    Serial.println("Enfrente - izquierda");
    goLeft(false, 70, 10, 1);
    proportionalUsed = 0;
    goto start;
  }
  else if (v_Front == 2)
  {
    Serial.println("Enfrente - derecha");
    goFull_right();
    proportionalUsed = 0;
    goto start;
  }

  // ---------- Side ----------

  v_Side = sideSensors();

  if (v_Side == 1)
  {
    Serial.println("Lado - izquierda");
    goLeft(false, 120, 10, 1);
    proportionalUsed = 0;
    goto start;
  }
  else if (v_Side == 2)
  {
    Serial.println("Lado - derecha");
    goFull_right();
    proportionalUsed = 0;
    goto start;
  }
  else
  {
    proportionalUsed = 0;
    if (counter == 20000)
    {
      goFull_left();
      delay(7);
      stopi(true);
      counter = 0;
    }
    else
    {
      stopi(true);
      counter++;
      Serial.println(counter);
    }
    goto start;
  }
  // } else {
  //   Serial.println("Apagado");
  //   stopi(false);
  //   proportionalUsed = 0;
  //   FLAG.write(90);
  // }

} // <<<---loop end
