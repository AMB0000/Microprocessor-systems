#define GREEN_LED 4
#define RED_LED   5
#define BUTTON    2

volatile unsigned long gSeconds = 0;
volatile unsigned char gTick10Hz = 0;
unsigned long lastPrint = 0;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  Serial.begin(9600);

  cli();

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 62499;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A);

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 155;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 |= (1 << OCIE2A);

  sei();
}

ISR(TIMER1_COMPA_vect) {
  static unsigned char state = 0;
  state ^= 1;
  digitalWrite(GREEN_LED, state);
  gSeconds++;
}

ISR(TIMER2_COMPA_vect) {
  gTick10Hz++;
  if (gTick10Hz >= 10) {
    gTick10Hz = 0;
    if (digitalRead(BUTTON) == HIGH)
      digitalWrite(RED_LED, HIGH);
    else
      digitalWrite(RED_LED, LOW);
  }
}

void loop() {
  unsigned long now;
  cli();
  now = gSeconds;
  sei();

  if (now - lastPrint >= 3) {
    lastPrint = now;
    Serial.println("Lab 3 running: 3 seconds elapsed");
  }
}
