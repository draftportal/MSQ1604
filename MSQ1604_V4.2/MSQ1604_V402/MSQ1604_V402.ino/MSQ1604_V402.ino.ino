#include <MIDI.h>                    // Using midi lib 4.2
#include <LiquidCrystal.h>           // Using LCD lib
#include <Encoder_Polling.h>         // Library for reading the encoder

MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiTXtwo);   // midi initialised ont TX2
MIDI_CREATE_INSTANCE(HardwareSerial, Serial3, midiTXthree); // midi initialised onto TX3

//___  ___ _____  _____  __    ____ _____    ___           ___  ______________ _____   _____ _____ ___________   _____ _____ _____ _   _ _____ _   _ _____  ___________  
//|  \/  |/  ___||  _  |/  |  / ___|  _  |  /   |          |  \/  |_   _|  _  \_   _| /  ___|_   _|  ___| ___ \ /  ___|  ___|  _  | | | |  ___| \ | /  __ \|  ___| ___ \ 
//| .  . |\ `--. | | | |`| | / /___| |/' | / /| |  ______  | .  . | | | | | | | | |   \ `--.  | | | |__ | |_/ / \ `--.| |__ | | | | | | | |__ |  \| | /  \/| |__ | |_/ /
//| |\/| | `--. \| | | | | | | ___ \  /| |/ /_| | |______| | |\/| | | | | | | | | |    `--. \ | | |  __||  __/   `--. \  __|| | | | | | |  __|| . ` | |    |  __||    / 
//| |  | |/\__/ /\ \/' /_| |_| \_/ \ |_/ /\___  |          | |  | |_| |_| |/ / _| |_  /\__/ / | | | |___| |     /\__/ / |___\ \/' / |_| | |___| |\  | \__/\| |___| |\ \ 
//\_|  |_/\____/  \_/\_\\___/\_____/\___/     |_/          \_|  |_/\___/|___/  \___/  \____/  \_/ \____/\_|     \____/\____/ \_/\_\\___/\____/\_| \_/\____/\____/\_| \_|
//



////////////////////////////////////////---MIDI SEQUENCER - MSQ1604---\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//                                                                                                                                                                                        
//     Project By: Chris Wojcicki                                                            
//       *  16 Step MIDI Sequencer: Tempo, Velocity and Pitch Control. 2015 Vesrion 4.2    10/10/2015
//        + 8 Steps        
//        + Veriable Step Length (not tempo) 
//        + LED Showing Current Step    
//        + Adding velocity and note selection through pot                   28/05/2015
//        + Basic LCD Display                                                15/06/2015
//        + Start/Stop fuctionality (with external sequencer start/stop)     04/08/2015 
//        + BPM Implemented for tempo                                        10/08/2015 
//        + Midi clock signal implemented                                    14/08/2015 
//        + Reading Rotary Encoder and altering tempo                        18/08/2015 
//        + Rotary Encoder control for both Tempos and midi channel          20/08/2015 
//        + Implemented 16 Steps                                             14/09/2015
//        + 2 Channels implemented                                           17/09/2015
//        + 4 Channels implemented                                           23/09/2015
//        + Midi clock timing improvment                                     10/10/2015
//
///////////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//*************************************************************************************************************\\
///////////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
//                                          Pin Mapping
//
///////////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

  // Analog Pins
    int step1 =A0;         //Pot for Step1 
    int step2 =A1;         //Pot for Step2
    int step3 =A2;         //Pot for Step3
    int step4 =A3;         //Pot for Step4
    int step5 =A4;         //Pot for Step5
    int step6 =A5;         //Pot for Step6
    int step7 =A6;         //Pot for Step7
    int step8 =A7;         //Pot for Step8
    int step9 =A8;         //Pot for Step9 
    int step10 =A9;        //Pot for Step10
    int step11 =A10;       //Pot for Step11
    int step12 =A11;       //Pot for Step12
    int step13 =A12;       //Pot for Step13
    int step14 =A13;       //Pot for Step14
    int step15 =A14;       //Pot for Step15
    int step16 =A15;       //Pot for Step16

  // Digital Pins LEDs
    int led1 =22;             //LED1
    int led2 =23;             //LED2
    int led3 =24;             //LED3
    int led4 =25;             //LED4
    int led5 =26;             //LED5
    int led6 =27;             //LED6
    int led7 =28;             //LED7
    int led8 =29;             //LED8
    int led9 =30;             //LED1
    int led10 =31;            //LED2
    int led11 =32;            //LED3
    int led12 =33;            //LED4
    int led13 =34;            //LED5
    int led14 =35;            //LED6
    int led15 =36;            //LED7
    int led16 =37;            //LED8
  
  // System LEDs
    int ledMIDI =53;          // MIDI Control LED
    int ledTempo =51;         // Tempo Control LED
    int ledVelocity =47;      // Velocity Control LED
    int ledNote = 49;         // Note Control LED
    int ledPlay = 45;         // Play indicator LED
    int Ch1LED = 38;          // Channel 1 LED
    int Ch2LED = 40;          // Channel 2 LED
    int Ch3LED = 42;          // Channel 3 LED
    int Ch4LED = 44;          // Channel 4 LED
   
  // Digital Pins - Channel Control
    int NoteVelPin = 41;                         // Channel velocity/note control selection   
    int StopPlay = 39;                           // Start Stop Seqquencer button
    int TempoMidi = 43;                          // Tempo MIDI Select
    int Ch1Pin = 18;                             // Channel 1 Select
    int Ch2Pin = 19;                             // Channel 2 Select
    int Ch3Pin = 20;                             // Channel 3 Select
    int Ch4Pin = 21;                             // Channel 4 Select
    
  // Code Veriables 
    int TempoDelay;
    int MIDICh1 = 1;                             // Midi channel for channel 1 
    int MIDICh2 = 2;                             // Midi channel for channel 2
    int MIDICh3 = 3;                             // Midi channel for channel 3
    int MIDICh4 = 4;                             // Midi channel for channel 4
    int InSequence = 1;                          // Sequence case pointer
    int ChannelSelect = 0;                       // Channel pointer
    int ChannelPoint = 1;
    
  // Tempo Values
    int BPM = 120;                               // Tempo Setting to 120 default
    float MidiClockDelay;                        // Delay per midi clock signal
    float MidiSystemDelay = 2.8;                 // Delay for midi clock - time taken for system checks
    
  // Code Arrays
    int pot[16] = {step1, step2, step3, step4, step5, step6, step7, step8, step9, step10, step11, step12, step13, step14, step15, step16};     // Pot array
    int led[16] = {led1, led2, led3, led4, led5, led6, led7, led8, led9, led10, led11, led12, led13, led14, led15, led16};                     // LED array
   
  // Channel Arrays 
    int Ch1Note[16] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};                      // Setting initial value for Notes C4
    int Ch1Velo[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                      // Setting initial value for Velocity set to Maximum 

    int Ch2Note[16] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};                      // Setting initial value for Notes C4
    int Ch2Velo[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                      // Setting initial value for Velocity set to Maximum

    int Ch3Note[16] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};                      // Setting initial value for Notes C4
    int Ch3Velo[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                      // Setting initial value for Velocity set to Maximum 

    int Ch4Note[16] = {60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60};                      // Setting initial value for Notes C4
    int Ch4Velo[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                      // Setting initial value for Velocity set to Maximum
        
    int PotLast[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                      // Array for last step 
    int PotCurrent[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                   // Array for the step

    int MidiArray[4] = {MIDICh1, MIDICh2, MIDICh3, MIDICh4};                                                 // Array for midi channel

   // LCD 
    LiquidCrystal lcd(12, 11, 7, 6, 5, 4);                                  // LCD pins
   
   // LCD Char arrays
    char TempoLCD[50];                                                      // Tempo LCD Char Array
    char MIDILCD[50];

   // Rotary Encoder
    const int encoderPin_A = 2;                                             // Encoder PinA
    const int encoderPin_B = 3;                                             // Encoder PinB
    int counter;                                                            // Inisializing Encoder Counter, what is BPM



////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
//                                          Void Setup
//
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
    
void setup() 
{
  
// Midi Setup  
 
  midiTXtwo.begin(1);                                 // Launch MIDI on serial port 2
  midiTXthree.begin(1);                             // Launch MIDI on serial port 3

// LED Output Setup
  pinMode(led1, OUTPUT);                              // Setting led1 to an output
  pinMode(led2, OUTPUT);                              // Setting led2 to an output
  pinMode(led3, OUTPUT);                              // Setting led3 to an output
  pinMode(led4, OUTPUT);                              // Setting led4 to an output
  pinMode(led5, OUTPUT);                              // Setting led5 to an output
  pinMode(led6, OUTPUT);                              // Setting led6 to an output
  pinMode(led7, OUTPUT);                              // Setting led7 to an output
  pinMode(led8, OUTPUT);                              // Setting led8 to an output
  pinMode(led9, OUTPUT);                              // Setting led1 to an output
  pinMode(led10, OUTPUT);                             // Setting led2 to an output
  pinMode(led11, OUTPUT);                             // Setting led3 to an output
  pinMode(led12, OUTPUT);                             // Setting led4 to an output
  pinMode(led13, OUTPUT);                             // Setting led5 to an output
  pinMode(led14, OUTPUT);                             // Setting led6 to an output
  pinMode(led15, OUTPUT);                             // Setting led7 to an output
  pinMode(led16, OUTPUT);                             // Setting led8 to an output  
  
  pinMode(ledMIDI, OUTPUT);                           // MIDI LED to output
  pinMode(ledTempo, OUTPUT);                          // Tempo LED to output
  pinMode(ledNote, OUTPUT);                           // Note LED to output
  pinMode(ledVelocity, OUTPUT);                       // Velocity LED to output
  pinMode(ledPlay, OUTPUT);                           // Play LED to output

  pinMode(Ch1LED, OUTPUT);
  pinMode(Ch2LED, OUTPUT);
  pinMode(Ch3LED, OUTPUT);
  pinMode(Ch4LED, OUTPUT);
  
// Channel Control Setup
  pinMode (NoteVelPin, INPUT);                        // Setting up Channel 1 Note Velocity Selection to an input
  pinMode (StopPlay, INPUT);                          // Start stop the sequencer
  pinMode (TempoMidi, INPUT);                         // Tempo Midi Select
  pinMode (Ch1Pin, INPUT);                            // Channel 1 select
  pinMode (Ch2Pin, INPUT);                            // Channel 2 select
  pinMode (Ch3Pin, INPUT);                            // Channel 3 select
  pinMode (Ch4Pin, INPUT);                            // Channel 4 select
  
 
// LCD Display
  lcd.begin(16, 2);                                  // Start LCD

// Rotary Encoder  
  pinMode (encoderPin_A, INPUT);                     // Setting the first encoder line to an input
  pinMode (encoderPin_B, INPUT);                     // Setting the first encoder line to an input
  encoder_begin(encoderPin_A, encoderPin_B);         // Start the decoder
    
// Setup LCD, Tempo and Systems

  StartUp();                                         // Start Routine for Display and LEDs
  TempoSet(BPM);                                     // Setting BPM and Setep Delays
  chanSelect();                                      // Checking what channel is selected
  Display(BPM);                                      // Displaying Tempo and midi channel
    
}

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
//                                          Main Code
//                                          void_loop
//
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void loop() 
{ 

  // Main Sequencer Code - Switch for in and out of Sequence.
  switch (InSequence)
  {
        case 1:
        // Out of Sequence - standby mode
          digitalWrite(ledPlay, LOW);                              // Play LED is low
          TempoMidiFuction(BPM, MidiArray[ChannelSelect]) ;        // Setting BPM
          Systems(MidiClockDelay);                                 // Systems Veriable (Delay, Clock, Display, System Maintenance)
          VelocitylNoteIndication ();                              // Standby Indication
          chanSelect();                                            // Check Channel selection
          if ((digitalRead(StopPlay) == HIGH))
          {
            InSequence = 2;                                        // Play has been pressed, break to main sequence
            break;
          }
          else    
          {
            InSequence = 1;                                        // Stay in out of sequence
            break;
          }
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      
         case 2:
         // Sending MIDI start
         digitalWrite(ledPlay, HIGH);
         using namespace midi;
         midiTXtwo.sendRealTime(Start);                             // MIDI Start for external sequencers
         midiTXthree.sendRealTime(Start);                             // MIDI Start for external sequencers
         InSequence = 3;
         break;
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\        
         case 3:    
         // in sequence loop        
              for (int x=0; x<16; x++)                                       // Loop for send step[x] information 
              {
                  // steps 1-16
                  chanSelect();                                                 // Check Channel
                  
                  // Clock signal 1
                  using namespace midi;      
                  midiTXtwo.sendRealTime(Clock);
                  midiTXthree.sendRealTime(Clock);      
                  digitalWrite(led[x], HIGH);                                 // Turn LED [x] on
                  
                  // Read Pot and Send signal out to start note             
                  TempoMidiFuction(BPM, MidiArray[ChannelSelect]);            // Tempo Length function.
                  VelocitylNoteSelection (x);                                 // Note/Velocity Selection and Array Fill
                  midiTXtwo.sendNoteOn((Ch1Note[x]-12),Ch1Velo[x],MidiArray[0]);   // Send a Note (pitch [x], velo [x] on  MIDIchanne1)
                  midiTXthree.sendNoteOn(Ch3Note[x],Ch3Velo[x],MidiArray[2]); // Send a Note (pitch [x], velo [x] on  MIDIchanne3)  
                  midiTXtwo.sendNoteOn(Ch2Note[x],Ch2Velo[x],MidiArray[1]);   // Send a Note (pitch [x], velo [x] on  MIDIchanne2)
                  midiTXthree.sendNoteOn(Ch4Note[x],Ch4Velo[x],MidiArray[3]); // Send a Note (pitch [x], velo [x] on  MIDIchanne4)
                  Systems(MidiClockDelay);
                  
                  // Clock signal 2
                  using namespace midi;      
                  midiTXtwo.sendRealTime(Clock);                              // MIDI tempo clock tick
                  midiTXthree.sendRealTime(Clock);
                  Systems(MidiClockDelay);		                                // Systems Veriable (Delay, Clock, Display, System Maintenance)
                  
                  // Clock signal 3
                  using namespace midi;      
                  midiTXtwo.sendRealTime(Clock);
                  midiTXthree.sendRealTime(Clock);
                  Systems(MidiClockDelay);                  
                  
                  // Clock signal 4
                  using namespace midi;      
                  midiTXtwo.sendRealTime(Clock); 
                  midiTXthree.sendRealTime(Clock); 
                  Systems(MidiClockDelay);                 
                  
                  // Clock signal 5
                  using namespace midi;      
                  midiTXtwo.sendRealTime(Clock); 
                  midiTXthree.sendRealTime(Clock); 
                  Systems(MidiClockDelay);
                  
                  // Clock signal 6             
                  using namespace midi;      
                  midiTXtwo.sendRealTime(Clock);
                  midiTXthree.sendRealTime(Clock);

                  //Send signal out to stop note                  
                  midiTXtwo.sendNoteOff((Ch1Note[x]-12),Ch1Velo[x],MidiArray[0]);     // Stop the note 
                  midiTXthree.sendNoteOff(Ch3Note[x],Ch3Velo[x],MidiArray[2]);   
                  midiTXtwo.sendNoteOff(Ch2Note[x],Ch2Velo[x],MidiArray[1]);
                  midiTXthree.sendNoteOff(Ch4Note[x],Ch4Velo[x],MidiArray[3]);  
                  Systems(MidiClockDelay);  
                  digitalWrite(led[x], LOW);                                     // Turn LED [x] off                                     
                  
                  // Check if stopped or Passed Step Limit
                  if ((digitalRead(StopPlay) == LOW))
                    {
                      InSequence = 1;                                             // Play stopped, switch to out of cycle
                      x = 0;
                      using namespace midi;
                      midiTXtwo.sendRealTime(Stop);                               // MIDI Stop 
                      midiTXthree.sendRealTime(Stop);                             // MIDI Stop 
                      break;
                    }
                  if ( x == 16)                                                   // Setting loop point for sequence
                    {
                      x=0;                                                        // Resetting position tracker 'x'
                      InSequence = 3;                                             // looping back to start of sequence
                      break;
                    }
               }
               
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\      
        // default:  // not working....???            
             // error();
             // StartUp(); 
             // InSequence = 1;                                              // Default state, send back to out of sequence
             // break;        
  }
}

 
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
//
//                                          Functions
//
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//  Tempo Set
void TempoSet(int Tempo)
{
      float StepDelay = (60000/(24*Tempo));                   // for midi delay set by tempo
      MidiClockDelay = (StepDelay - MidiSystemDelay);         // Return step length  
}

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Rotary Control for Tempo 

void TempoMidiFuction (int BPMF, int MIDIChnF)                 
{
  
  if (digitalRead(TempoMidi) == LOW) 
  {    
      // Tempo Control
      counter = Encoder_direction(BPMF);                        // Checking Rotation ++ --
    
        if(counter >= 200 )                                     // Maximum limit of tmepo is set to 200
        {
          counter = 200;
        }
      
        if (counter <= 50)                                      // Minimum limit of tempo is set to 50
        {
          counter = 50;
        }     

      digitalWrite(ledMIDI, LOW);                               // LED indication
      digitalWrite(ledTempo, HIGH);
      
      BPM = counter;                                            // Making counter = BPM
      float StepDelay = (60000/(24*BPM));                       // for midi delay set by tempo
      MidiClockDelay = (StepDelay - MidiSystemDelay);           // Return step length  
  }  
  
    if (digitalRead(TempoMidi) == HIGH) 
  {    
      // Tempo Control
      counter = Encoder_direction(MIDIChnF);                    // Checking Rotation ++ --
    
        if(counter >= 16 )                                      // Maximum limit of tmepo is set to 200
        {
          counter = 16;
        }
      
        if (counter <= 1)                                       // Minimum limit of tempo is set to 50
        {
          counter = 1;
        }     

      digitalWrite(ledTempo, LOW);                              // LED indication
      digitalWrite(ledMIDI, HIGH);
      
      MidiArray[ChannelSelect] = counter;                       // Making counter = BPM 
  }  
}

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Rotary Encoder Direction Functions

int Encoder_direction(int Counter)
{
  int dir = encoder_data();                                      // Checking Rotation of encoder
  
  if(dir == 1)                                                   // Forward rotation
  {
    Counter++;                                                   // Increment the Counter
  }
  
  else if(dir == -1)                                             // Backward rotation
  {
    Counter--;                                                   // Decrement the counter
  }
  
  return Counter;                                                // Returning counter value
}

///////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
// Velocity/Note Selection 
	
void VelocitylNoteSelection (int x)
{
  switch (ChannelSelect)
  {
    // Channel 1
    case 0:
      if (digitalRead(NoteVelPin) == HIGH)                             // Velocit Control
      {
    
        digitalWrite(ledVelocity, HIGH);
        digitalWrite(ledNote, LOW);
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int VelocityScaled = map (PotCurrent[x], 0, 1023, 0, 127);    // Mapping pot value to Velocity int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch1Velo[x] = Ch1Velo[x];                                    // Not Moved 
        }
        else
        {
          Ch1Velo[x] = VelocityScaled;                                // Writing scaled value to Velocity array
        }
        PotLast[x] = PotCurrent[x] ;                                  // Setting Last Step
       }
      if (digitalRead(NoteVelPin) == LOW)                              // Note Control
      {
    
        digitalWrite(ledVelocity, LOW);
        digitalWrite(ledNote, HIGH);    
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int NoteScaled = map (PotCurrent[x], 0, 1023, 60, 71);        // Mapping pot value to Note int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch1Note[x] = Ch1Note[x];                                    // Not Moved 
        }
        else
        {
          Ch1Note[x] = NoteScaled;                                    // Writing scaled value to Note array
        }
        PotLast[x] = PotCurrent[x];                                   // Setting Last Step
      }
      
     break;

    // Channel 2
    case 1:
    
      if (digitalRead(NoteVelPin) == HIGH)                             // Velocit Control
      {
    
        digitalWrite(ledVelocity, HIGH);
        digitalWrite(ledNote, LOW);
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int VelocityScaled = map (PotCurrent[x], 0, 1023, 0, 127);    // Mapping pot value to Velocity int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch2Velo[x] = Ch2Velo[x];                                    // Not Moved 
        }
        else
        {
          Ch2Velo[x] = VelocityScaled;                                // Writing scaled value to Velocity array
        }
        PotLast[x] = PotCurrent[x] ;                                  // Setting Last Step
      }
      
      if (digitalRead(NoteVelPin) == LOW)                              // Note Control
      {
    
        digitalWrite(ledVelocity, LOW);
        digitalWrite(ledNote, HIGH);    
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int NoteScaled = map (PotCurrent[x], 0, 1023, 60, 71);        // Mapping pot value to Note int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch2Note[x] = Ch2Note[x];                                    // Not Moved 
        }
        else
        {
          Ch2Note[x] = NoteScaled;                                    // Writing scaled value to Note array
        }
        PotLast[x] = PotCurrent[x];                                   // Setting Last Step
      }
     break;

    // Channel 3
    case 2:
      if (digitalRead(NoteVelPin) == HIGH)                             // Velocit Control
      {
    
        digitalWrite(ledVelocity, HIGH);
        digitalWrite(ledNote, LOW);
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int VelocityScaled = map (PotCurrent[x], 0, 1023, 0, 127);    // Mapping pot value to Velocity int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch3Velo[x] = Ch3Velo[x];                                    // Not Moved 
        }
        else
        {
          Ch3Velo[x] = VelocityScaled;                                // Writing scaled value to Velocity array
        }
        PotLast[x] = PotCurrent[x] ;                                  // Setting Last Step
       }
      if (digitalRead(NoteVelPin) == LOW)                              // Note Control
      {
    
        digitalWrite(ledVelocity, LOW);
        digitalWrite(ledNote, HIGH);    
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int NoteScaled = map (PotCurrent[x], 0, 1023, 60, 71);        // Mapping pot value to Note int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch3Note[x] = Ch3Note[x];                                    // Not Moved 
        }
        else
        {
          Ch3Note[x] = NoteScaled;                                    // Writing scaled value to Note array
        }
        PotLast[x] = PotCurrent[x];                                   // Setting Last Step
      }
      
     break;

    // Channel 4
    case 3:
      if (digitalRead(NoteVelPin) == HIGH)                             // Velocit Control
      {
    
        digitalWrite(ledVelocity, HIGH);
        digitalWrite(ledNote, LOW);
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int VelocityScaled = map (PotCurrent[x], 0, 1023, 0, 127);    // Mapping pot value to Velocity int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch4Velo[x] = Ch4Velo[x];                                    // Not Moved 
        }
        else
        {
          Ch4Velo[x] = VelocityScaled;                                // Writing scaled value to Velocity array
        }
        PotLast[x] = PotCurrent[x] ;                                  // Setting Last Step
       }
      if (digitalRead(NoteVelPin) == LOW)                              // Note Control
      {
    
        digitalWrite(ledVelocity, LOW);
        digitalWrite(ledNote, HIGH);    
    
        PotCurrent[x] = analogRead(pot[x]);                           // Reading tempo analog input
        int NoteScaled = map (PotCurrent[x], 0, 1023, 60, 71);        // Mapping pot value to Note int
        PotCurrent[x] = map (PotCurrent[x], 0, 1023, 0, 50);
        if (PotCurrent[x] == PotLast[x])                              // if statement compairing Position of pot and checking if moved
        {
          Ch4Note[x] = Ch4Note[x];                                    // Not Moved 
        }
        else
        {
          Ch4Note[x] = NoteScaled;                                    // Writing scaled value to Note array
        }
        PotLast[x] = PotCurrent[x];                                   // Setting Last Step
      }
      
     break;
  } 
}

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Velocity Note LED indication

void VelocitylNoteIndication()
{
  if (digitalRead(NoteVelPin) == HIGH)                             // Velocit Control
  {
    
    digitalWrite(ledVelocity, HIGH);
    digitalWrite(ledNote, LOW);
    
  }
  if (digitalRead(NoteVelPin) == LOW)                              // Note Control
  {
    
    digitalWrite(ledVelocity, LOW);
    digitalWrite(ledNote, HIGH);    
  } 
}

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Systems Function

void Systems(float stepDelay1)
  {
    Display(BPM);                                                 // Display Function
    delay(stepDelay1);                                        // Tempo Delay (milliseconds)
  }
  
////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Display Function

void Display (int tempo)
  {
    sprintf(TempoLCD, "%3d", tempo);
    lcd.setCursor(7, 1);	                                        // Set Cursor for tempo
    lcd.print(TempoLCD);                                          // Print Tempo

    sprintf(MIDILCD, "%2d", MidiArray[ChannelSelect]);
    lcd.setCursor(14, 0);	                                        // Set Cursor for MIDI Channel	          
    lcd.print(MIDILCD);                                           // Print MIDI Channel 
  }

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Error message

void error()
{
  lcd.setCursor(0, 0);	
  lcd.print("                ");
  lcd.setCursor(0, 1);	
  lcd.print("ERROR!");
  lcd.setCursor(0, 0);	
  lcd.print("****************");
  delay(100);
  lcd.setCursor(0, 0);	
  lcd.print("                ");
  lcd.setCursor(0, 1);	
  lcd.print("                ");
}

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// StartUp

void StartUp()
  {
    lcd.print("MSQ1604");	
    lcd.setCursor(0, 1);	
    lcd.print("MIDI Sequencer");
    delay(2000);	
  
  LEDStartUp();                                                   // LED Start Up display
  
    for (int positionCounter = 0; positionCounter < 15; positionCounter++) 
    {
    lcd.scrollDisplayLeft();                                      // Scroll one position left
    delay(100);	                                                  // Delay
    }
  
  LEDStartUp();                                                   // LED Start Up Display
  
  // Setting Fixed Titles LCD 
  lcd.setCursor(0, 0);	
  lcd.print("                ");
  lcd.setCursor(0, 1);	
  lcd.print("                ");
  lcd.setCursor(0, 1);	
  lcd.print("Tempo:");
  lcd.setCursor(0, 0);	
  lcd.print("Midi Channel:");
  
      for (int positionCounter = 0; positionCounter < 15; positionCounter++) 
    {
    lcd.scrollDisplayRight();                                     // Scroll one position left
    delay(100);	                                                  // Delay
    }
  }

////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// LED Start Up display

void LEDStartUp ()
  {
    for (int x=0; x<16; x++)                                      // Turne LEDs ON 1-16
    {
      digitalWrite(led[x], HIGH);
      delay(50);
    }

    digitalWrite(ledMIDI, HIGH);
    delay(50);
    digitalWrite(ledTempo, HIGH);    
    delay(50);     
    digitalWrite(ledVelocity,HIGH);  
    delay(50);    
    digitalWrite(ledNote,HIGH);
    delay(50);                
    digitalWrite(Ch1LED,HIGH);   
    delay(50);      
    digitalWrite(Ch2LED,HIGH);
    delay(50);          
    digitalWrite(Ch3LED,HIGH);
    delay(50);          
    digitalWrite(Ch4LED,HIGH);
    delay(50); 
    
    for (int x=0; x<16; x++)                                      // Turne LEDs OFF 1-16
    {
      digitalWrite(led[x], LOW);
      delay(50);
    }
    
    digitalWrite(ledMIDI, LOW);
    delay(50);
    digitalWrite(ledTempo, LOW);    
    delay(50);     
    digitalWrite(ledVelocity,LOW);  
    delay(50);    
    digitalWrite(ledNote,LOW);
    delay(50);                
    digitalWrite(Ch1LED,LOW);   
    delay(50);      
    digitalWrite(Ch2LED,LOW);
    delay(50);          
    digitalWrite(Ch3LED,LOW);
    delay(50);          
    digitalWrite(Ch4LED,LOW);
    delay(50);    
  }

 ////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

// Channel Select

void chanSelect()
{

  //Channel 1 Conditions
  if ((digitalRead(Ch1Pin) == HIGH))
  {
    ChannelSelect = 0;
    //ChannelPoint = 1;
    digitalWrite(Ch1LED, HIGH);
    digitalWrite(Ch2LED, LOW);
    digitalWrite(Ch3LED, LOW);
    digitalWrite(Ch4LED, LOW);
  }

  // Channel 2 Conditions
   else if ((digitalRead(Ch2Pin) == HIGH))
  {
    ChannelSelect = 1;
    //ChannelPoint = 2;
    digitalWrite(Ch1LED, LOW);
    digitalWrite(Ch2LED, HIGH);
    digitalWrite(Ch3LED, LOW);
    digitalWrite(Ch4LED, LOW);
  }

  // Channel 3 Conditions
   else if ((digitalRead(Ch3Pin) == HIGH))
  {
    ChannelSelect = 2;
    //ChannelPoint = 3;
    digitalWrite(Ch1LED, LOW);
    digitalWrite(Ch2LED, LOW);
    digitalWrite(Ch3LED, HIGH);
    digitalWrite(Ch4LED, LOW);
  }

  // Channel 4 Conditions
    else if ((digitalRead(Ch4Pin) == HIGH))
  {
    ChannelSelect = 3;
    //ChannelPoint = 4;
    digitalWrite(Ch1LED, LOW);
    digitalWrite(Ch2LED, LOW);
    digitalWrite(Ch3LED, LOW);
    digitalWrite(Ch4LED, HIGH);
  }
}

