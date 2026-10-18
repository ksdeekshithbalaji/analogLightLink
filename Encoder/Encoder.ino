const int LED_PIN = 22;
const int DOT_TIME = 200;  // Dot duration in milliseconds

const char* morseTable[36] = {
  ".-", "-...", "-.-.", "-..", ".",       // A-E
  "..-.", "--.", "....", "..", ".---",    // F-J
  "-.-", ".-..", "--", "-.", "---",       // K-O
  ".--.", "--.-", ".-.", "...", "-",      // P-T
  "..-", "...-", ".--", "-..-", "-.--",   // U-Y
  "--..",                                 // Z
  "-----", ".----", "..---", "...--",     // 0-3
  "....-", ".....", "-....", "--...",     // 4-7
  "---..", "----."                        // 8-9
};

void blinkDot() {
  digitalWrite(LED_PIN, HIGH);
  delay(DOT_TIME);
  digitalWrite(LED_PIN, LOW);
  delay(DOT_TIME);
}

void blinkDash() {
  digitalWrite(LED_PIN, HIGH);
  delay(DOT_TIME * 3);
  digitalWrite(LED_PIN, LOW);
  delay(DOT_TIME);
}

void encodeMorse(char c) {
  c = toupper(c);

  if (c >= 'A' && c <= 'Z') {
    const char* code = morseTable[c - 'A'];

    for (int i = 0; code[i] != '\0'; i++) {
      if (code[i] == '.') {
        blinkDot();
      } else {
        blinkDash();
      }
    }

    delay(DOT_TIME * 2); // Remaining 2 units: letter gap
  }
  else if (c >= '0' && c <= '9') {
    const char* code = morseTable[26 + c - '0'];

    for (int i = 0; code[i] != '\0'; i++) {
      if (code[i] == '.') {
        blinkDot();
      } else {
        blinkDash();
      }
    }

    delay(DOT_TIME * 2);
  }
  else if (c == ' ') {
    delay(DOT_TIME * 4); // Word gap
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.begin(115200);
  Serial.println("ESP32 Morse Code Encoder");
  Serial.println("Type text and press Enter:");
}

void loop() {
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    message.trim();

    Serial.print("Text: ");
    Serial.println(message);

    Serial.print("Morse: ");

    for (int i = 0; i < message.length(); i++) {
      char c = toupper(message[i]);

      if (c >= 'A' && c <= 'Z') {
        Serial.print(morseTable[c - 'A']);
      }
      else if (c >= '0' && c <= '9') {
        Serial.print(morseTable[26 + c - '0']);
      }
      else if (c == ' ') {
        Serial.print(" / ");
      }

      Serial.print(' ');
    }

    Serial.println();
    Serial.println("Blinking LED...");

    for (int i = 0; i < message.length(); i++) {
      encodeMorse(message[i]);
    }

    digitalWrite(LED_PIN, LOW);
    Serial.println("Done. Enter another message:");
  }
}
