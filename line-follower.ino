// Motor Control Pins
const int IN1 = 3;  // Right motor forward
const int IN2 = 5;  // Right motor backward
const int IN3 = 6;  // Left motor forward
const int IN4 = 9;  // Left motor backward

// IR Sensor Pins
const int IR1_LEFT  = 7;
const int IR2_RIGHT = 4;
const int IR3_MID   = 12;

// Base forward speed
const int baseSpeed = 150;
float lastError = 0;


int Kp = 8;

void setup(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IR1_LEFT, INPUT);
  pinMode(IR2_RIGHT, INPUT);
  pinMode(IR3_MID, INPUT);
}

int calculateError(int L, int R,int M){
  if(L == 1 && R == 0 && M == 1){
    lastError = -2;
    return -2; //slight left
  }
  if(L == 1 && R == 0 && M == 0){
    lastError = -4; 
    return -4;
    //far left
  }
  if(L == 0 && R == 1 && M == 0){
    lastError = 4; 
    return 4;
    //far right
  }
  if(L == 0 && R == 1 && M == 1){
    lastError = 2;
    return 2;
     //slight right
  }
  if(L == 0 && R == 0 && M == 1){
    lastError = 0; 
    return 0;
    //straight
  }
  if(L == 0 && R == 0 && M == 0){
    return lastError; //continue with previous error
  }
  if(L == 1 && R == 1 && M == 1){
    return 10; //stop motors
  }
  if(L == 1 && R == 1 && M == 0){
    return lastError; //continue with previous error
  }

  return 0;
}

void driveMotors(int left, int right){
  left = constrain(left, 0, 255);
  right = constrain(right, 0, 255);

  analogWrite(IN2, 0);
  analogWrite(IN1, left);
  analogWrite(IN4, 0);
  analogWrite(IN3, right);
}

void stopMotors(){
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

void loop(){
  int leftTriggered = digitalRead(IR1_LEFT);
  int rightTriggered = digitalRead(IR2_RIGHT);

  int middleTriggered = digitalRead(IR3_MID);

  int error = calculateError(leftTriggered, rightTriggered, middleTriggered);

  if(error == 10){
    stopMotors();
    return;
  }

  int correction = abs((Kp * error));

  int leftSpeed;
  int rightSpeed;

  if(error == 0){
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;
  }
  else{
    int outerBase = baseSpeed - (baseSpeed * 0.2);  // outer wheel: 40% reduction
    int innerBase = baseSpeed - (baseSpeed * 0.9);  // inner wheel: 90% reduction

    if(error < 0){
      // turning left: right wheel is inner, left wheel is outer
      leftSpeed  = outerBase + correction;
      rightSpeed = innerBase - correction;
    }
    else{
      // turning right: left wheel is inner, right wheel is outer
      leftSpeed  = innerBase - correction;
      rightSpeed = outerBase + correction;
    }
  }

  driveMotors(leftSpeed, rightSpeed);
}
