#define CHANNELS 15

// 5v sense selector
#define R0 3
#define R1 2
#define R2 5
#define R3 4
#define SENSE A0
#define TRIGGER_V 900

// mux selector
#define S0 7
#define S1 8
#define S2 9
#define S3 10

// buttons
#define SW1 12
#define SW2 11

// IR
#define IR 0


// let -1 mean none are active
const int ALL_OFF = -1;
static int currentChannel = ALL_OFF;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("BOOTED");

  pinMode(R0, OUTPUT);
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
}

void setSenseChannel(int channel) {
  digitalWrite(R0, (channel & 0b0001) > 0);
  digitalWrite(R1, (channel & 0b0010) > 0);
  digitalWrite(R2, (channel & 0b0100) > 0);
  digitalWrite(R3, (channel & 0b1000) > 0);
  delay(10);
}

int senseChannel(int channel) {
  setSenseChannel(channel);
  return analogRead(SENSE);
}

// returns most active channel
int sweepChannels() {
  for (int i = 0; i < CHANNELS; i++) {
    int sense = senseChannel(i);
    Serial.print(i);
    Serial.print(" ");
    Serial.println(sense);
    // For now the first one that's on is on
    if (sense > TRIGGER_V) {
      return i;
    }
  }
  return ALL_OFF;
}

void setChannel(int channel) {
  if (channel < 0) return;
  digitalWrite(S0, (channel & 0b0001) > 0);
  digitalWrite(S1, (channel & 0b0010) > 0);
  digitalWrite(S2, (channel & 0b0100) > 0);
  digitalWrite(S3, (channel & 0b1000) > 0);
}

void loop() {
  bool off = currentChannel == ALL_OFF;
  delay(off ? 2000 : 6000);
  if (off) {
    currentChannel = sweepChannels();
    setChannel(currentChannel);
  } else {
    // we only need to check if the current channel turned off (should we make this test more than once?)
    if (senseChannel(currentChannel) < TRIGGER_V) {
      currentChannel = ALL_OFF;
    }
  }
  Serial.print("Current Channel = ");
  Serial.println(currentChannel);
}
