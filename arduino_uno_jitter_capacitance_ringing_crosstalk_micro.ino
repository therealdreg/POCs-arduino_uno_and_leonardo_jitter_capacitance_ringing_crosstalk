// by David Reguera Garcia aka Dreg

#include <Arduino.h>

const int jitterPin = 8;
const int crosstalkPin = 7;
const unsigned int baseDelay = 100;

void setup() {
  pinMode(jitterPin, OUTPUT);
  pinMode(crosstalkPin, OUTPUT);
  pinMode(9, OUTPUT);

  randomSeed(analogRead(A0));

  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);
  ICR1   = 15;
  OCR1A  = 7;

  TCCR3A = 0;
  TCCR3B = _BV(WGM32) | _BV(CS30);
  OCR3A  = 200;
  TIMSK3 = _BV(OCIE3A);

  sei();
}

ISR(TIMER3_COMPA_vect) {
  if (random(0, 1000) < 5) {
    PORTE &= ~(1 << PE6);
    asm volatile ("nop\n\t"
                  "nop\n\t");
    PORTE |= (1 << PE6);
  }
}

void loop() {
  unsigned int jitter = random(0, 50);

  digitalWrite(jitterPin, HIGH);
  delayMicroseconds(baseDelay + jitter);

  jitter = random(0, 50);
  digitalWrite(jitterPin, LOW);
  delayMicroseconds(baseDelay + jitter);
}
