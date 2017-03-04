#define greenPin 3
#define redPin1 5
#define redPin2 6
#define brightnessLimit 250
#define redDelay 100

int switchState = LOW;
int before = 0;
int delta = 0;
int fadeDelay = redDelay;

int greenLevel = 0;

int activeLevel = 0;
int activeLed = redPin1;
int inactiveLevel = 0;
int inactiveLed = redPin2;
int switchLed = 0;
bool noDelay;

void setup() {
  pinMode(greenPin, OUTPUT);
  pinMode(redPin1, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(2, INPUT);

  noDelay = true;
}

void loop() {
  switchState = digitalRead(2);

  noDelay = (digitalRead(7) == HIGH);

  delta = millis() - before;

 if(noDelay || delta > 1) {
    before = millis();
    
    if(switchState == LOW) {
      analogWrite(greenPin, greenLevel++);
      greenLevel = ensureBelowLimit(greenLevel);

      analogWrite(activeLed, activeLevel--);
      activeLevel = ensureOverZero(activeLevel);
      analogWrite(inactiveLed, inactiveLevel--);
      inactiveLevel = ensureOverZero(inactiveLevel);
    }
    else {
      if(activeLevel == brightnessLimit) {
        if(noDelay) {
          delay(250);
        }
        
        inactiveLevel = brightnessLimit;
        activeLevel = 0;
  
        switchLed = activeLed;
        activeLed = inactiveLed;
        inactiveLed = switchLed;

        fadeDelay = redDelay;
      }
      else if(fadeDelay-- < 0) {
        greenLevel = ensureOverZero(greenLevel-1);
        analogWrite(greenPin, greenLevel);
        
        analogWrite(activeLed, activeLevel++);
        activeLevel = ensureBelowLimit(activeLevel);
        analogWrite(inactiveLed, inactiveLevel--);
        inactiveLevel = ensureOverZero(inactiveLevel);
      }
    }
  }
}

int ensureOverZero(int level) {
  if(level < 0) {
    return 0;
  }
  else {
    return level;
  }
}

int ensureBelowLimit(int level) {
  if(level > brightnessLimit) {
    return brightnessLimit;
  }
  else {
    return level;
  }
}

