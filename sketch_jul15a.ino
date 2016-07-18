#include <Multiservo.h>
#include <Wire.h>

#define DEFAULT_DELAY 1000
#define STOP 90

// These predeterminations are using for clarifying all pins in one spot: 

#define conveyorButtonPin 0 
#define liftUpButtonPin 1 
#define liftDownButtonPin 2 
#define controlButtonPin 3 
#define alertButtonPin 4

#define conveyorServoPin 1 
#define initialGrabServoPin 2 
#define liftServoPin 3 
#define secondaryGrabServoPin 4 
#define railServoPin 5

		//////////////////////////////////////////////////////////////////////
		// * * * * * * * * * * VARIABLES DESCRIPTION * * * * * * * * * * *  //
		//
		// I. SERVOMOTORS:
		// conveyorServo - conveyor
		// initialGrabServo - "initial" grab
		// liftServo - lift
		// secondaryGrabServo - "secondary"	grab
		// railServo - the guide rail's "move out"
		//
		// II. BUTTONS (pins):
		// conveyorButton - limit switch for conveyor
		// liftUpButton - limit switch for lift - upper part
		// liftDownButton - limit switch for lift - bottom part
		// controlButton - initiate next action manually
		// alertButton - notice about moving of the device to frame
		//
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  //
		//////////////////////////////////////////////////////////////////////

Multiservo conveyorServo; 
Multiservo initialGrabServo; 
Multiservo liftServo; 
Multiservo secondaryGrabServo; 
Multiservo railServo; 


int conveyorButton = conveyorButtonPin; 
int liftUpButton = liftUpButtonPin; 
int liftDownButton = liftDownButtonPin; 
int controlButton = controlButtonPin; 
int alertButton = alertButtonPin; 


//Just initiate all our servomotors: 
									
void setup (void) {
	conveyorServo.attach(conveyorServoPin);
	initialGrabServo.attach(initialGrabServoPin);
	liftServo.attach(liftServoPin);
	secondaryGrabServo.attach(secondaryGrabServoPin);
	railServo.attach(railServoPin);
}

		////////////////////////////////////////////////////////////////////////////////
		// * * * * * * * * * * LOOP DESCRIPTION * * * * * * * * * * * * * * * * * * * //
		//
		// In general the Loop could be separated in five stages:
		//
		// I. Conveyor: 
		// Some button (seems like control one) determines the start of our process. 
		// The conveyor will work until nearest bag touches corresponding limit switch. 
		//
		// II. Lift: 
		// When nearest bag touches the limit switch, lift will start to fall till another 
		// limit switch stops it. It would be nice if grab mechanism will stop accurately at 
		// our bag. 
		// After grab process lift will move up for moving into framework. 
		//
		// III. Initial grab:
		// When the mechanism disposed at the bag, take in the bag with moving in hooks 
		//
		// IV. Secondary grab: 
		// ...
		//
		// V. Move to the framework: 
		// 
		//
		//
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
		/////////////////////////////////////////////////////////////////////////////////

void loop (void) {
	
	if (digitalRead(controlButton) == HIGH)  {
		do    {
			delay(50); // Stabilising
			conveyorServo.write (180); // Conveyor's initiate 
		}
		while (digitalRead(conveyorButton) == LOW); // Work till the button's press
		conveyorServo.write(STOP);
		delay(DEFAULT_DELAY);
	} 
	else	{
		return;
	}
  


label2:
	if (digitalRead(controlButton == HIGH))
	{
		do {
			liftServo.write(0); // Move down
		} 
		while (liftDownButton != HIGH); // Again we're waiting for limit switch
	
		liftServo.write (STOP);
		delay(DEFAULT_DELAY);
	} 
	/*else {
		goto label2;
	}*/

	
	initialGrabServo.write(180); // Initiate grab - do we need to check the succcess?? 

	delay (DEFAULT_DELAY);

	
	do	{
		liftServo.write (180); // Lift bag up till...
	} 
	while (liftUpButton != HIGH); // ... till the limit switch's press
	liftServo.write(STOP);
	delay (DEFAULT_DELAY);
	
	initialGrabServo.write (100); // Spread out eyelets
	delay (DEFAULT_DELAY);
	
	liftServo.write (180); // Again lift up
	delay (3000);
	liftServo.write (STOP); // Stop
	
	initialGrabServo.write (80); // Budge the eyelets
	liftServo.write (0); // Lift down within 3 seconds
	liftServo.write (STOP); // Stop
	initialGrabServo.write (180); // Spread out the grab
	
	do {
		liftServo.write (180); // Lift up the mechanism 
	} 
	while (liftUpButton != HIGH);
	liftServo.write(STOP);
	delay (DEFAULT_DELAY);

	
	// - - - Penetration into the framework: - - - 
	
	secondaryGrabServo.write (180); // Initiate the mechanism of bag's move in
	delay(DEFAULT_DELAY);
	do	{
		railServo.write (180);
	} while (digitalRead(alertButton == HIGH)); // Limit switch is on?? 
	delay (DEFAULT_DELAY); 
	// Is mechanism in the framework? 

	
	// - - - Transfer eyelets from the mechanism to framework: - - - 
	
	secondaryGrabServo.write (0);
	delay (DEFAULT_DELAY);
	// Is bag in framework? 

	railServo.write (0); // Move out from the framework (within 5 seconds)
	delay (5000);
	railServo.write (STOP); // Set up initial position

}

