const int MORSE_PIN = 22;

// Adjust these to match the transmitter
const unsigned long DOT_DASH_THRESHOLD = 400;
const unsigned long LETTER_GAP = 500;
const unsigned long WORD_GAP = 1400;

const unsigned long MIN_PULSE = 40;
const unsigned long MAX_PULSE = 3000;

String morse = "";
String currentWord = "";

bool wasHigh = false;
bool havePulse = false;

unsigned long pulseStart = 0;
unsigned long lastPulseEnd = 0;

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

char decodeMorse(const String& code) {
  for (int i = 0; i < 36; i++) {
    if (code == morseTable[i]) {
      if (i < 26) {
        return char('A' + i);
      }
      return char('0' + i - 26);
    }
  }

  return '\0';
}

void finishLetter() {
  if (morse.length() == 0) {
    return;
  }

  char decoded = decodeMorse(morse);

  // Invalid patterns are ignored instead of printing false letters.
  if (decoded != '\0') {
    currentWord += decoded;
  }

  morse = "";
}

void finishWord() {
  finishLetter();

  if (currentWord.length() > 0) {
    Serial.println(currentWord);
    currentWord = "";
  }
}

void setup() {
  pinMode(MORSE_PIN, INPUT_PULLDOWN);
  Serial.begin(115200);
}

void loop() {
  unsigned long now = millis();
  bool signalHigh = (digitalRead(MORSE_PIN) == HIGH);

  // Start of a pulse
  if (signalHigh && !wasHigh) {
    pulseStart = now;
    wasHigh = true;
  }

  // End of a pulse
  if (!signalHigh && wasHigh) {
    unsigned long duration = now - pulseStart;

    wasHigh = false;
    lastPulseEnd = now;
    havePulse = true;

    // Reject very short glitches and abnormally long pulses.
    if (duration >= MIN_PULSE && duration <= MAX_PULSE) {
      if (duration < DOT_DASH_THRESHOLD) {
        morse += '.';
      } else {
        morse += '-';
      }
    }
  }

  // Process gaps only while the input is LOW.
  if (!signalHigh && !wasHigh && havePulse) {
    unsigned long gap = now - lastPulseEnd;

    if (gap >= WORD_GAP) {
      finishWord();
      havePulse = false;
    } else if (gap >= LETTER_GAP) {
      finishLetter();
    }
  }
}
