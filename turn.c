#pragma config(Sensor, port7,  a,              sensorVexIQ_LED)
#pragma config(Sensor, port8,  b,              sensorVexIQ_LED)
#pragma config(Sensor, port9,  c,              sensorVexIQ_LED)
#pragma config(Sensor, port10, d,              sensorVexIQ_LED)
#pragma config(Sensor, port11, e,              sensorVexIQ_LED)
#pragma config(Sensor, port12, f,              sensorVexIQ_LED)
#pragma config(Motor,  motor1,          kFront,        tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor2,          kBack,         tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor3,          kLeft,         tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor4,          kRight,        tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor5,          kUp,           tmotorVexIQ, PIDControl, reversed, encoder)
#pragma config(Motor,  motor6,          kDown,         tmotorVexIQ, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//Includes
#include "EthanPID.h"
#include "algorithm.h"

////////////////////////////////////////////////////////// DEFINITIONS //////////////////////////////////////////////////////////

/*----------------			PID			----------------*/
	PIDStruct m_u;
	PIDStruct m_d;
	PIDStruct m_l;
	PIDStruct m_r;
	PIDStruct m_f;
	PIDStruct m_b;

	const float kmP = 2;
	const float kmI = 0.5;

/*----------------	  Cube Rotations	----------------*/
	int u_p = 0;
	int d_p = 0;
	int l_p = 0;
	int r_p = 0;
	int f_p = 0;
	int b_p = 0;

////////////////////////////////////////////////////////// END DEFINITIONS //////////////////////////////////////////////////////////

void init()
//function called at the very beginning of the program, ensuring proper initialization of variables
//and structures used in the code.
{
	EthanPID(m_u);
	EthanPID(m_d);
	EthanPID(m_l);
	EthanPID(m_r);
	EthanPID(m_f);
	EthanPID(m_b);

	setConstants(m_u, kmP,kmI);
	setConstants(m_d, kmP,kmI);
	setConstants(m_l, kmP,kmI);
	setConstants(m_r, kmP,kmI);
	setConstants(m_f, kmP,kmI);
	setConstants(m_b, kmP,kmI);
}//init

void resetMotors()
//Function used to reset the motor encoders, which sets the inital position of the arms,
//since the code bases the rest of the moves off of this relative position.
{
	resetMotorEncoder(kUp);
	resetMotorEncoder(kDown);
	resetMotorEncoder(kLeft);
	resetMotorEncoder(kRight);
	resetMotorEncoder(kFront);
	resetMotorEncoder(kBack);
}//resetMotors

void stopAllMotors()
//Forces all motors to stop outputing any power, used in the event of a necessary "emergency" stop
{
	motor[motor1] = 0;
	motor[motor2] = 0;
	motor[motor3] = 0;
	motor[motor4] = 0;
	motor[motor5] = 0;
	motor[motor6] = 0;
}//stopAllMotors

bool stepMove = true;//True on the first run through the function, ensures the position only updates when it changes.

bool runAlgorithm(Algorithm &al)
//function to run an algorithm. 
//accepts an algorithm struct as a parameter, of which it will run.
//returns true when complete to allow program flow control later
//in the code.
{
	//Strings and such for debugging display on the robot display.
	string doutput1;
	string doutput2;
	string x;

	stringFormat(x, "Step: %d / %d", al.pos, al.length);
	stringFormat(doutput1, "U: %i, D: %i, L: %i", CubeMoves[al.state[al.pos]][0], CubeMoves[al.state[al.pos]][1], CubeMoves[al.state[al.pos]][2]);
	stringFormat(doutput2, "R: %i, F: %i, B: %i", CubeMoves[al.state[al.pos]][3], CubeMoves[al.state[al.pos]][4], CubeMoves[al.state[al.pos]][5]);
	displayCenteredTextLine(0, x);
	displayCenteredTextLine(2, doutput1);
	displayCenteredTextLine(4, doutput2);

	//ensure this code only runs once when the algorithm changes the necessary move.
	if (stepMove)
	{
		//updates the position as a 90* turn. Used a continuous sum
		//to ensure that after a move is complete, the motor doesn't 
		//move back to the orginal position.
		u_p +=  CubeMoves[al.state[al.pos]][0] * 90;
		d_p +=  CubeMoves[al.state[al.pos]][1] * 90;
		l_p +=  CubeMoves[al.state[al.pos]][2] * 90;
		r_p +=  CubeMoves[al.state[al.pos]][3] * 90;
		f_p +=  CubeMoves[al.state[al.pos]][4] * 90;
		b_p +=  CubeMoves[al.state[al.pos]][5] * 90;

		//Move the array to match the move of the motors
		move(al.state[al.pos]);

		stepMove = false;
	}
	
	//PID control -> setting the value that is desired for the motor to achieve 
	setDesiredValue(m_u, u_p);
	setDesiredValue(m_d, d_p);
	setDesiredValue(m_l, l_p);
	setDesiredValue(m_r, r_p);
	setDesiredValue(m_f, f_p);
	setDesiredValue(m_b, b_p);

	//condition by which porgresses the program flow.
	//If the encoder is within an epsilon of one (1) 
	//degree, the code moves to the next turn.
	if (abs(u_p - getMotorEncoder(kUp)) < 1 && abs(d_p - getMotorEncoder(kDown)) < 1 && abs(l_p - getMotorEncoder(kLeft)) < 1 && abs(r_p - getMotorEncoder(kRight)) < 1 && abs(f_p - getMotorEncoder(kFront)) < 1 && abs(b_p - getMotorEncoder(kBack)) < 1 )
	{
		//If the algorithm is still running...
		if (al.pos < al.length)
		{
			//progress to the next move
			al.pos += 1;
			stepMove = true;
		}
		//Other wise if the algorithm just finished
		//the last turn
		else if (al.pos == al.length)
		{
			//reset the algorithm structure and exit the function
			al.pos = 0;
			stepMove = true;
			return true;
		}
	}
	//default return statement.
	return false;
} //runAlgorithm

//The code below is dedicated to the "flashy lights" on the robot. 
//It runs a state machine, which causes a "flowing" animation.
int robotState = 0;
int colourLimiterDopeTransition = 11;
int loopCounter = 0;
void flashyLights()
{
	switch (colourLimiterDopeTransition)
	{
		case 0:
			setTouchLEDRGB(a, 0,255,0);
			setTouchLEDRGB(b, 0,0,0);
			setTouchLEDRGB(c, 0,0,0);
			setTouchLEDRGB(d, 0,0,0);
			setTouchLEDRGB(e, 0,0,0);
			setTouchLEDRGB(f, 0,0,0);
			break;
		case 1:
			setTouchLEDRGB(a, 0,255,0);
			setTouchLEDRGB(b, 0,255,0);
			setTouchLEDRGB(c, 0,0,0);
			setTouchLEDRGB(d, 0,0,0);
			setTouchLEDRGB(e, 0,0,0);
			setTouchLEDRGB(f, 0,0,0);
			break;
		case 2:
			setTouchLEDRGB(a, 0,255,0);
			setTouchLEDRGB(b, 0,255,0);
			setTouchLEDRGB(c, 0,255,0);
			setTouchLEDRGB(d, 0,0,0);
			setTouchLEDRGB(e, 0,0,0);
			setTouchLEDRGB(f, 0,0,0);
			break;
		case 3:
			setTouchLEDRGB(a, 0,255,0);
			setTouchLEDRGB(b, 0,255,0);
			setTouchLEDRGB(c, 0,255,0);
			setTouchLEDRGB(d, 0,255,0);
			setTouchLEDRGB(e, 0,0,0);
			setTouchLEDRGB(f, 0,0,0);
			break;
		case 4:
			setTouchLEDRGB(a, 0,255,0);
			setTouchLEDRGB(b, 0,255,0);
			setTouchLEDRGB(c, 0,255,0);
			setTouchLEDRGB(d, 0,255,0);
			setTouchLEDRGB(e, 0,255,0);
			setTouchLEDRGB(f, 0,0,0);
			break;
		case 5:
			setTouchLEDRGB(a, 0,255,0);
			setTouchLEDRGB(b, 0,255,0);
			setTouchLEDRGB(c, 0,255,0);
			setTouchLEDRGB(d, 0,255,0);
			setTouchLEDRGB(e, 0,255,0);
			setTouchLEDRGB(f, 0,255,0);
			break;
		case 6:
			setTouchLEDRGB(a, 0,0,0);
			setTouchLEDRGB(b, 0,255,0);
			setTouchLEDRGB(c, 0,255,0);
			setTouchLEDRGB(d, 0,255,0);
			setTouchLEDRGB(e, 0,255,0);
			setTouchLEDRGB(f, 0,255,0);
			break;
		case 7:
			setTouchLEDRGB(a, 0,0,0);
			setTouchLEDRGB(b, 0,0,0);
			setTouchLEDRGB(c, 0,255,0);
			setTouchLEDRGB(d, 0,255,0);
			setTouchLEDRGB(e, 0,255,0);
			setTouchLEDRGB(f, 0,255,0);
			break;
		case 8:
			setTouchLEDRGB(a, 0,0,0);
			setTouchLEDRGB(b, 0,0,0);
			setTouchLEDRGB(c, 0,0,0);
			setTouchLEDRGB(d, 0,255,0);
			setTouchLEDRGB(e, 0,255,0);
			setTouchLEDRGB(f, 0,255,0);
			break;
		case 9:
			setTouchLEDRGB(a, 0,0,0);
			setTouchLEDRGB(b, 0,0,0);
			setTouchLEDRGB(c, 0,0,0);
			setTouchLEDRGB(d, 0,0,0);
			setTouchLEDRGB(e, 0,255,0);
			setTouchLEDRGB(f, 0,255,0);
			break;
		case 10:
			setTouchLEDRGB(a, 0,0,0);
			setTouchLEDRGB(b, 0,0,0);
			setTouchLEDRGB(c, 0,0,0);
			setTouchLEDRGB(d, 0,0,0);
			setTouchLEDRGB(e, 0,0,0);
			setTouchLEDRGB(f, 0,255,0);
			break;
		case 11:
			setTouchLEDRGB(a, 0,0,0);
			setTouchLEDRGB(b, 0,0,0);
			setTouchLEDRGB(c, 0,0,0);
			setTouchLEDRGB(d, 0,0,0);
			setTouchLEDRGB(e, 0,0,0);
			setTouchLEDRGB(f, 0,0,0);
			break;
	}

	//The code below deals with the state machine transitions
	if (colourLimiterDopeTransition==11)
	{
		colourLimiterDopeTransition = 0;
	}
	else
	{
		if (loopCounter > 2)
		{
			colourLimiterDopeTransition++;
			loopCounter = 0;
		}
		else
		{
			loopCounter++;
		}
	}
}

//The beginnings of a "solve cube" function. 
//although the code is incomplete, the beginnings 
//of a structured solve program is visible.
//Again, this code uses multiple state machines
//to transition between manuvers on the cube, acting
//out the raw moves and checking the array for values.
int solveCubeState = 0;

int subSolveCubeState = 0;
int subSolveCounter = 0;

int subSubSolveCubeState = 0;

bool SolveCube()
{
	switch (solveCubeState)
	{
		case 0:
			if (runAlgorithm(turnFace))
			{
				solveCubeState++;
			}
			break;
			case 1:
				switch(subSolveCubeState)
				{
					case 0:
						if (whiteFace[1][2] == WHITE && redFace[1][0] == RED)
						{
							subSolveCubeState = 3;
							subSolveCounter = 0;
						}
						else if (whiteFace[1][2] == RED && redFace[1][0] == WHITE)
						{
							subSolveCubeState = 2;
							subSolveCounter = 0;
						}
						else
						{
							subSolveCubeState++;
						}
						break;
					case 1:
						switch (subSolveCounter)
						{
							case 0:
								if (runAlgorithm(whiteCrossPt1Red))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 1:
								if (runAlgorithm(whiteCrossPt1Red))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 2:
								if (runAlgorithm(whiteCrossPt1Red))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 3:
								switch (subSubSolveCubeState)
								{
									case 0:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 1:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 2:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 3:
										if (runAlgorithm(whiteBackRotateRed))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState = 0;
										}
										break;
								}
								break;
						}
						break;
					case 2:
						if (runAlgorithm(whiteCrossPt2Red))
						{
							subSolveCubeState = 0;
						}
						break;
					case 3:
						solveCubeState++;
						subSolveCubeState = 0;
						subSolveCounter = 0;
						break;
				}
				break;
			case 2:
				switch(subSolveCubeState)
				{
					case 0:
						if (whiteFace[0][1] == WHITE && greenFace[1][0] == GREEN)
						{
							subSolveCubeState = 3;
							subSolveCounter = 0;
						}
						else if (whiteFace[0][1] == GREEN && greenFace[1][0] == WHITE)
						{
							subSolveCubeState = 2;
							subSolveCounter = 0;
						}
						else
						{
							subSolveCubeState++;
						}
						break;
					case 1:
						switch (subSolveCounter)
						{
							case 0:
								if (runAlgorithm(whiteCrossPt1Green))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 1:
								if (runAlgorithm(whiteCrossPt1Green))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 2:
								if (runAlgorithm(whiteCrossPt1Green))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 3:
								switch (subSubSolveCubeState)
								{
									case 0:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 1:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 2:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 3:
										if (runAlgorithm(whiteBackRotateGreen))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState = 0;
										}
										break;
								}
								break;
						}
						break;
					case 2:
						if (runAlgorithm(whiteCrossPt2Green))
						{
							subSolveCubeState = 0;
						}
						break;
					case 3:
						solveCubeState++;
						subSolveCubeState = 0;
						subSolveCounter = 0;
						break;
				}
				break;
			case 3:
				switch(subSolveCubeState)
				{
					case 0:
						if (whiteFace[1][0] == WHITE && orangeFace[1][0] == ORANGE)
						{
							subSolveCubeState = 3;
							subSolveCounter = 0;
						}
						else if (whiteFace[1][0] == ORANGE && orangeFace[1][0] == WHITE)
						{
							subSolveCubeState = 2;
							subSolveCounter = 0;
						}
						else
						{
							subSolveCubeState++;
						}
						break;
					case 1:
						switch (subSolveCounter)
						{
							case 0:
								if (runAlgorithm(whiteCrossPt1Orange))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 1:
								if (runAlgorithm(whiteCrossPt1Orange))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 2:
								if (runAlgorithm(whiteCrossPt1Orange))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 3:
								switch (subSubSolveCubeState)
								{
									case 0:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 1:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 2:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 3:
										subSolveCubeState = 0;
										subSolveCounter = 0;
										subSubSolveCubeState = 0;
										break;
								}
								break;
						}
						break;
					case 2:
						if (runAlgorithm(whiteCrossPt2Orange))
						{
							subSolveCubeState = 0;
						}
						break;
					case 3:
						solveCubeState++;
						subSolveCubeState = 0;
						subSolveCounter = 0;
						break;
				}
				break;
			case 4:
				switch(subSolveCubeState)
				{
					case 0:
						if (whiteFace[2][1] == WHITE && blueFace[1][0] == BLUE)
						{
							subSolveCubeState = 3;
							subSolveCounter = 0;
							writeDebugStreamLine("ITS IN THE RIGHT SPOT!");
						}
						else if (whiteFace[2][1] == BLUE && blueFace[1][0] == WHITE)
						{
							subSolveCubeState = 2;
							subSolveCounter = 0;
							writeDebugStreamLine("ITS IN THE RIGHT SPOT but backwards...");
						}
						else
						{
							subSolveCubeState++;
						}
						break;
					case 1:
						switch (subSolveCounter)
						{
							case 0:
								if (runAlgorithm(whiteCrossPt1Blue))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 1:
								if (runAlgorithm(whiteCrossPt1Blue))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 2:
								if (runAlgorithm(whiteCrossPt1Blue))
								{
									subSolveCubeState = 0;
									subSolveCounter++;
								}
								break;
							case 3:
								switch (subSubSolveCubeState)
								{
									case 0:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 1:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 2:
										if (runAlgorithm(whiteBottomRotate))
										{
											subSolveCubeState = 0;
											subSolveCounter = 0;
											subSubSolveCubeState++;
										}
										break;
									case 3:
										subSolveCubeState = 0;
										subSolveCounter = 0;
										subSubSolveCubeState = 0;
										break;
								}
								break;
						}
						break;
					case 2:
						if (runAlgorithm(whiteCrossPt2Blue))
						{
							subSolveCubeState = 0;
						}
						break;
					case 3:
						solveCubeState++;
						subSolveCubeState = 0;
						subSolveCounter = 0;
						break;
				}
				break;
			case 5:
				solveCubeState = 0;
				subSolveCubeState = 0;
				subSolveCounter = 0;
				return true;
				break;
	}
	return false;
}//solveCube

task main()
//Main program task
{
	init();//initialization
	algorithmInit();//algorithm initialization
	resetMotors();//reset the motor encoders

	//Main program loop
	while(true)
	{
		//Update the display
		eraseDisplay();

		//main robot state machine.
		//deals with the "enabled" and "idle" states
		switch(robotState)
		{
			case 0: //Idle
				flashyLights();//show the lights
				if (getTouchLEDValue(port12)) //if pressed, enable the solve
				{
					robotState = 1;//next state
					playNote(noteC, octave2, 5);//brief feedback to inform the user that the button has been pressed
				}
				break;
			case 1: //Moving
				//causes solid output on the light strip
				setTouchLEDRGB(a, 255,0,0);
				setTouchLEDRGB(b, 255,0,0);
				setTouchLEDRGB(c, 255,0,0);
				setTouchLEDRGB(d, 255,0,0);
				setTouchLEDRGB(e, 255,0,0);
				setTouchLEDRGB(f, 255,0,0);
				//Solves the cube (kind of)
				if (SolveCube())
				{
					//once solved, return to the idle position
					robotState = 0;
				}
			break;
		}
		
		//update motor output based on PID feedback
		motor[kUp] = calcPID(m_u, getMotorEncoder(kUp));
		motor[kDown] = calcPID(m_d, getMotorEncoder(kDown));
		motor[kRight] = calcPID(m_r, getMotorEncoder(kRight));
		motor[kLeft] = calcPID(m_l, getMotorEncoder(kLeft));
		motor[kFront] = calcPID(m_f, getMotorEncoder(kFront));
		motor[kBack] = calcPID(m_b, getMotorEncoder(kBack));
		
		//Limit clock cycles
		wait1Msec(20);
		//ensure the robot does not enter a "sleep" state
		alive();
	}
	//Stop all motor output
	stopAllMotors();
	//turn off the touch LED's
	setTouchLEDRGB(a, 0,0,0);
	setTouchLEDRGB(b, 0,0,0);
	setTouchLEDRGB(c, 0,0,0);
	setTouchLEDRGB(d, 0,0,0);
	setTouchLEDRGB(e, 0,0,0);
	setTouchLEDRGB(f, 0,0,0);
}