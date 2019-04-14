// *************************************
// *****  Strobe lights controller *****
// *************************************

// Software and the hardware designed in a such way that only one LED is energised at any one point in time.
// this allows to avoid current surges in the electric system.

// The LeftStrobe and the RightStrobe considered to be primary
// Top and Bottom strobes are driven by the LeftStrobe and the RightStrobe respectively

// These variables store the flash pattern
// and the current state of the LED

// Digital pins associated with the strobe
// nubers can be swapped around if desired.
int LeftStrobe =  2;      // Left Strobe pin 
int RightStrobe =  3;     // Right Strobe pin 
int TopStrobe = 4;        // Top Strobe pin 
int BottomStrobe = 5;     // Bottom Strobe pin 


int WhichStrobe = 2;                     // Temporary variable to identify which strobe is active
int StrobeState = LOW;                   // Current state of the Strobe
unsigned long previousMillis = 0;        // will store last time primary Strobe was updated
unsigned long OtherPreviousMillis = 0;   // will store last time the secondary Strobe was updated


long OnTime = 30;         // milliseconds of on-time. About 1000 for landing lights. About 30 for strobes.
long OffTime = 100;       // milliseconds of off-time. =0 for landing lights, 100-250 fro strobes
int n_pulses = 4;         // Number of pulses in series per light. =1 for landing lights 1..3 for strobe lights
long Pause_between = 500; // pause between series of flashes

int i = 0; // counter
boolean LeftRight = true;  // Current: Left or Right
boolean GapStrobe = false; // Waiting betwwen serieses
unsigned long currentMillis;

// Control pins to modify the behaviour
// Ground the pins and you will get a control mechanism to modify the controllers behaviour
// this is mainly for landing lights, or you can get creative with strobes patterns

void setup() 
{
   Serial.begin(115200);    
  // set the digital pin as output:
  pinMode(LeftStrobe, OUTPUT);      
  pinMode(RightStrobe, OUTPUT);     
  pinMode(TopStrobe, OUTPUT);      
  pinMode(BottomStrobe, OUTPUT);   
}

void loop()
{
  
 currentMillis = millis();
 
 if (GapStrobe == true && (currentMillis - previousMillis >= Pause_between)) {
   GapStrobe = false;
 }

// Beginning of the block for left and right strobes 
 if ((StrobeState == HIGH) && (currentMillis - previousMillis >= OnTime)) 
  {
    StrobeState = LOW;
    previousMillis = currentMillis;  // Remember the time
    OtherPreviousMillis = currentMillis; // this one is for timekeeping for the Top and Bottom strobes
    digitalWrite(WhichStrobe, StrobeState);  // Update the actual LED
     if (i >= n_pulses) {
      i = 0;
      LeftRight = !LeftRight;
      GapStrobe = true; 
      if (LeftRight == true) {
        WhichStrobe = LeftStrobe;
      } else {
        WhichStrobe = RightStrobe;
      }
    }
  }

  // turn the LED on
  if ((StrobeState == LOW) && (currentMillis - previousMillis >= OffTime) && GapStrobe == false)
  {
    previousMillis = currentMillis;   // Remember the time
    StrobeState = HIGH;
    digitalWrite(WhichStrobe, StrobeState);
    i++;
  }
// The end of the Left/Right block


// Control the top and bottom

    if (WhichStrobe == LeftStrobe && StrobeState == HIGH) {
      digitalWrite(TopStrobe, LOW);
    }

    if (WhichStrobe == LeftStrobe && StrobeState == LOW && (currentMillis - OtherPreviousMillis < OnTime)) {
         // Remember the time
      digitalWrite(TopStrobe, HIGH);
    } else {
      digitalWrite(TopStrobe, LOW);
    }
    

    if (WhichStrobe == RightStrobe && StrobeState == HIGH) {
      digitalWrite(BottomStrobe, LOW);
    }

    if (WhichStrobe == RightStrobe && StrobeState == LOW && (currentMillis - OtherPreviousMillis < OnTime)) {
      digitalWrite(BottomStrobe, HIGH);
    }  else {
      digitalWrite(BottomStrobe, LOW);
    }
}
