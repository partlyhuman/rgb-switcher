#define CHANNELS 15

// 5v VIN_ADC selector
#define VIN_A0 10
#define VIN_A1 16
#define VIN_A2 14
#define VIN_A3 15
#define VIN_ADC A0
// out of 1024, fraction of VCC
// about 3V at 4.9V
#define TRIGGER_V 620

// video mux
#define VID_A0 6
#define VID_A1 7
#define VID_A2 8
#define VID_A3 9
// audio mux
#define AUD_A0 5
#define AUD_A1 4
#define AUD_A2 3
#define AUD_A3 2

// Look up the address of a channel here
const uint8_t AUDIO_MAP[] {8,9,10,11,12,13,14,15,7,6,5,4,3,2,1,0};

const int ALL_OFF = -1;
static int currentChannel = ALL_OFF;

void flash(uint8_t pin, int times = 1, unsigned long onfor = 100, unsigned long offfor = 50) {
  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(onfor);
    digitalWrite(pin, LOW);
    delay(offfor);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("BOOTED");

  pinMode(VIN_A0, OUTPUT);
  pinMode(VIN_A1, OUTPUT);
  pinMode(VIN_A2, OUTPUT);
  pinMode(VIN_A3, OUTPUT);

  pinMode(VID_A0, OUTPUT);
  pinMode(VID_A1, OUTPUT);
  pinMode(VID_A2, OUTPUT);
  pinMode(VID_A3, OUTPUT);

  pinMode(AUD_A0, OUTPUT);
  pinMode(AUD_A1, OUTPUT);
  pinMode(AUD_A2, OUTPUT);
  pinMode(AUD_A3, OUTPUT);

  pinMode(VIN_ADC, INPUT);
  // pinMode(SW1, INPUT_PULLUP);
  // pinMode(SW2, INPUT_PULLUP);

  pinMode(LED_BUILTIN, OUTPUT);
  // pinMode(LED_BUILTIN_RX, OUTPUT);
  // pinMode(LED_BUILTIN_TX, OUTPUT);

  flash(LED_BUILTIN, 3);
  // flash(LED_BUILTIN_RX);
  // flash(LED_BUILTIN_TX);
}

void setSenseChannel(int channel) {
  digitalWrite(VIN_A0, (channel & 0b0001) > 0);
  digitalWrite(VIN_A1, (channel & 0b0010) > 0);
  digitalWrite(VIN_A2, (channel & 0b0100) > 0);
  digitalWrite(VIN_A3, (channel & 0b1000) > 0);
  delay(10);
}

int senseChannel(int channel) {
  setSenseChannel(channel);
  return analogRead(VIN_ADC);
}

// returns most active channel
int sweepChannels() {
  for (int i = 0; i < CHANNELS; i++) {
    int voltage = senseChannel(i);
    if (voltage > 0) {
      Serial.print(i);
      Serial.print(" ");
      Serial.println(voltage);
    }
    // For now the first one that's on is on
    if (voltage > TRIGGER_V) {
      return i;
    }
  }
  return ALL_OFF;
}

void setChannel(int channel) {
  if (channel < 0) return;

  digitalWrite(VID_A0, (channel & 0b0001) > 0);
  digitalWrite(VID_A1, (channel & 0b0010) > 0);
  digitalWrite(VID_A2, (channel & 0b0100) > 0);
  digitalWrite(VID_A3, (channel & 0b1000) > 0);

  uint8_t achannel = AUDIO_MAP[channel];
  Serial.print(channel);
  Serial.print(" -> audio channel ");
  Serial.println(achannel);
  digitalWrite(AUD_A0, (achannel & 0b0001) > 0);
  digitalWrite(AUD_A1, (achannel & 0b0010) > 0);
  digitalWrite(AUD_A2, (achannel & 0b0100) > 0);
  digitalWrite(AUD_A3, (achannel & 0b1000) > 0);
}

void loop() {
  bool off = currentChannel == ALL_OFF;
  delay(off ? 1000 : 4000);
  if (off) {
    int newChannel = sweepChannels();
    setChannel(newChannel);
    if (newChannel != currentChannel) {
      flash(LED_BUILTIN);
      Serial.print("Channel = ");
      Serial.println(newChannel);
    }
    currentChannel = newChannel;
  } else {
    // we only need to check if the current channel turned off (should we make this test more than once?)
    if (senseChannel(currentChannel) < TRIGGER_V) {
      currentChannel = ALL_OFF;
      Serial.println("OFF");
    }
  }
}
