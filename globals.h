// Global variables
bool mute = false;
bool automode = false;

String timezone = "Tokyo";
const uint8_t fingerprint[] = {0xD0, 0x38, 0x24, 0x5A, 0x49, 0x54, 0xFE, 0x2B, 0xAF, 0x19, 0xBE, 0x61, 0xDE, 0x39, 0x74, 0x8C, 0x1A, 0x24, 0xCF, 0xFB};

uint8_t food = 10;
uint8_t happiness = 10;

byte sequence[4][8]; // Sequences are 4 frames
unsigned int framerate = 1000; // Default time for each frame
unsigned long animationMillis = 0; // Animation timer
uint8_t currentFrame = 0; // Current frame in sequence

unsigned int noteDuration = 120;
unsigned long melodyMillis = 0; // Melody length timer

unsigned int rythmn = 125; // noteDuration + pause before next note
unsigned long noteMillis = 0; // Rythmn timer
uint8_t currentNote[] = { 0, 0 }; // note being played now in frame / sequence

unsigned int eventInterval = 0; // Time before next event. Initialized in updateRandomEvents()
unsigned int eventDuration = 7000; // Arbitrary length of events
unsigned long eventMillis = 0; // Event timer

unsigned long currentMillis = millis(); // Clock
bool isIdle = true; // Is puyotchi idle
bool isActionEvent = false; // Is there a manually triggered action event ongoing
bool isRandomEvent = false; // Is there a randomly triggered action event ongoing
