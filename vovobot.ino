
// set to 1 to print to serial monitor, 0 to disable
#define DEBUG 1

// set to 1 to simulate movement (no signal for motors)
#define SIMULATION 0

// signal input of Dc motor driven plate
const int leftMotorPin1 = 4; // digital (yellow)
const int leftMotorPin2 = 5; // digital pwn (red)
const int rightMotorPin1 = 6; // digital pwm (green)
const int rightMotorPin2 = 7; // digital (blue)

// signal input&output for ultrasonic
const int ultrasonicEchoPin = 2; // digital
const int ultrasonicTrigPin = 3; // digital

boolean running = false;
int distance = 0;

void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  
  // set the ultrasonic pins
  pinMode(ultrasonicEchoPin, INPUT);
  pinMode(ultrasonicTrigPin, OUTPUT);
  
  // set the motor pins to be output pins
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);
}

void loop() {
//  manualMode();
  autoMode();
}

void forward() {
  if (DEBUG) {
    Serial.println("Forward");
  }
  
  running = true;
  
  if (!SIMULATION) {
    // left motor forward
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    // right motor forward
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);
  }
}

void backward() {
  if (DEBUG) {
    Serial.println("Backward");
  }
  
  running = true;
  
  if (!SIMULATION) {
    // left motor backward
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    // right motor backward
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);
    
    delay(1000);
  }  
}

void left() {
  if (DEBUG) {
    Serial.println("Left");
  }
  
  running = true;
  
  if (!SIMULATION) {
    // left motor backward
    digitalWrite(leftMotorPin1, LOW);
    digitalWrite(leftMotorPin2, HIGH);
    // right motor forward
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, LOW);  
    
    delay(1500);
    
    stop();
  }
}

void right() {
  if (DEBUG) {
    Serial.println("Right");
  }
  
  running = true;
  
  if (!SIMULATION) {
    // left motor forward
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, LOW);
    // right motor backward
    digitalWrite(rightMotorPin1, LOW);
    digitalWrite(rightMotorPin2, HIGH);  
    
    delay(1500);
    
    stop();
  }
}

void stop() {
  if (DEBUG) {
    Serial.println("Stop");
  }
  
  running = false;
  
  if (!SIMULATION) {
    digitalWrite(leftMotorPin1, HIGH);
    digitalWrite(leftMotorPin2, HIGH);
    digitalWrite(rightMotorPin1, HIGH);
    digitalWrite(rightMotorPin2, HIGH);
    
    delay(1000);
  }
}

/*
 * This mode allows the ARDUINO to receive and implement the following commands:
 * f – forward
 * b – backward
 * l – left
 * r – right
 * s – stop
 */
void manualMode() {
  char input;

  // check if data has been sent from the computer:
  if (Serial.available()) {  
     // read the most recent character
    input = Serial.read();
    // switch based on the character
    switch(input) {
      case 'f':  // forward
        forward();
        break;
      case 'b' : // backward
        backward();
        break;
      case 'l' : // left
        left();
        break;
      case 'r' : // right
        right();
        break;
      case 's' : // stop
        stop();
        break;
      default: // if character not recognized
        stop();
        break;
    }
  }
}

void autoMode() {
  distance = getDistance(); // measure front distance
  if (DEBUG) {
    Serial.print("Current Distance: ");
    Serial.println(distance);
  }
  
  if (distance > 30) {
    forward();
  } else if (distance < 15) {
    backward();
    randomTurn();
  } else {
  // whichWay();
    randomTurn();
  }
}

/*
 * Measure distance, unit "cm".
 */
long getDistance() {
  long duration;
  //pinMode(ultrasonicTrigPin, OUTPUT);
  digitalWrite(ultrasonicTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonicTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ultrasonicTrigPin, LOW);
  //pinMode(ultrasonicEchoPin, INPUT);
  duration = pulseIn(ultrasonicEchoPin, HIGH);
  
  return duration / 29 / 2;
}

/*
 * Turn left or right.
 */
void randomTurn() {
  long randomNumber;
  randomSeed(analogRead(0));
  randomNumber = random(0, 10);
  if (randomNumber > 5) {
    right();
  } else {
    left();
  }
}
