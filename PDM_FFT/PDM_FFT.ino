#include <PDM.h>                    //to get microphone input
#include <arduinoFFT.h>             //for the Fourier transform

#define SAMPLES 256                 //Must be a power of 2 (change at all)
#define SAMPLING_FREQUENCY 10000    //figure how frequency of squeak

short sampleBuffer[SAMPLES];
volatile int samplesRead;

double vReal[SAMPLES];
double vImag[SAMPLES];

arduinoFFT FFT = arduinoFFT();

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
}

void loop() {
  if(samplesRead) {
    for (int i = 0; i < SAMPLES; i++) {
      vReal[i] = sampleBuffer[i];
      vImag[i] = 0;
    }
    
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    Serial.print(0);    // To freeze the lower limit
    Serial.print(" ");
    Serial.print(5000); // To freeze the upper limit
    Serial.print(" ");
    Serial.println(peak);
    if (peak <=600)
      lightOne();
    if (peak >600 && peak < 1200)
      lightTwo();
    if (peak >= 1200)
      lightThree();
    samplesRead = 0;
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
