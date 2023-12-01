 #include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int buzzerPin = 3;
int i = 0;
volatile int impulsCount = 0;
float total_amount = 0;
float inserted_coin = 0; // variable for display screen
int relayState = LOW;  // Initialize relayState as LOW
const int relay = 8;
unsigned long relayActivationTime = 0;
unsigned long relayOffTime = 0;
long relayActivationDuration = 60000;
long relayOffDuration = 0;
bool dummy = false;


void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(relay, OUTPUT); // Set relay as output
  pinMode(2, INPUT); // Pin untuk arduino baca coins dari coin acceptor
  Serial.begin(9600);
  // Interrupt connected to PIN D2 executing IncomingImpuls function when signal goes from HIGH to LOW
  attachInterrupt(0, incomingImpuls, RISING);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0); //0 column 1 row
  lcd.print("Mini Charging");
  lcd.setCursor(5, 1);
  lcd.print("Station");
  delay(2000);

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("Please insert");
  lcd.setCursor(5, 1);
  lcd.print("coins");
  delay(2000);
  lcd.clear();
}

void incomingImpuls() {
  impulsCount++;
  i = 0;
}

void loop() {
  i = i + 1;
  Serial.print("i=");
  Serial.print(i);
  Serial.print(" Impulses:");
  Serial.print(impulsCount);
  Serial.print(" Total:");
  Serial.println(total_amount);

  // 50sen baru
  if (i > 30 and impulsCount == 1) {
    if (dummy == false) {
      total_amount = total_amount + 0.5;
      inserted_coin = inserted_coin + 0.5;
      impulsCount = 0;
      i = 0;
    }
    else {
      dummy = false;
      i = 0;
      impulsCount = 0;
    }
  }
  // 50sen lama
  if (i > 30 and impulsCount == 2) {
    if (dummy == false) {
      total_amount = total_amount + 0.5;
      inserted_coin = inserted_coin + 0.5;
      impulsCount = 0;
      i = 0;
    }
    else {
      dummy = false;
      i = 0;
      impulsCount = 0;
    }
  }

    // 20sen baru
  if (i > 30 and impulsCount == 3) {
    if (dummy == false) {
      total_amount = total_amount + 0.2;
      inserted_coin = inserted_coin + 0.2;
      impulsCount = 0;
      i = 0;
    }
    else {
      dummy = false;
      i = 0;
      impulsCount = 0;
    }
  } 
   // 20 sen lama
    if (i > 30 and impulsCount == 4) {
    if (dummy == false) {
      total_amount = total_amount + 0.2;
      inserted_coin = inserted_coin + 0.2;
      impulsCount = 0;
      i = 0;
    }
    else {
      dummy = false;
      i = 0;
      impulsCount = 0;
    }
  }

 // 10 sen baru
  if (i > 30 and impulsCount == 5) {
    if (dummy == false) {
      total_amount = total_amount + 0.1;
      inserted_coin = inserted_coin + 0.1;
      impulsCount = 0;
      i = 0;
    }
    else {
      dummy = false;
      i = 0;
      impulsCount = 0;
    }
  }

 // 10 lama
    if (i > 30 and impulsCount == 6) {
    if (dummy == false) {
      total_amount = total_amount + 0.1;
      inserted_coin = inserted_coin + 0.1;
      impulsCount = 0;
      i = 0;
    }
    else {
      dummy = false;
      i = 0;
      impulsCount = 0;
    }
  }
  

  // set timer for relay
  unsigned long timeNow = millis();

  if (relayState == LOW && total_amount >= 1.00) {
    relayActivationTime = timeNow;
    relayState = HIGH;
    dummy = true;
    total_amount -= 1.00;  // Deduct the coin value
  }

  if (relayState == HIGH && timeNow - relayActivationTime >= relayActivationDuration) {
    dummy = false;
    relayOffTime = timeNow;
    relayState = LOW;
    relayActivationTime = 0; // Reset relayActivationTime
  }

  if (relayState == LOW && relayOffTime > 0 && timeNow - relayOffTime >= relayOffDuration) {
    relayOffTime = 0; // Reset relayOffTime

    // buzzer will beeps 3 times to indicate the user that timer has ended
    digitalWrite(buzzerPin, HIGH);
    delay(1000); // Buzzer will sound for 10 seconds
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    delay(1000);
    digitalWrite(buzzerPin, HIGH);
    delay(1000); // Buzzer will sound for 10 seconds
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    delay(1000);
    digitalWrite(buzzerPin, HIGH);
    delay(1000); // Buzzer will sound for 10 seconds
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    delay(1000);
    
    // after the timer ends, the total amount of coins will be reset to zero.
    total_amount = 0;
    inserted_coin = 0;
  }

  digitalWrite(relay, relayState);
  lcd.setCursor(0, 0);
  lcd.print("Inserted: ");
  lcd.print(inserted_coin);
  lcd.setCursor(0, 1);
  lcd.print("Plug:");
  if (relayState == HIGH && relayActivationTime > 0) {
    lcd.print("ON ");
    unsigned long remainingTime = relayActivationDuration - (timeNow - relayActivationTime);
    int remainingMinutes = remainingTime / 60000;
    int remainingSeconds = (remainingTime % 60000) / 1000;
    String formattedSeconds = (remainingSeconds < 10) ? "0" + String(remainingSeconds) : String(remainingSeconds);

    lcd.print(remainingMinutes);
    lcd.print("min ");
    lcd.print(formattedSeconds);
    lcd.print("s ");
  }

  else if (relayState == LOW && relayOffTime > 0) {
    lcd.print("OFF ");
    unsigned long remainingTime = relayOffDuration - (timeNow - relayOffTime);
    int remainingMinutes = remainingTime / 60000;
    int remainingSeconds = (remainingTime % 60000) / 1000;
    String formattedSeconds = (remainingSeconds < 10) ? "0" + String(remainingSeconds) : String(remainingSeconds);

    lcd.print(remainingMinutes);
    lcd.print("min ");
    lcd.print(formattedSeconds);
    lcd.print("s ");
    lcd.clear();
  } else {
    lcd.print("OFF ");
  }
}
