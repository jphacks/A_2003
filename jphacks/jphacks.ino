typedef struct Timer {
  int hour;
  int minute;
  int second;
  int ms;
} Timer, *pTimer;

Timer timer;

unsigned long int ms, ms_diff;

/** モーターのスイッチプログラム **/
void setup() {
  pinMode(11, OUTPUT);
  pinMode(7, INPUT);
  Serial.begin(9600);
  fetch_timer(__TIME__);
}
void loop() {
  static unsigned long int tmp = 0;
  ms = millis();
  ms_diff = ms - tmp;

  forward_timer(ms_diff);

  Serial.print(timer.hour, DEC);
  Serial.print(":");
  Serial.print(timer.minute, DEC);
  Serial.print(":");
  Serial.print(timer.second, DEC);
  Serial.print("\n");

  if (digitalRead(7) == LOW) {
    analogWrite(11, 255);
    Serial.print(digitalRead(7));
    delay(10000);
  }

  if ((timer.hour == 17) && (timer.minute == 0)) {  // 00:08:00設定
    analogWrite(11, 255);
    delay(1000);
  } else {
    analogWrite(11, 0);
    delay(100);
  }

  tmp = ms;
}

bool fetch_timer(const char *str) {
  int hour, minute, second;

  if (sscanf(str, "%d:%d:%d", &hour, &minute, &second) != 3) return false;
  timer.hour = hour;
  timer.minute = minute;
  timer.second = second;
  timer.ms = 0;

  return true;
}

void forward_timer(unsigned long int ms_diff) {
  int c_hour = timer.hour;
  int c_minute = timer.minute;
  int c_second = timer.second;
  unsigned long int c_ms = timer.ms + ms_diff;

  c_second += c_ms / 1000;
  c_ms = c_ms % 1000;
  c_minute += c_second / 60;
  c_second = c_second % 60;
  c_hour += c_minute / 60;
  c_minute = c_minute % 60;
  c_hour = c_hour % 24;

  timer.hour = c_hour;
  timer.minute = c_minute;
  timer.second = c_second;
  timer.ms = c_ms;

  return;
}
