#define MORSE_PIN A2
#define GROUND_PIN  A0

#define DOT_TIME 200

const char* messages[] = {
  "HELLO",
  "HOW ARE YOU"
};

const int MESSAGE_COUNT = 2;

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

void sendSymbol(char symbol) {
  digitalWrite(MORSE_PIN, HIGH);

  if (symbol == '.') {
    delay(DOT_TIME);
  } else {
    delay(DOT_TIME * 3);
  }

  digitalWrite(MORSE_PIN, LOW);
  delay(DOT_TIME); // Gap between symbols
}

void sendLetter(char c) {
  if (c >= 'A' && c <= 'Z') {
    const char* code = morseTable[c - 'A'];

    for (int i = 0; code[i] != '\0'; i++) {
      sendSymbol(code[i]);
    }

    delay(DOT_TIME * 2); // Additional gap: 3 units total
  }
  else if (c >= '0' && c <= '9') {
    const char* code = morseTable[26 + c - '0'];

    for (int i = 0; code[i] != '\0'; i++) {
      sendSymbol(code[i]);
    }

    delay(DOT_TIME * 2);
  }
}

void sendMessage(const char* message) {
  for (int i = 0; message[i] != '\0'; i++) {
    if (message[i] == ' ') {
      delay(DOT_TIME * 4); // 7 units total, including prior gap
    } else {
      sendLetter(toupper(message[i]));
    }
  }
}

void setup() {
  pinMode(MORSE_PIN, OUTPUT);
  digitalWrite(MORSE_PIN, LOW);

  // GROUND_PIN is a configuration placeholder.
  // Do not configure a physical GND pin as a digital output.
}

void loop() {
  for (int i = 0; i < MESSAGE_COUNT; i++) {
    sendMessage(messages[i]);

    digitalWrite(MORSE_PIN, LOW);
    delay(3000);
  }
}
