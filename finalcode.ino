#include "arduino_secrets.h"
#include "thingProperties.h"
#include <WiFiNINA.h>
#include <NeoPixelConnect.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BH1750.h>
#include <NanoRP2040MICTEST_inferencing.h>
#include <PDM.h>
#include <DHT11.h>
#include <ArduinoBLE.h>

#define PIN_un 5
#define NUMPIXELS_un 2
#define PIN_up 15
#define NUMPIXELS_up 12
#define touch_pin 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#define DHT_PIN 2
#define DHTTYPE DHT11
#define SERVICE_UUID        "0000ffe0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000ffe1-0000-1000-8000-00805f9b34fb"

bool voicesetupdone = 0;
bool btsetupdone = 0;
int colorneo[3] = {0, 0, 0};
int icolor[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int jcolor[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int kcolor[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int modenum;
int newmode = 5;
int colorcode;
int Aturnnum = 0;
float humi = 0.0;
float temp = 0.0;
float lux;
double Saturation;
double Hue;
double Value;
int maxlocation;
unsigned long touchStartTime = 0;
bool isTouching = false;
bool isInhibited = false;
const unsigned long minTouchTime = 200;
static const unsigned char PROGMEM bamyanggang_image[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xf0, 0xc6, 0x30, 0xc9, 0x8c, 0x47, 0x81, 0xe3, 0x19, 0x9e, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x99, 0x46, 0x70, 0x49, 0xce, 0x48, 0x02, 0x07, 0x1d, 0xb0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xf1, 0x47, 0x70, 0x31, 0x4f, 0x58, 0x06, 0x05, 0x95, 0xa0, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xdb, 0x65, 0x50, 0x32, 0x4d, 0x59, 0x86, 0x65, 0x93, 0xa7, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x9b, 0x65, 0x90, 0x33, 0x6c, 0xc8, 0x82, 0x2c, 0x93, 0xb3, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xfa, 0x34, 0x90, 0x36, 0x2c, 0xcf, 0x83, 0xe8, 0xd1, 0x9f, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x21, 0x44, 0x08, 0x67, 0x1d, 0x08, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0d, 0xb1, 0x46, 0x1c, 0xe4, 0x31, 0x18, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0d, 0xb9, 0x46, 0x1c, 0xa4, 0x41, 0x19, 0xc0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0f, 0x2d, 0x46, 0x1c, 0xa7, 0x41, 0xf9, 0x20, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0c, 0x27, 0x46, 0x1b, 0x24, 0x61, 0x99, 0xe0, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0c, 0x23, 0x24, 0x1b, 0x24, 0x31, 0x1b, 0x30, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x18, 0x00, 0x07, 0x1d, 0x0a, 0x10, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

BH1750 lightMeter(0x23);
NeoPixelConnect p(PIN_un, NUMPIXELS_un, pio0, 1);
NeoPixelConnect pup(PIN_up, NUMPIXELS_up, pio1, 1);
Adafruit_SSD1306 myOledDisplay(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT11 dht11(DHT_PIN);
BLEService bleService(SERVICE_UUID);
BLEStringCharacteristic bleCharacteristic(CHARACTERISTIC_UUID, BLERead | BLEWrite | BLENotify, 32);

volatile bool touchInterrupt = false;
unsigned long sensingInterval=1000;
unsigned long speechsensingInterval=4000;
unsigned long seningPreviousMillis=0;
unsigned long seningPreviousMillisdht=0;
unsigned long speechseningPreviousMillis=0;
unsigned long dynamicAPreviousMillis=0;
unsigned long dynamicAsensingInterval=1000;
unsigned long dynamicBPreviousMillis=0;
unsigned long dynamicBsensingInterval=1000;
typedef struct {
    int16_t *buffer;
    uint8_t buf_ready;
    uint32_t buf_count;
    uint32_t n_samples;
} inference_t;
static inference_t inference;
static signed short sampleBuffer[2048];
static bool debug_nn = false;
static volatile bool record_ready = false;
float nothappy;
int casenum = 0;

void setup() {
  Serial.begin(115200);
  pinMode(touch_pin, INPUT_PULLUP);
  Wire.begin();
  lightMeter.begin();
  attachInterrupt(touch_pin, touchHandler, CHANGE);
  modenum = 5;
  delay(1500);
  executemode(modenum);
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  if(!myOledDisplay.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  myOledDisplay.clearDisplay();
  myOledDisplay.display();
}

void loop() {
  ArduinoCloud.update();
  if (touchInterrupt) {
    touchInterrupt = false;
    handleTouch();
  }
  if (isInhibited == false) {
  executemode(modenum);
  }
}

void touchHandler() {
  touchInterrupt = true;
}

void handleTouch() {
  int touchState = digitalRead(touch_pin);
  unsigned long currentTime = millis();

  if (touchState == HIGH) { // Touching
    Serial.println("0");
    if (!isTouching) { // Touch started
      touchStartTime = currentTime;
      isTouching = true;
    }
  }
  else { // Touch ended
    Serial.println("00");
    if (isTouching) {
      unsigned long touchDuration = currentTime - touchStartTime;
      isTouching = false;

      if (touchDuration < minTouchTime) {
        // 무시
        return;
      } else if (currentTime - touchStartTime >= 5000) { // Touching for 5 seconds
      isInhibited = true;
      pup.neoPixelClear(true);
      myOledDisplay.clearDisplay();
      myOledDisplay.display();
      Serial.println("Inhibited mode activated, lights cleared");
      }

      if (isInhibited) {
        if (touchDuration >= 2000 && touchDuration < 5000) { // Touch duration between 2 and 5 seconds
          pup.neoPixelFill(250, 250, 250, true);
          oled_display();
          pup.neoPixelClear(true);
          modenum = 5;
          executemode(modenum);
          isInhibited = false;
          Serial.println("Inhibited mode deactivated, switched to mode 5");
        }
      } else {
        if ((touchDuration < 2000) && (modenum == 5)) { // Touch duration less than 2 seconds & in idle mode
          colorneo[0] = random(0, 256);
          colorneo[1] = random(0, 256);
          colorneo[2] = random(0, 256);
          pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
          Serial.println("Random color displayed");
        }
      }
    }
  }
}

void onModevariableChange() {
  int newmode = (int)modevariable;
  if (newmode != modenum) {
    Serial.print("Mode variable changed: ");
    Serial.println(newmode);
    modenum = newmode;
  }
}

void executemode(int mode) {
  switch (mode) {
    case 1: // BLE control
        btcontrol();
      break;
    case 2: // Emotional light
      emotional_light();
      break;
    case 3: // Auto brightness
      controldhttemphumidata();
      break;
    case 4: // Speech control
      if((unsigned long)(millis() - speechseningPreviousMillis) >= speechsensingInterval) {
        speechseningPreviousMillis = millis();
        recordvoice();
        sendcolordata();
      }
      break;
    case 5: // Idle
      getdhttemphumidata();
      break;
    case 6: // Dynamic light #1
      if((unsigned long)(millis() - dynamicAPreviousMillis) >= dynamicAsensingInterval) {
        dynamicAPreviousMillis = millis();
        dynamicA();
        getdhttemphumidata();
      }
      break;
    case 7: // Dynamic light #2
      if((unsigned long)(millis() - dynamicBPreviousMillis) >= dynamicBsensingInterval) {
        dynamicBPreviousMillis = millis();
        dynamicB();
        getdhttemphumidata();
      }
      break;
    case 8: // Off
      pup.neoPixelClear(true);
      getdhttemphumidata();
      break;
    case 9: // White
      pup.neoPixelFill(250, 250, 250, true);
      getdhttemphumidata();
      break;
    case 10: // Red
      pup.neoPixelFill(250, 0, 0, true);
      getdhttemphumidata();
      break;
    case 11: // Green
      pup.neoPixelFill(0, 250, 0, true);
      getdhttemphumidata();
      break;
    case 12: // Blue
      pup.neoPixelFill(0, 0, 250, true);
      getdhttemphumidata();
      break;
    default:
      Serial.println("Unknown mode");
      getdhttemphumidata();
      break;
  }
}

void dynamicA() {
  for (int i=0;i<12;i++) {
    icolor[i] = random(0, 256);
    jcolor[i] = random(0, 256);
    kcolor[i] = random(0, 256);
  }
  for (int i=0;i<12;i++) {
    pup.neoPixelSetValue(i, icolor[i], jcolor[i], kcolor[i], true);
  }
}

void dynamicB() {
  switch(casenum) {
    case 0:
      pup.neoPixelFill(255, 0, 0, true);
      break;
    case 1:
      pup.neoPixelFill(255, 0, 255, true);
      break;
    case 2:
      pup.neoPixelFill(255, 127, 0, true);
      break;
    case 3:
      pup.neoPixelFill(255, 255, 0, true);
      break;
    case 4:
      pup.neoPixelFill(0, 255, 0, true);
      break;
    case 5:
      pup.neoPixelFill(0, 255, 255, true);
      break;
    case 6:
      pup.neoPixelFill(0, 0, 255, true);
      casenum = -1;
      break;
  }
  casenum = casenum + 1;
}

void senddata(float temperature, float humidity, float light) {
  mytemp = temperature;
  myhumid = humidity;
  mylight = light;
}

void btcontrol() {
  if (!btsetupdone) {
    if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("Bamyanggang");
  BLE.setDeviceName("Bamyanggang");
  BLE.setAdvertisedService(bleService);
  bleService.addCharacteristic(bleCharacteristic);
  BLE.addService(bleService);
  BLE.advertise();
  Serial.println("Bluetooth device active, waiting for connections...");
  btsetupdone = 1;
  }
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
    while (central.connected()) {
      if (bleCharacteristic.written()) {
        Serial.print("Characteristic written: ");
        Serial.println(bleCharacteristic.value());
        String value = bleCharacteristic.value();
        colorcode = value.toInt();
        colorneo[0] = (colorcode / 1000000) % 1000;
        colorneo[1] = (colorcode / 1000) % 1000;
        colorneo[2] = colorcode % 1000;
        for (int i=0;i<3;i++) {
          if (colorneo[i] > 255) {
          } else {
            pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
          }
        }
        bleCharacteristic.writeValue("Message received!");
      }
    }
    Serial.print("Disconnected from central: ");
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    pup.neoPixelClear(true);
    Serial.println(central.address());
    modenum = 5;
    executemode(modenum);
    btsetupdone = 0;
  }
}

void oleddatadisplay(float t, float h, float l) {
  myOledDisplay.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  myOledDisplay.fillRect(0, 0, 128, 8, BLACK); myOledDisplay.setCursor(0, 0);
  myOledDisplay.print(" T: "); myOledDisplay.print(t);
  myOledDisplay.print("["); myOledDisplay.print((char)247);
  myOledDisplay.print("C"); myOledDisplay.print("]"); myOledDisplay.display();
  myOledDisplay.fillRect(0, 8, 128, 8, BLACK); myOledDisplay.setCursor(0, 8);
  myOledDisplay.print(" H: "); myOledDisplay.print(h);
  myOledDisplay.print((String)"[%]"); myOledDisplay.display();
  myOledDisplay.fillRect(0, 16, 128, 8, BLACK); myOledDisplay.setCursor(0, 16);
  myOledDisplay.print(" L: "); myOledDisplay.print(l);
  myOledDisplay.print(" lux"); myOledDisplay.display();
  myOledDisplay.fillRect(0, 24, 128, 8, BLACK); myOledDisplay.setCursor(0, 24);
  myOledDisplay.print(" BamYangGang "); myOledDisplay.display();
}

void controldhttemphumidata() {
  if((unsigned long)(millis() - seningPreviousMillis) >= sensingInterval) {
    seningPreviousMillis = millis();
    int err;
    err = dht11.read(humi, temp);
    float lux = lightMeter.readLightLevel();
    senddata(temp, humi, lux);
    oleddatadisplay(temp, humi, lux);
    luxcontrol(lux);
    sendcolordata();
  }
}

void luxcontrol(float light) {
  if (light > 500) {
    pup.neoPixelClear(true);
  } else {
    int whiteval = (int)((500-light) * 255 / 500);
    pup.neoPixelFill(whiteval, whiteval, whiteval, true);
  }
}

void getdhttemphumidata() {
  if((unsigned long)(millis() - seningPreviousMillisdht) >= sensingInterval) {
    seningPreviousMillisdht = millis();
    int err;
    err = dht11.read(humi, temp);
    float lux = lightMeter.readLightLevel();
    senddata(temp, humi, lux);
    sendcolordata();
    oleddatadisplay(temp, humi, lux);
  }
}

void emotional_light() {
  if((unsigned long)(millis() - seningPreviousMillisdht) >= sensingInterval) {
    seningPreviousMillisdht = millis();
    int err;
    err = dht11.read(humi, temp);
    float lux = lightMeter.readLightLevel();
    senddata(temp, humi, lux);
    nothappy = (temp + humi/100);
    Serial.print("not happy? : ");
    Serial.println(nothappy);
    if (nothappy < 5) {
      colorneo[0] = 255;
      colorneo[1] = 89;
      colorneo[2] = 89;  
    } else if (nothappy < 10) {
      colorneo[0] = 254;
      colorneo[1] = 155;
      colorneo[2] = 120;
    } else if (nothappy < 25) {
      colorneo[0] = 255;
      colorneo[1] = 255;
      colorneo[2] = 255;
    } else if (nothappy < 30) {
      colorneo[0] = 58;
      colorneo[1] = 199;
      colorneo[2] = 131;
    } else {
      colorneo[0] = 0;
      colorneo[1] = 0;
      colorneo[2] = 250;
    }
    pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
    sendcolordata();
  }
}

void recordvoice() {
  if (!voicesetupdone) {
    Serial.println("Edge Impulse Inferencing Demo");
    ei_printf("Inferencing settings:\n");
    ei_printf("\tInterval: ");
    ei_printf_float((float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf(" ms.\n");
    ei_printf("\tFrame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tSample length: %d ms.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT / 16);
    ei_printf("\tNo. of classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));
    if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) {
      ei_printf("ERR: Could not allocate audio buffer (size %d), this could be due to the window length of your model\r\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
      return;
    }
  voicesetupdone = 1;
  }
  ei_printf("Starting inferencing in 2 seconds...\n");
  delay(2000);
  ei_printf("Recording...\n");
  p.neoPixelFill(150, 0, 0, true);
  bool m = microphone_inference_record();
  if (!m) {
    ei_printf("ERR: Failed to record audio...\n");
  return;
  }
  ei_printf("Recording done\n");
  p.neoPixelClear(true);
  signal_t signal;
  signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT;
  signal.get_data = &microphone_audio_signal_get_data;
  ei_impulse_result_t result = { 0 };
  EI_IMPULSE_ERROR res = run_classifier_continuous(&signal, &result, debug_nn);
  if (res != EI_IMPULSE_OK) {
    ei_printf("ERR: Failed to run classifier (%d)\n", res);
    return;
  }
  ei_printf("run_classifier returned: %d\r\n", res);
  print_inference_result(result);
  controlNeopixel(result);
}

static void pdm_data_ready_inference_callback(void) {
  int bytesAvailable = PDM.available();
  int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable);
  if ((inference.buf_ready == 0) && (record_ready == true)) {
    for(int i = 0; i < bytesRead>>1; i++) {
      inference.buffer[inference.buf_count++] = sampleBuffer[i];
      if(inference.buf_count >= inference.n_samples) {
        inference.buf_count = 0;
        inference.buf_ready = 1;
        break;
      }
    }
  }
}

static bool microphone_inference_start(uint32_t n_samples) {
  inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));
  if(inference.buffer == NULL) {
    return false;
  }
  inference.buf_count  = 0;
  inference.n_samples  = n_samples;
  inference.buf_ready  = 0;
  PDM.onReceive(pdm_data_ready_inference_callback);
  PDM.setBufferSize(2048);
  delay(250);
  if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY)) {
    ei_printf("ERR: Failed to start PDM!");
    microphone_inference_end();
    voicesetupdone = 0;
    return false;
  }
  return true;
}

static bool microphone_inference_record(void) {
  bool ret = true;
  record_ready = true;
  while (inference.buf_ready == 0) {
    delay(10);
  }
  inference.buf_ready = 0;
  record_ready = false;
  return ret;
}

static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr) {
  numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);
  return 0;
}

static void microphone_inference_end(void) {
  PDM.end();
  ei_free(inference.buffer);
}

void print_inference_result(ei_impulse_result_t result) {
  ei_printf("Timing: DSP %d ms, inference %d ms, anomaly %d ms\r\n",
  result.timing.dsp,
  result.timing.classification,
  result.timing.anomaly);
  ei_printf("Predictions:\r\n");
  for (uint16_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    ei_printf("  %s: ", ei_classifier_inferencing_categories[i]);
    ei_printf("%.5f\r\n", result.classification[i].value);
  }
  #if EI_CLASSIFIER_HAS_ANOMALY == 1
  ei_printf("Anomaly prediction: %.3f\r\n", result.anomaly);
  #endif
}

void controlNeopixel(ei_impulse_result_t result) {
    float maxval = result.classification[0].value;
    maxlocation = 0;
    for (int i = 1; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
        if (result.classification[i].value > maxval) {
            maxval = result.classification[i].value;
            maxlocation = i;
        }
    }
    Serial.println(maxlocation);
    if (maxlocation == 0) {
        colorneo[0] = 0;
        colorneo[1] = 0;
        colorneo[2] = 255;
        pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
    } else if (maxlocation == 1) {
        colorneo[0] = 0;
        colorneo[1] = 255;
        colorneo[2] = 0;
        pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
    } else if (maxlocation == 4) {
        pup.neoPixelClear(true);
    } else if (maxlocation == 5) {
        colorneo[0] = 255;
        colorneo[1] = 255;
        colorneo[2] = 255;
        pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
    } else if (maxlocation == 6) {
        colorneo[0] = 255;
        colorneo[1] = 0;
        colorneo[2] = 0;
        pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
    } else {}
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor."
#endif

void onMytempChange() {
}

void onMyhumidChange() {
}

void onMylightChange() {
}

void onMycolorChange() {
  if (mycolor.getSwitch()) {
    uint8_t r, g, b;
    mycolor.getValue().getRGB(r, g, b);
    colorneo[0] = map(r, 0, 255, 0, 255);
    colorneo[1] = map(g, 0, 255, 0, 255);
    colorneo[2] = map(b, 0, 255, 0, 255);
    pup.neoPixelFill(colorneo[0], colorneo[1], colorneo[2], true);
  }
}
  
void rgb_to_hsv(double r, double g, double b) {
  r = r / 255.0;
  g = g / 255.0;
  b = b / 255.0;
  double cmax = max(r, max(g, b));
  double cmin = min(r, min(g, b));
  double delta = cmax - cmin;
  Value = cmax * 100;
  if (cmax == 0) {
    Saturation = 0;
  } else {
    Saturation = (delta / cmax) * 100;
  }
  if (delta == 0) {
    Hue = 0;
  } else if (cmax == r) {
    Hue = fmod((60 * ((g - b) / delta) + 360), 360);
  } else if (cmax == g) {
    Hue = fmod((60 * ((b - r) / delta) + 120), 360);
  } else if (cmax == b) {
    Hue = fmod((60 * ((r - g) / delta) + 240), 360);
  }
}

void sendcolordata() {
  rgb_to_hsv(colorneo[0], colorneo[1], colorneo[2]);
  mycolor.setSwitch(true);
  mycolor.setHue(Hue);
  mycolor.setSaturation(Saturation);
  mycolor.setBrightness(Value);
}

void oled_display() {
  myOledDisplay.clearDisplay();
  myOledDisplay.drawBitmap(0, 0, bamyanggang_image, 128, 32, 1);
  myOledDisplay.display();
  delay(2000);
  myOledDisplay.clearDisplay();
  myOledDisplay.display();
}
