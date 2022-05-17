/***********************************************************
 * Authors: Sadie Thomas and Emma Koehmstedt
 * Description: Rotates a motor to release baseballs into 
 *    pitching machine based on button/rf input and 
 *    coordinates LED indicator lights
 * Inputs: audio, button
 * Outputs: Serial monitor, LED colors
 * References: PDM + FFT code - Juna Salviati
 * Components: Arduino Nano Sense, toggle button
 * Date: 5/14/2022
 ***********************************************************/
 
#include <PDM.h>                    //to get microphone input
#include <arduinoFFT.h>             //for the Fourier transform


/***PDM variables***/
#define SAMPLES 256                 //Must be a power of 2 (change at all)
#define SAMPLING_FREQUENCY 12000    //figure how frequency of squeak - max 6kHz

short sample_buffer[SAMPLES];
volatile int samples_read;


/***FFT variables***/
double vReal[SAMPLES];
double vImag[SAMPLES];

arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQUENCY);


/***interaction variables***/
#define BUTTON_PIN 2

#define RED_LED 22
#define GREEN_LED 23
#define BLUE_LED 24


/***function prototypes***/
void light(int, int, int);
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
  
  pinMode(BUTTON_PIN, INPUT);
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED , OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  light(HIGH, HIGH, HIGH);
}

void loop() {
  if(samples_read) {
    //setup FFT arrays
    for (int i=0; i<SAMPLES; i++) {
      vReal[i] = sample_buffer[i];
      vImag[i] = 0;
    }

    //perform FFT
    FFT.Windowing(FFT_WIN_TYP_FLT_TOP, FFT_FORWARD);    //srt consider using FFT_WIN_TYP_FLT_TOP (specialized) vs. FFT_WIN_TYP_HAMMING (standard)
    FFT.Compute(FFT_FORWARD);
    FFT.ComplexToMagnitude();
    double peak = FFT.MajorPeak();
    
    //srt mess with these numbers so they're more helpful
    if (peak <= 600){
      light(LOW, HIGH, HIGH);
    }
    else if (peak >600 && peak < 1200){
      light(HIGH, LOW, HIGH);
    }
    else{
      light(HIGH, HIGH, LOW);
    }

    //only print peak if we are recording
    int button = digitalRead(BUTTON_PIN);
    Serial.print("Button: ");
    Serial.println(button);
    if(digitalRead(BUTTON_PIN)){
      Serial.println(peak);
    }
    
    samples_read = 0;
  }
}


void light(int r, int g, int b){
  digitalWrite(RED_LED, r);
  digitalWrite(GREEN_LED, g);
  digitalWrite(BLUE_LED, b);
}


void onPDMdata(){
  int bytes_available = PDM.available();
  PDM.read(sample_buffer, bytes_available);
  samples_read = bytes_available / 2;
}
