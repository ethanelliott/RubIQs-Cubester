#-------------------------------------------------------------------------------
# Name:        camera
# Author:      Ethan
# Created:     04/05/2016
# Copyright:   (c) Ethan 2016
#-------------------------------------------------------------------------------

#imports
import pygame.camera
import pygame.image
from pygame.locals import *
import os, sys

#define names for the colours
colours = ["NUL", "WHITE", "RED", "ORANGE", "YELLOW", "GREEN", "BLUE"];

#start pygame
pygame.init()
#start the camera class
pygame.camera.init()

#create a list of the available cameras
#select the first one, and start it
camlist = pygame.camera.list_cameras()
cam = None;
if camlist:
    print len(camlist);
    print camlist;
    cam = pygame.camera.Camera(camlist[0])
    cam.start()
    # grab first frame
    img = cam.get_image()

#Window Width
ww = 1000
wh = 480

#Camera width and height
WIDTH = 720
HEIGHT = 480

#create screen
screen = pygame.display.set_mode(( ww, wh ))
pygame.display.set_caption("")

#printf function to print text on the screen
def printf(screen, color, _size, _font, _x, _y, textString, _center = False):
    font = pygame.font.SysFont(_font, _size)
    textBitmap = font.render(textString, True, color)
    _width = textBitmap.get_width()
    if (_center):
        screen.blit(textBitmap, [(_x - (_width / 2)), _y])
    else:
        screen.blit(textBitmap, [_x, _y])
    return textBitmap.get_size();

#turns RGB tupple into a number referencing the colour name array above
def getColour(rgb):
    if (rgb[0] > 150 and rgb[1] > 150 and rgb[2] > 150):
        return 1;
    elif (rgb[0] > 100 and rgb[1] < 70 and rgb[2] < 70):
        return 2;
    elif (rgb[0] > 150 and rgb[1] < 100 and rgb[2] < 100):
        return 3;
    elif (rgb[0] > 100 and rgb[1] > 150 and rgb[2] > 100):
        return 4;
    elif (rgb[0] < 50 and rgb[1] > 100 and rgb[2] < 150):
        return 5;
    elif (rgb[0] < 50 and rgb[1] > 0 and rgb[2] > 100):
        return 6;
    else:
        return 0;

#program running variable
running = True;
#program frame rate control
clock = pygame.time.Clock()

#various surfaces to put the camera stuff onto
Frame = pygame.Surface((WIDTH, HEIGHT))
snapshot = pygame.Surface((WIDTH,HEIGHT))
vdio = pygame.Surface((720, 480))

#main program loop
while running :
	#detect a program quit
    for event in pygame.event.get() :
        if event.type == pygame.QUIT :
            running = False;

	#get image from camera
    Frame = cam.get_image(Frame)
	#scale the image to fit on the screen
    pygame.transform.smoothscale(Frame, (720,480), vdio)
	#put the newly scaled image on the screen
    screen.blit(vdio, (0,0))
	
	#grab the colours of the cube from the area on the image
	#done before drawing shapes to the screen to ensure a correct reading
    c1 = pygame.transform.average_color(screen, (((WIDTH-50)/2)-150,((HEIGHT-50)/2)-150,50,50))
    c2 = pygame.transform.average_color(screen, (((WIDTH-50)/2),((HEIGHT-50)/2)-150,50,50))
    c3 = pygame.transform.average_color(screen, (((WIDTH-50)/2)+150,((HEIGHT-50)/2)-150,50,50))
    c4 = pygame.transform.average_color(screen, (((WIDTH-50)/2)-150,((HEIGHT-50)/2),50,50))
    c5 = pygame.transform.average_color(screen, (((WIDTH-50)/2),((HEIGHT-50)/2),50,50))
    c6 = pygame.transform.average_color(screen, (((WIDTH-50)/2)+150,((HEIGHT-50)/2),50,50))
    c7 = pygame.transform.average_color(screen, (((WIDTH-50)/2)-150,((HEIGHT-50)/2)+150,50,50))
    c8 = pygame.transform.average_color(screen, (((WIDTH-50)/2),((HEIGHT-50)/2)+150,50,50))
    c9 = pygame.transform.average_color(screen, (((WIDTH-50)/2)+150,((HEIGHT-50)/2)+150,50,50))

	#draw a big square
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-450)/2),((HEIGHT-450)/2),450,450),5)

	#draw the smaller squares inside the larger one
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2)-150,((HEIGHT-150)/2)-150,150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2),((HEIGHT-150)/2)-150,150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2)+150,((HEIGHT-150)/2)-150,150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2)-150,((HEIGHT-150)/2),150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2),((HEIGHT-150)/2),150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2)+150,((HEIGHT-150)/2),150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2)-150,((HEIGHT-150)/2)+150,150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2),((HEIGHT-150)/2)+150,150,150),5)
    pygame.draw.rect(screen, (255,255,255), (((WIDTH-150)/2)+150,((HEIGHT-150)/2)+150,150,150),5)

	#draw rectangles with the correct colours, in a layout correspoding to the cube
    pygame.draw.rect(screen, c1, (700,0,100,100))
    pygame.draw.rect(screen, c2, (800,0,100,100))
    pygame.draw.rect(screen, c3, (900,0,100,100))
    pygame.draw.rect(screen, c4, (700,100,100,100))
    pygame.draw.rect(screen, c5, (800,100,100,100))
    pygame.draw.rect(screen, c6, (900,100,100,100))
    pygame.draw.rect(screen, c7, (700,200,100,100))
    pygame.draw.rect(screen, c8, (800,200,100,100))
    pygame.draw.rect(screen, c9, (900,200,100,100))

	#print the name of the colour on the screen in the worlds most amazing font
    printf(screen, (0,0,0), 20, "comicsansms", 750,25, colours[getColour(c1)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 850,25, colours[getColour(c2)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 950,25, colours[getColour(c3)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 750,125, colours[getColour(c4)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 850,125, colours[getColour(c5)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 950,125, colours[getColour(c6)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 750,225, colours[getColour(c7)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 850,225, colours[getColour(c8)], True);
    printf(screen, (0,0,0), 20, "comicsansms", 950,225, colours[getColour(c9)], True);
	
	#debugging prints for testing with colour identification
    print(c1);
    print(c9);
	
	#update the screen
    pygame.display.flip()
	#limit the speed of the camera updates
    clock.tick(120);

#stop the camera and delete the objects ascociated with the program, quit program
cam.stop();
pygame.quit()
sys.exit()