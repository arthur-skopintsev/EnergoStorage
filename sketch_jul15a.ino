#include <Multiservo.h>
#include <Wire.h>

#define DEFAULT_DELAY 1000
#define STOP 90

		//////////////////////////////////////////////////////////////////////
		// * * * * * * * * * * VARIABLES DESCRIPTION * * * * * * * * * * *  //
		//																	//
		// I. SERVOMOTORS:  												//
		// conveyorServo - conveyor											//
		// initialGrabServo - "initial" grab								//
		// liftServo - lift													//
		// secondaryGrabServo - "secondary"	grab							//
		// railServo - the guide rail's "move out"							//
		// 																	//
		// II. BUTTONS (pins): 												//
		// conveyorButton - limit switch for conveyor 						//
		// liftUpButton - limit switch for lift - upper part 				//
		// liftDownButton - limit switch for lift - bottom part 			//
		// controlButton - initiate next action manually 					//
		// alertButton - notice about moving of the device to frame			//
		//																	//
		// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  //
		//////////////////////////////////////////////////////////////////////

Multiservo conveyorServo; 
Multiservo initialGrabServo; 
Multiservo liftServo; 
Multiservo secondaryGrabServo; 
Multiservo railServo; 


int conveyorButton = 0; 
int liftUpButton = 1; 
int liftDownButton = 2; 
int controlButton = 3; 
int alertButton = 4; 


//Just initiate all our servomotors:// 
									//
void setup (void) { 				//
	conveyorServo.attach(1);		//
	initialGrabServo.attach(2);		//
	liftServo.attach(3);			//
	secondaryGrabServo.attach(4);	//
	railServo.attach(5);			//
}									//
									//
//----------------------------------//



void loop (void) {
	
	if (digitalRead(controlButton) == HIGH)  {
		do    {
			delay(50); // wait for stabilize
			conveyorServo.write (180); // запуск конвеера
		}
		while (digitalRead(conveyorButton) == LOW); // работает до тех пор, пока кнопка не нажата
		conveyorServo.write(STOP);// остановить конвеер
		delay(DEFAULT_DELAY);
	} 
	else	{
		return;
	}
  // сделать через goto? label: или switch case


label2:
	if (digitalRead(controlButton == HIGH))
	{
		do {
			liftServo.write(0); // опустить подхват вниз
		} 
		while (liftDownButton != HIGH);
	
		liftServo.write (STOP);
		delay(DEFAULT_DELAY);
	} 
	else {
		goto label2;
	}

	
	initialGrabServo.write(180); // подхват мешка
	//проверять не проверять??? servomotor постоянный или угловой?
	delay (DEFAULT_DELAY);

	
	do	{
		liftServo.write (180); // поднять мешок вверх в течении трех секунд// проверить то, что мешок зацепился???
	} 
	while (liftUpButton != HIGH);
		
	liftServo.write(STOP);
	delay (DEFAULT_DELAY);
	initialGrabServo.write (100); // раздвинул ушки мешка
	delay (DEFAULT_DELAY);
	liftServo.write (180);// еще приподнялся в течении трёх секунд
	delay (3000);
	liftServo.write (STOP); // остановился
	initialGrabServo.write (80);// сдвинул ушки мешка
	liftServo.write (0); //опустился в течении трех секунд
	liftServo.write (STOP);//остановился
	initialGrabServo.write (180); // расправил подхват
	
	do {
		liftServo.write (180); // поднять механизм подхвата вверх
	} 
	while (liftUpButton != HIGH);
	liftServo.write(STOP);
	delay (DEFAULT_DELAY);

	// въезд в раму
	secondaryGrabServo.write (180); //собрали механизм загонки мешка
	delay(DEFAULT_DELAY);
	do	{
		railServo.write (180);
	} while (digitalRead(alertButton == HIGH)); // сработал концевик
	delay (DEFAULT_DELAY); // завели мешок с машинкой подхвата в раму

	// переносим ушки мешка на раму
	secondaryGrabServo.write (0);
	delay (DEFAULT_DELAY);
	// мешок в раме

	railServo.write (0); // в течении 5 секунд машинка выезжает из рамы
	delay (5000);
	railServo.write (STOP); // установлена начальная позицию для машинки

}

