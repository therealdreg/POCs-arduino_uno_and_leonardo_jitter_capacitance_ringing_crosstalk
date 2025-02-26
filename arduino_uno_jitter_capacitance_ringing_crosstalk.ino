#include <Arduino.h>

// by David Reguera Garcia aka Dreg https://github.com/therealdreg/arduino_uno_jitter_capacitance_ringing_crosstalk

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
  ICR1 = 15;
  OCR1A = 7;

  TCCR2A = _BV(WGM21);
  TCCR2B = _BV(CS20);
  OCR2A = 200;
  TIMSK2 = _BV(OCIE2A);

  sei();
}

ISR(TIMER2_COMPA_vect) {
  if (random(0, 1000) < 5) {
    PORTD &= ~(1 << PD7);
    asm volatile ("nop\n\t"
                  "nop\n\t");
    PORTD |= (1 << PD7);
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
