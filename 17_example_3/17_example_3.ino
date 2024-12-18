#include <Servo.h>

// Arduino pin assignment
#define PIN_IR    0         // IR sensor at Pin A0
#define PIN_LED   9
#define PIN_SERVO 10

#define _DUTY_MIN 0         // servo full clockwise position (0 degree)
#define _DUTY_NEU 1500      // servo neutral position (90 degree)
#define _DUTY_MAX 3000      // servo full counter-clockwise position (180 degree)

#define _DIST_MIN  100.0    // minimum distance 100mm (10cm)
#define _DIST_MAX  250.0    // maximum distance 250mm (25cm)

#define EMA_ALPHA  0.1      // for EMA Filter
#define LOOP_INTERVAL 20    // Loop Interval (unit: msec)

Servo myservo;
unsigned long last_loop_time = 0; // unit: msec

float dist_prev = _DIST_MAX;
float dist_ema = _DIST_MIN;

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  
  Serial.begin(1000000);    // 1,000,000 bps
}

void loop()
{
  unsigned long time_curr = millis();
  int duty;
  float a_value, dist_raw;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Read analog value from IR sensor
  a_value = analogRead(PIN_IR);
  dist_raw = ((6762.0 / (a_value - 9.0)) - 4.0) * 10.0;

  // Range Filter: Update dist_ema only if within range and control LED
  if (dist_raw >= _DIST_MIN && dist_raw <= _DIST_MAX) {
    dist_ema = EMA_ALPHA * dist_raw + (1 - EMA_ALPHA) * dist_ema;
    dist_prev=dist_ema;
    digitalWrite(PIN_LED, HIGH); // Turn on LED if within range
  } else {
    dist_ema=dist_prev;
    digitalWrite(PIN_LED, LOW);  // Turn off LED if out of range
  }

  // Map equivalent code for servo duty (0 to 180 degrees)
  duty = _DUTY_MIN + (int)((dist_ema - _DIST_MIN) * (_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN));
  
  myservo.writeMicroseconds(duty);

  // Serial output for debugging
  Serial.print("_DUTY_MIN:");  Serial.print(_DUTY_MIN);
  Serial.print("_DIST_MIN:");  Serial.print(_DIST_MIN);
  Serial.print(",IR:");        Serial.print(a_value);
  Serial.print(",dist_raw:");  Serial.print(dist_raw);
  Serial.print(",ema:");       Serial.print(dist_ema);
  Serial.print(",servo:");     Serial.print(duty);
  Serial.print(",_DIST_MAX:"); Serial.print(_DIST_MAX);
  Serial.print(",_DUTY_MAX:"); Serial.print(_DUTY_MAX);
  Serial.println("");
}
