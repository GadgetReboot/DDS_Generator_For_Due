//
// DDS Sine Generator for Arduino Due
// Based on examples for Due timers here: http://rcarduino.blogspot.com/2012/12/arduino-due-dds-part-1-sinewaves-and.html
//
// Generates sine waves at chosen frequency using a 44100 Hz Sample Rate
//
// DDS generator equation:   fOut = (M (sampleRate)) / (2^32)
//
//              where        fOut = target output frequency
//                              M = tuningWord
//                     sampleRate = 44100 Hz (one sample every 22.675737 uSec)
//                           2^32 = highest count of a 32 bit phase accumulator
//
// Gadget Reboot

#define samplesPerCycle 512

// data table for one cycle of sine wave
int sineData[samplesPerCycle]  = {
  2048, 2073, 2098, 2123, 2148, 2173, 2198, 2223, 2248, 2273, 2298, 2323, 2348, 2373, 2398, 2422, 2447, 2472, 2496, 2521, 2545, 2569, 2594, 2618, 2642, 2666, 2690, 2714, 2737, 2761, 2784, 2808, 2831, 2854, 2877, 2900, 2923, 2946, 2968, 2990, 3013, 3035, 3057, 3078, 3100, 3122, 3143, 3164, 3185, 3206, 3226, 3247, 3267, 3287, 3307, 3327, 3346, 3366, 3385, 3404, 3423, 3441, 3459, 3477, 3495, 3513, 3530, 3548, 3565, 3581, 3598, 3614, 3630, 3646, 3662, 3677, 3692, 3707, 3722, 3736, 3750, 3764, 3777, 3791, 3804, 3816, 3829, 3841, 3853, 3865, 3876, 3888, 3898, 3909, 3919, 3929, 3939, 3949, 3958, 3967,
  3975, 3984, 3992, 3999, 4007, 4014, 4021, 4027, 4034, 4040, 4045, 4051, 4056, 4060, 4065, 4069, 4073, 4076, 4080, 4083, 4085, 4087, 4089, 4091, 4093, 4094, 4094, 4095, 4095, 4095, 4094, 4094, 4093, 4091, 4089, 4087, 4085, 4083, 4080, 4076, 4073, 4069, 4065, 4060, 4056, 4051, 4045, 4040, 4034, 4027, 4021, 4014, 4007, 3999, 3992, 3984, 3975, 3967, 3958, 3949, 3939, 3929, 3919, 3909, 3898, 3888, 3876, 3865, 3853, 3841, 3829, 3816, 3804, 3791, 3777, 3764, 3750, 3736, 3722, 3707, 3692, 3677, 3662, 3646, 3630, 3614, 3598, 3581, 3565, 3548, 3530, 3513, 3495, 3477, 3459, 3441, 3423, 3404, 3385, 3366,
  3346, 3327, 3307, 3287, 3267, 3247, 3226, 3206, 3185, 3164, 3143, 3122, 3100, 3078, 3057, 3035, 3013, 2990, 2968, 2946, 2923, 2900, 2877, 2854, 2831, 2808, 2784, 2761, 2737, 2714, 2690, 2666, 2642, 2618, 2594, 2569, 2545, 2521, 2496, 2472, 2447, 2422, 2398, 2373, 2348, 2323, 2298, 2273, 2248, 2223, 2198, 2173, 2148, 2123, 2098, 2073, 2048, 2022, 1997, 1972, 1947, 1922, 1897, 1872, 1847, 1822, 1797, 1772, 1747, 1722, 1697, 1673, 1648, 1623, 1599, 1574, 1550, 1526, 1501, 1477, 1453, 1429, 1405, 1381, 1358, 1334, 1311, 1287, 1264, 1241, 1218, 1195, 1172, 1149, 1127, 1105, 1082, 1060, 1038, 1017,
  995, 973, 952, 931, 910, 889, 869, 848, 828, 808, 788, 768, 749, 729, 710, 691, 672, 654, 636, 618, 600, 582, 565, 547, 530, 514, 497, 481, 465, 449, 433, 418, 403, 388, 373, 359, 345, 331, 318, 304, 291, 279, 266, 254, 242, 230, 219, 207, 197, 186, 176, 166, 156, 146, 137, 128, 120, 111, 103, 96, 88, 81, 74, 68, 61, 55, 50, 44, 39, 35, 30, 26, 22, 19, 15, 12, 10, 8, 6, 4, 2, 1, 1, 0, 0, 0, 1, 1, 2, 4, 6, 8, 10, 12, 15, 19, 22, 26, 30, 35,
  39, 44, 50, 55, 61, 68, 74, 81, 88, 96, 103, 111, 120, 128, 137, 146, 156, 166, 176, 186, 197, 207, 219, 230, 242, 254, 266, 279, 291, 304, 318, 331, 345, 359, 373, 388, 403, 418, 433, 449, 465, 481, 497, 514, 530, 547, 565, 582, 600, 618, 636, 654, 672, 691, 710, 729, 749, 768, 788, 808, 828, 848, 869, 889, 910, 931, 952, 973, 995, 1017, 1038, 1060, 1082, 1105, 1127, 1149, 1172, 1195, 1218, 1241, 1264, 1287, 1311, 1334, 1358, 1381, 1405, 1429, 1453, 1477, 1501, 1526, 1550, 1574, 1599, 1623, 1648, 1673, 1697, 1722,
  1747, 1772, 1797, 1822, 1847, 1872, 1897, 1922, 1947, 1972, 1997, 2022
};

const float sampleRate = 44100.0;    // sample rate 44.1KHz
float fOut;                          // target output frequency (Hz)

volatile int phInc;                  // dds phase increment
volatile unsigned long phAcc;        // dds phase accumulator
unsigned long tuningWord;            // dds tuning word (M)

// tableAddrWidth is number of bits needed to hold an index number that can
// address all samples in the sinewave data table.
// eg. if table contains 512 samples addressable from 0 to 511,
// 511 dec = 0b111111111 = 9 bit wide index number needed
// if # of data samples is a multiple of 2^n eg 256, 512, 1024,
// the index value will always point to an existing table entry so
// the index can cycle through, wrap around, start over, and always
// land on a sine wave sample.
// if the number of samples is not a multiple of 2^n,
// more steps will be required to handle accessing the data table
// otherwise the index may point beyond the table
// eg if there are 400 samples (index 0 through 399) and the index
// must be 9 bits to store the number 399, if the index is set to
// 400 through 511, it is outside the data table range
byte tableAddrWidth;

void setup()
{
  // to access the highest data sample in the table,
  // how many bits are needed to count up to that
  // index position? eg. 511 would be a 9 bit number
  tableAddrWidth = numBits(samplesPerCycle - 1);  // (samples -1) because we index from 0 to (samples -1)

  fOut = 1000.0;                                  // set output frequency in Hz
  tuningWord = pow(2, 32) * fOut / sampleRate;    // DDS tuning word for target frequency


  // configure timers/interrupts for a 44100 sample rate
  // interrupt handler will send a data sample to the DAC at that sample rate
  // configuration obtained from http://rcarduino.blogspot.com/2012/12/arduino-due-dds-part-1-sinewaves-and.html

  // turn on the timer clock in the power management controller
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC4);

  // we want wavesel 01 with RC
  TC_Configure(TC1, 1, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC1, 1, 238); // sets <> 44.1 Khz interrupt rate
  TC_Start(TC1, 1);

  // enable timer interrupts on the timer
  TC1->TC_CHANNEL[1].TC_IER = TC_IER_CPCS;
  TC1->TC_CHANNEL[1].TC_IDR = ~TC_IER_CPCS;

  // Enable the interrupt in the nested vector interrupt controller
  // TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number (=(1*3)+1) for timer1 channel1
  NVIC_EnableIRQ(TC4_IRQn);

  // enable the DAC
  analogWrite(DAC0, 0);
}

void loop()
{

  // examples of setting various frequencies and calculating the required tuning word for that frequency
  // frequency is fOut, in Hz and is floating point
  // the frequency and tuning word only need to be set once for each change in frequency.
  // then the interrupt handler will automatically generate the new frequency
  // at 44.1KHz, the max frequency should probably be 20KHz, and it will only contain 2 to 3 data points per cycle
  // but it is still 20 KHz

  //fOut = 0.42;
  //tuningWord = pow(2, 32) * fOut / sampleRate;
  //delay(5000);

  //fOut = 555.555;
  //tuningWord = pow(2, 32) * fOut / sampleRate;
  //delay(5000);

  //fOut = 2112;
  //tuningWord = pow(2, 32) * fOut / sampleRate;
  //delay(5000);

  //fOut = 10000;
  //tuningWord = pow(2, 32) * fOut / sampleRate;
  //delay(5000);

  //fOut = 20000;
  //tuningWord = pow(2, 32) * fOut / sampleRate;
  //delay(5000);

  // sweep frequency up and down from 0 to 4KHz
  for (int i = 0; i < 4000; i++) {
    fOut = i;                                     // set output frequency in Hz
    tuningWord = pow(2, 32) * fOut / sampleRate;  // DDS tuning word for target frequency
    delay(5);
  }

  for (int i = 4000; i > 0; i--) {
    fOut = i;                                     // set output frequency in Hz
    tuningWord = pow(2, 32) * fOut / sampleRate;  // DDS tuning word for target frequency
    delay(5);
  }


}

void TC4_Handler()
{
  // we need to get the status to clear it and allow the interrupt to fire again
  TC_GetStatus(TC1, 1);

  // increment 32 bit phase accumulator by the tuning word amount, then
  // use the (tableAddrWidth) number of most significant bits of the phase
  // accumulator as a phase increment, which is the index position in the sine 
  // data table to get a sample from
  // the phase accumulator is always incrementing and when it reaches the maximum 
  // number possible in 32 bits, it rolls over and continues accumulating from 0, so
  // the phase increment (index pointer) is also continuously incrementing by some 
  // amount based on target frequency and rolling over to continue accessing sine table
  // data at the required intervals to generate a sine wave at the desired frequency.
  phAcc += tuningWord;
  phInc = phAcc >> (32 - tableAddrWidth);

  // send current sine data sample to the DAC
  dacc_write_conversion_data(DACC_INTERFACE, sineData[phInc]);

}

// returns the number of bits needed to store the number being tested
// eg 4 decimal is 0b100, which requires 3 bits 
// adapted from SirNickity:   https://forum.arduino.cc/index.php?topic=237306.0
uint8_t numBits(uint32_t num) {
  uint8_t width = 32;                // maximum bit width of number being tested

  while (width) {
    width--;
    if (num & (1 << width)) break;   // look for bit position of first "1" from MSB toward LSB
  }
  return (width + 1);
}
