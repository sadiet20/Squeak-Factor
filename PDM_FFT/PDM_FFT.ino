#include <PDM.h>                    //to get microphone input
#include <arduinoFFT.h>             //for the Fourier transform

#define SAMPLES 256                 //Must be a power of 2 (change at all)
#define SAMPLING_FREQUENCY 10000    //figure how frequency of squeak - max 6kHz

short sampleBuffer[SAMPLES];
volatile int samplesRead;

int max_freq = 0;
int corrected_max_freq = 0;
const float correction = 1.591;
unsigned long start_time;

double vReal[SAMPLES];
double vImag[SAMPLES];

arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);

const int redLed = 22;    //red
const int greenLed = 23;  //green
const int blueLed = 24;   //blue


void onPDMdata(void);

void setup() {
  Serial.begin(115200);
  while(!Serial);              //wait for serial monitor to connect
  
  PDM.onReceive(onPDMdata);
  PDM.setBufferSize(SAMPLES);
  if (!PDM.begin(1, 16000)) {
    Serial.println("Failed to start PDM!");
    while(1);
  }
  
  pinMode(redLed, OUTPUT);
  pinMode(greenLed , OUTPUT);
  pinMode(blueLed, OUTPUT);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);

  start_time = millis();
}

void loop() {
  if(samplesRead) {
    for (int i = 0; i < SAMPLES; i++) {
      vReal[i] = sampleBuffer[i];
      vImag[i] = 0;
    }
    
    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);    //srt consider using FFT_WIN_TYP_FLT_TOP
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();
    
    double peak = FFT.MajorPeak();
    /*
    Serial.print(0);    // To freeze the lower limit
    Serial.print(" ");
    Serial.print(2000); // To freeze the upper limit
    Serial.print(" ");
    Serial.println(peak);
    */
    if (peak <=600)
      lightOne();
    if (peak >600 && peak < 1200)
      lightTwo();
    if (peak >= 1200)
      lightThree();
    samplesRead = 0;

    if(peak > max_freq){
      max_freq = peak;
    }
  }

  //print maximum frequency every 5 seconds
  if(millis() - start_time > 100){
    //corrected_max_freq = max_freq*correction;
    //Serial.print("Corrected max frequency: ");
    //Serial.print(corrected_max_freq);
    Serial.println(max_freq);
    start_time = millis();
    max_freq = 0;
  }
}


void lightOne(){
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, HIGH);
}


void lightTwo(){
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, HIGH);
}


void lightThree() {
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, LOW);
}


void onPDMdata(){
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / 2;
}
