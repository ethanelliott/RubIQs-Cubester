#pragma systemFile

//enumerated colours for improved ease of colour reference.
enum CubeColours {
		WHITE,	//0
		YELLOW,	//1
		RED,		//2
		ORANGE,	//3
		BLUE,		//4
		GREEN		//5
};

//creates the cube faces, in the data type of "CubeColours"
CubeColours whiteFace[3][3];
CubeColours yellowFace[3][3];
CubeColours redFace[3][3];
CubeColours orangeFace[3][3];
CubeColours blueFace[3][3];
CubeColours greenFace[3][3];

//The algorithm struct
typedef struct {
	int state[15];//Moves in the algorithm
	int length;//length of the algorithm -1
	int pos; //Position in the algorithm to start. used for tracking in the "run algorithm" function
}Algorithm;

//Init moves
//Each 
float CubeMoves[12][6] = {
	//U		D			L			R			F			B
	{1, 	0, 		0, 		0, 		0, 		0}, 		//U  = 0
	{-1, 	0, 		0,		0, 		0, 		0},			//U' = 1
	{0, 	1, 		0,		0, 		0, 		0},			//D	 = 2
	{0,  -1,		0, 		0, 		0, 		0},			//D' = 3
	{0, 	0, 		1, 		0, 		0, 		0},			//L	 = 4
	{0, 	0, 	 -1, 		0, 		0, 		0},			//L' = 5
	{0, 	0, 		0, 		1, 		0, 		0},			//R	 = 6
	{0, 	0, 		0, 	 -1,		0, 		0},			//R' = 7
	{0, 	0, 		0, 		0, 		1, 		0},			//F	 = 8
	{0, 	0, 		0, 		0, 	 -1,		0},			//F' = 9
	{0, 	0, 		0, 		0, 		0,		1},			//B	 = 10
	{0, 	0, 		0, 		0, 		0, 	 -1}			//B' = 11
};


/*----------------	  	Algorithms		----------------*/

Algorithm turnFace;

//Stage 1 -> White Cross
Algorithm whiteCrossPt1Red;
Algorithm whiteCrossPt2Red;
Algorithm whiteCrossPt1Blue;
Algorithm whiteCrossPt2Blue;
Algorithm whiteCrossPt1Orange;
Algorithm whiteCrossPt2Orange;
Algorithm whiteCrossPt1Green;
Algorithm whiteCrossPt2Green;

Algorithm whiteBottomRotate;
Algorithm whiteBackRotateRed;
Algorithm whiteBackRotateGreen;

//Stage 2 -> White Corners
Algorithm whiteCornersPt1Red;
Algorithm whiteCornersPt2Red;
Algorithm whiteCornersPt1Blue;
Algorithm whiteCornersPt2Blue;
Algorithm whiteCornersPt1Orange;
Algorithm whiteCornersPt2Orange;
Algorithm whiteCornersPt1Green;
Algorithm whiteCornersPt2Green;

//Stage 3 -> Middle Layer
Algorithm middleLayerMoveRightRed;
Algorithm middleLayerMoveLeftRed;
Algorithm middleLayerMoveRightBlue;
Algorithm middleLayerMoveLeftBlue;
Algorithm middleLayerMoveRightOrange;
Algorithm middleLayerMoveLeftOrange;
Algorithm middleLayerMoveRightGreen;
Algorithm middleLayerMoveLeftGreen;

//Stage 4 -> Yellow Cross
Algorithm yellowCrossState1;
Algorithm yellowCrossState2;
Algorithm yellowCrossState3;
Algorithm yellowCrossState4;

//Stage 5 -> Yellow on top
Algorithm yellowOnTop;

//Stage 6 -> Yellow Corners
Algorithm yellowCorners;

//Stage 7 -> Yellow Edges
Algorithm yellowEdgesRightRed;
Algorithm yellowEdgesLeftRed;
Algorithm yellowEdgesRightBlue;
Algorithm yellowEdgesLeftBlue;
Algorithm yellowEdgesRightOrange;
Algorithm yellowEdgesLeftOrange;
Algorithm yellowEdgesRightGreen;
Algorithm yellowEdgesLeftGreen;

/*----------------	  	Algorithms		----------------*/

void algorithmInit()
{
	whiteFace[0][0] = WHITE;
	whiteFace[0][1] = WHITE;
	whiteFace[0][2] = WHITE;
	whiteFace[1][0] = WHITE;
	whiteFace[1][1] = WHITE;
	whiteFace[1][2] = WHITE;
	whiteFace[2][0] = WHITE;
	whiteFace[2][1] = WHITE;
	whiteFace[2][2] = WHITE;

	yellowFace[0][0] = YELLOW;
	yellowFace[0][1] = YELLOW;
	yellowFace[0][2] = YELLOW;
	yellowFace[1][0] = YELLOW;
	yellowFace[1][1] = YELLOW;
	yellowFace[1][2] = YELLOW;
	yellowFace[2][0] = YELLOW;
	yellowFace[2][1] = YELLOW;
	yellowFace[2][2] = YELLOW;

	redFace[0][0] = RED;
	redFace[0][1] = RED;
	redFace[0][2] = RED;
	redFace[1][0] = RED;
	redFace[1][1] = RED;
	redFace[1][2] = RED;
	redFace[2][0] = RED;
	redFace[2][1] = RED;
	redFace[2][2] = RED;

	orangeFace[0][0] = ORANGE;
	orangeFace[0][1] = ORANGE;
	orangeFace[0][2] = ORANGE;
	orangeFace[1][0] = ORANGE;
	orangeFace[1][1] = ORANGE;
	orangeFace[1][2] = ORANGE;
	orangeFace[2][0] = ORANGE;
	orangeFace[2][1] = ORANGE;
	orangeFace[2][2] = ORANGE;

	blueFace[0][0] = BLUE;
	blueFace[0][1] = BLUE;
	blueFace[0][2] = BLUE;
	blueFace[1][0] = BLUE;
	blueFace[1][1] = BLUE;
	blueFace[1][2] = BLUE;
	blueFace[2][0] = BLUE;
	blueFace[2][1] = BLUE;
	blueFace[2][2] = BLUE;

	greenFace[0][0] = GREEN;
	greenFace[0][1] = GREEN;
	greenFace[0][2] = GREEN;
	greenFace[1][0] = GREEN;
	greenFace[1][1] = GREEN;
	greenFace[1][2] = GREEN;
	greenFace[2][0] = GREEN;
	greenFace[2][1] = GREEN;

	//Actual Algorithms:
	//creating the algorithm, and giving it the moves
	turnFace.length = 3;
	turnFace.pos = 0;
	turnFace.state[0] = 8;
	turnFace.state[1] = 0;
	turnFace.state[2] = 10;
	turnFace.state[3] = 2;

	whiteCrossPt1Red.length = 0;
	whiteCrossPt1Red.pos = 0;
	whiteCrossPt1Red.state[0] = 8;

	whiteCrossPt2Red.length = 3;
	whiteCrossPt2Red.pos = 0;
	whiteCrossPt2Red.state[0] = 9;
	whiteCrossPt2Red.state[1] = 0;
	whiteCrossPt2Red.state[2] = 5;
	whiteCrossPt2Red.state[3] = 1;

	whiteCrossPt1Blue.length = 0;
	whiteCrossPt1Blue.pos = 0;
	whiteCrossPt1Blue.state[0] = 6;

	whiteCrossPt2Blue.length = 3;
	whiteCrossPt2Blue.pos = 0;
	whiteCrossPt2Blue.state[0] = 7;
	whiteCrossPt2Blue.state[1] = 0;
	whiteCrossPt2Blue.state[2] = 9;
	whiteCrossPt2Blue.state[3] = 1;

	whiteCrossPt1Orange.length = 0;
	whiteCrossPt1Orange.pos = 0;
	whiteCrossPt1Orange.state[0] = 10;

	whiteCrossPt2Orange.length = 3;
	whiteCrossPt2Orange.pos = 0;
	whiteCrossPt2Orange.state[0] = 11;
	whiteCrossPt2Orange.state[1] = 0;
	whiteCrossPt2Orange.state[2] = 7;
	whiteCrossPt2Orange.state[3] = 1;

	whiteCrossPt1Green.length = 0;
	whiteCrossPt1Green.pos = 0;
	whiteCrossPt1Green.state[0] = 4;

	whiteCrossPt2Green.length = 3;
	whiteCrossPt2Green.pos = 0;
	whiteCrossPt2Green.state[0] = 5;
	whiteCrossPt2Green.state[1] = 0;
	whiteCrossPt2Green.state[2] = 11;
	whiteCrossPt2Green.state[3] = 1;

	whiteBottomRotate.length = 0;
	whiteBottomRotate.pos = 0;
	whiteBottomRotate.state[0] = 2;

	whiteBackRotateRed.length = 0;
	whiteBackRotateRed.pos = 0;
	whiteBackRotateRed.state[0] = 10;

	whiteBackRotateGreen.length = 0;
	whiteBackRotateGreen.pos = 0;
	whiteBackRotateGreen.state[0] = 6;

}//algorithmInit


//Moving things in the array
//below is the biggest mess of array movements.
//those who have read the entire thing have never
//been the same again, most of them living in 
//complete isolation for many years, never to
//be heard from again. My best recomendation,
//although I cannot force you to do anything,
//is to turn back now, before it't too late.
//Good luck
void move(int movementType)
{
	if(movementType == 0)
	{//U
		writeDebugStreamLine("U");
		CubeColours faceCopy[3][3];
		faceCopy[0][0] = whiteFace[0][0];
		faceCopy[0][1] = whiteFace[0][1];
		faceCopy[0][2] = whiteFace[0][2];
		faceCopy[1][0] = whiteFace[1][0];
		faceCopy[1][1] = whiteFace[1][1];
		faceCopy[1][2] = whiteFace[1][2];
		faceCopy[2][0] = whiteFace[2][0];
		faceCopy[2][1] = whiteFace[2][1];
		faceCopy[2][2] = whiteFace[2][2];
		whiteFace[0][0] = faceCopy[0][2];
		whiteFace[0][1] = faceCopy[1][2];
		whiteFace[0][2] = faceCopy[2][2];
		whiteFace[1][0] = faceCopy[0][1];
		whiteFace[1][1] = faceCopy[1][1];
		whiteFace[1][2] = faceCopy[2][1];
		whiteFace[2][0] = faceCopy[0][0];
		whiteFace[2][1] = faceCopy[1][0];
		whiteFace[2][2] = faceCopy[2][0];
		CubeColours tempRow[3] = {redFace[0][0], redFace[1][0], redFace[2][0]};
		redFace[0][0] = blueFace[0][0];
		redFace[1][0] = blueFace[1][0];
		redFace[2][0] = blueFace[2][0];
		blueFace[0][0] = orangeFace[0][0];
		blueFace[1][0] = orangeFace[1][0];
		blueFace[2][0] = orangeFace[2][0];
		orangeFace[0][0] = greenFace[0][0];
		orangeFace[1][0] = greenFace[1][0];
		orangeFace[2][0] = greenFace[2][0];
		greenFace[0][0] = tempRow[0];
		greenFace[1][0] = tempRow[1];
		greenFace[2][0] = tempRow[2];
	}
	else if(movementType == 1)
	{//U'
		writeDebugStreamLine("U'");
		move(0);
		move(0);
		move(0);
	}
	else if(movementType == 2)
	{//D
		writeDebugStreamLine("D");
		CubeColours faceCopy[3][3];
		faceCopy[0][0] = yellowFace[0][0];
		faceCopy[0][1] = yellowFace[0][1];
		faceCopy[0][2] = yellowFace[0][2];
		faceCopy[1][0] = yellowFace[1][0];
		faceCopy[1][1] = yellowFace[1][1];
		faceCopy[1][2] = yellowFace[1][2];
		faceCopy[2][0] = yellowFace[2][0];
		faceCopy[2][1] = yellowFace[2][1];
		faceCopy[2][2] = yellowFace[2][2];
		yellowFace[0][0] = faceCopy[0][2];
		yellowFace[0][1] = faceCopy[1][2];
		yellowFace[0][2] = faceCopy[2][2];
		yellowFace[1][0] = faceCopy[0][1];
		yellowFace[1][1] = faceCopy[1][1];
		yellowFace[1][2] = faceCopy[2][1];
		yellowFace[2][0] = faceCopy[0][0];
		yellowFace[2][1] = faceCopy[1][0];
		yellowFace[2][2] = faceCopy[2][0];
		CubeColours tempRow[3] = {redFace[0][2], redFace[1][2], redFace[2][2]};
		redFace[0][2] = greenFace[0][2];
		redFace[1][2] = greenFace[1][2];
		redFace[2][2] = greenFace[2][2];
		greenFace[0][2] = orangeFace[0][2];
		greenFace[1][2] = orangeFace[1][2];
		greenFace[2][2] = orangeFace[2][2];
		orangeFace[0][2] = blueFace[0][2];
		orangeFace[1][2] = blueFace[1][2];
		orangeFace[2][2] = blueFace[2][2];
		blueFace[0][2] = tempRow[0];
		blueFace[1][2] = tempRow[1];
		blueFace[2][2] = tempRow[2];
	}
	else if(movementType == 3)
	{//D'
		writeDebugStreamLine("D'");
		move(2);
		move(2);
		move(2);
	}
	else if(movementType == 4)
	{//L
		writeDebugStreamLine("L");
		CubeColours faceCopy[3][3];
		faceCopy[0][0] = greenFace[0][0];
		faceCopy[0][1] = greenFace[0][1];
		faceCopy[0][2] = greenFace[0][2];
		faceCopy[1][0] = greenFace[1][0];
		faceCopy[1][1] = greenFace[1][1];
		faceCopy[1][2] = greenFace[1][2];
		faceCopy[2][0] = greenFace[2][0];
		faceCopy[2][1] = greenFace[2][1];
		faceCopy[2][2] = greenFace[2][2];
		greenFace[0][0] = faceCopy[0][2];
		greenFace[0][1] = faceCopy[1][2];
		greenFace[0][2] = faceCopy[2][2];
		greenFace[1][0] = faceCopy[0][1];
		greenFace[1][1] = faceCopy[1][1];
		greenFace[1][2] = faceCopy[2][1];
		greenFace[2][0] = faceCopy[0][0];
		greenFace[2][1] = faceCopy[1][0];
		greenFace[2][2] = faceCopy[2][0];
		CubeColours tempRow[3] = {whiteFace[0][0], whiteFace[0][1], whiteFace[0][2]};
		whiteFace[0][0] = orangeFace[2][2];
		whiteFace[0][1] = orangeFace[2][1];
		whiteFace[0][2] = orangeFace[2][0];
		orangeFace[2][2] = yellowFace[0][0];
		orangeFace[2][1] = yellowFace[0][1];
		orangeFace[2][0] = yellowFace[0][2];
		yellowFace[0][0] = redFace[0][0];
		yellowFace[0][1] = redFace[0][1];
		yellowFace[0][2] = redFace[0][2];
		redFace[0][0] = tempRow[0];
		redFace[0][1] = tempRow[1];
		redFace[0][2] = tempRow[2];
	}
	else if(movementType == 5)
	{//L'
		writeDebugStreamLine("L'");
		move(4);
		move(4);
		move(4);
	}
	else if(movementType == 6)
	{//R
		writeDebugStreamLine("R");
		CubeColours faceCopy[3][3];
		faceCopy[0][0] = blueFace[0][0];
		faceCopy[0][1] = blueFace[0][1];
		faceCopy[0][2] = blueFace[0][2];
		faceCopy[1][0] = blueFace[1][0];
		faceCopy[1][1] = blueFace[1][1];
		faceCopy[1][2] = blueFace[1][2];
		faceCopy[2][0] = blueFace[2][0];
		faceCopy[2][1] = blueFace[2][1];
		faceCopy[2][2] = blueFace[2][2];
		blueFace[0][0] = faceCopy[0][2];
		blueFace[0][1] = faceCopy[1][2];
		blueFace[0][2] = faceCopy[2][2];
		blueFace[1][0] = faceCopy[0][1];
		blueFace[1][1] = faceCopy[1][1];
		blueFace[1][2] = faceCopy[2][1];
		blueFace[2][0] = faceCopy[0][0];
		blueFace[2][1] = faceCopy[1][0];
		blueFace[2][2] = faceCopy[2][0];
		CubeColours tempRow[3] = {whiteFace[2][2], whiteFace[2][1], whiteFace[2][0]};
		whiteFace[2][2] = redFace[2][2];
		whiteFace[2][1] = redFace[2][1];
		whiteFace[2][0] = redFace[2][0];
		redFace[2][2] = yellowFace[2][2];
		redFace[2][1] = yellowFace[2][1];
		redFace[2][0] = yellowFace[2][0];
		yellowFace[2][2] = orangeFace[0][0];
		yellowFace[2][1] = orangeFace[0][1];
		yellowFace[2][0] = orangeFace[0][2];
		orangeFace[0][0] = tempRow[0];
		orangeFace[0][1] = tempRow[1];
		orangeFace[0][2] = tempRow[2];
	}
	else if(movementType == 7)
	{//R'
		writeDebugStreamLine("R'");
		move(6);
		move(6);
		move(6);
	}
	else if(movementType == 8)
	{//F
		writeDebugStreamLine("F");
		CubeColours faceCopy[3][3];
		faceCopy[0][0] = redFace[0][0];
		faceCopy[0][1] = redFace[0][1];
		faceCopy[0][2] = redFace[0][2];
		faceCopy[1][0] = redFace[1][0];
		faceCopy[1][1] = redFace[1][1];
		faceCopy[1][2] = redFace[1][2];
		faceCopy[2][0] = redFace[2][0];
		faceCopy[2][1] = redFace[2][1];
		faceCopy[2][2] = redFace[2][2];
		redFace[0][0] = faceCopy[0][2];
		redFace[0][1] = faceCopy[1][2];
		redFace[0][2] = faceCopy[2][2];
		redFace[1][0] = faceCopy[0][1];
		redFace[1][1] = faceCopy[1][1];
		redFace[1][2] = faceCopy[2][1];
		redFace[2][0] = faceCopy[0][0];
		redFace[2][1] = faceCopy[1][0];
		redFace[2][2] = faceCopy[2][0];
		CubeColours tempRow[3] = {whiteFace[0][2], whiteFace[1][2], whiteFace[2][2]};
		whiteFace[0][2] = greenFace[2][2];
		whiteFace[1][2] = greenFace[2][1];
		whiteFace[2][2] = greenFace[2][0];
		greenFace[2][2] = yellowFace[2][0];
		greenFace[2][1] = yellowFace[1][0];
		greenFace[2][0] = yellowFace[0][0];
		yellowFace[2][0] = blueFace[0][0];
		yellowFace[1][0] = blueFace[0][1];
		yellowFace[0][0] = blueFace[0][2];
		blueFace[0][0] = tempRow[0];
		blueFace[0][1] = tempRow[1];
		blueFace[0][2] = tempRow[2];
	}
	else if(movementType == 9)
	{//F'
		writeDebugStreamLine("F'");
		move(8);
		move(8);
		move(8);
	}
	else if(movementType == 10)
	{//B
		writeDebugStreamLine("B");
		CubeColours faceCopy[3][3];
		faceCopy[0][0] = orangeFace[0][0];
		faceCopy[0][1] = orangeFace[0][1];
		faceCopy[0][2] = orangeFace[0][2];
		faceCopy[1][0] = orangeFace[1][0];
		faceCopy[1][1] = orangeFace[1][1];
		faceCopy[1][2] = orangeFace[1][2];
		faceCopy[2][0] = orangeFace[2][0];
		faceCopy[2][1] = orangeFace[2][1];
		faceCopy[2][2] = orangeFace[2][2];
		orangeFace[0][0] = faceCopy[0][2];
		orangeFace[0][1] = faceCopy[1][2];
		orangeFace[0][2] = faceCopy[2][2];
		orangeFace[1][0] = faceCopy[0][1];
		orangeFace[1][1] = faceCopy[1][1];
		orangeFace[1][2] = faceCopy[2][1];
		orangeFace[2][0] = faceCopy[0][0];
		orangeFace[2][1] = faceCopy[1][0];
		orangeFace[2][2] = faceCopy[2][0];
		CubeColours tempRow[3] = {whiteFace[2][0], whiteFace[1][0], whiteFace[0][0]};
		whiteFace[2][0] = blueFace[2][2];
		whiteFace[1][0] = blueFace[2][1];
		whiteFace[0][0] = blueFace[2][0];
		blueFace[2][2] = yellowFace[0][2];
		blueFace[2][1] = yellowFace[1][2];
		blueFace[2][0] = yellowFace[2][2];
		yellowFace[0][2] = greenFace[0][0];
		yellowFace[1][2] = greenFace[0][1];
		yellowFace[2][2] = greenFace[0][2];
		greenFace[0][0] = tempRow[0];
		greenFace[0][1] = tempRow[1];
		greenFace[0][2] = tempRow[2];
	}
	else if(movementType == 11)
	{//B'
		writeDebugStreamLine("B'");
		move(10);
		move(10);
		move(10);
	}
}//move
