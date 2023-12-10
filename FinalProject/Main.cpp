//Russell Wilis
//12-8-2023
#define TO_RADIANS 3.141592/180.0
#define _CRT_SECURE_NO_WARNINGS
#include <GL/freeglut.h>
#include <iostream>
#define IMAGE_ROWS 64      // Texture image rows and columns
#define IMAGE_COLS 64

#define PI 3.1415927

enum
{
	MOUSE_LEFT_BUTTON = 0,
	MOUSE_MIDDLE_BUTTON = 1,
	MOUSE_RIGHT_BUTTON = 2,
	MOUSE_SCROLL_UP = 3,
	MOUSE_SCROLL_DOWN = 4
};

// Texture
GLubyte imageData[IMAGE_ROWS][IMAGE_COLS][3]; // Texture image data
// angles and starting position
float xangle = 0.0f, yangle = 1.0f, zangle = 0.0f;

// XYZ position of the camera
float  x = -8.0f, y = 7.0f, z = 0.0f;

// Mouse rotation sensitivity
float sensitivity = 0.5f;

// width and height of the window
int ww, hh;

// Mouse position and yaw  
float x_pos = 0.0f, y_pos = 0.0f;

// Load the imageData array with checkerboad pattern
void loadTextureImageData() {
	int value;
	for (int row = 0; row < IMAGE_ROWS; row++) {
		for (int col = 0; col < IMAGE_COLS; col++) {
			// Each cell is 8x8, value is 0 or 255 (black or white)
			value = (((row & 0x8) == 0) ^ ((col & 0x8) == 0)) * 255;
			imageData[row][col][0] = (GLubyte)value;
			imageData[row][col][1] = (GLubyte)value;
			imageData[row][col][2] = (GLubyte)value;
		}
	}
}

// Initialize OpenGL
void initGL(GLvoid) {
	glShadeModel(GL_SMOOTH);               // Enable smooth shading of color
	// Depth-buffer for hidden surface removal
	glClearDepth(1.0f);       // Set clear depth value to farthest
	glEnable(GL_DEPTH_TEST);  // Enable depth-buffer for hidden surface removal
	glDepthFunc(GL_LEQUAL);   // The type of depth testing to do

	// Texture
	loadTextureImageData();   // Load pattern into image data array
	glTexImage2D(GL_TEXTURE_2D, 0, 3, IMAGE_COLS, IMAGE_ROWS, 0, GL_RGB,
		GL_UNSIGNED_BYTE, imageData);  // Create texture from image data
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glEnable(GL_TEXTURE_2D);  // Enable 2D texture 

}

void mouseMove(int xx, int yy) {

	// Get mouse position scaling by the center of the scene
	x_pos = (xx - ww / 2) * 0.01;
	y_pos = (yy - hh / 2) * 0.01;

	// Get the angle of the coordinate: we just use sin for both x/y and 
	// - cos for the z coordinate (since we're looking into the scene)
	xangle = sin(x_pos);
	yangle = sin(y_pos);
	zangle = -cos(x_pos);


	// Do not exceed the vertical view angle of the camera by 45 degree
	if (yangle * TO_RADIANS > 45 || yangle * TO_RADIANS < -45) {
		yangle = 45.0f;
	}

}

void pressKey(unsigned char key, int xx, int yy) {

	// Speed of movement
	float speed = 0.10f;

	// Case switch
	switch (key) {

		// We're moving left/right/up/down with the keyboard. For example, to achieve a 360 degree rotation
		// based on the camera angle given by the mouse, we add to the x variable the z angle and 
		// the z the x one in the A (left) case. Same happens with D. 
		// With W/S, since we just want too move forward and backward the camera, we add the correct 
		// angle to the x and z variable.
	case 'A':
	case 'a':
		x += zangle * speed;
		z -= xangle * speed;
		break;
	case 'D':
	case 'd':
		x -= zangle * speed;
		z += xangle * speed;
		break;
	case 'W':
	case 'w':
		x += xangle * speed;
		z += zangle * speed;
		break;
	case 'S':
	case 's':
		x -= xangle * speed;
		z -= zangle * speed;
		break;

	case 'Q':
	case 'q':
		y += yangle * speed;
		break;

	case 'E':
	case 'e':
		y -= yangle * speed;
		break;
	case 27:
		exit(0);
		break;

	}
}


void draw_cylinder1(GLfloat radius,GLfloat height,GLubyte R,GLubyte G,GLubyte B)
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;

	/** Draw the tube */
	glColor3ub(R - 40, G - 40, B - 40);
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();

	/** Draw the circle on top of cylinder */
	glColor3ub(R, G, B);
	glBegin(GL_POLYGON);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glEnd();
}

void draw_cylinder2(GLfloat radius,GLfloat height,GLubyte R,GLubyte G,GLubyte B)
{
	GLfloat x = 0.0;
	GLfloat y = 0.0;
	GLfloat angle = 0.0;
	GLfloat angle_stepsize = 0.1;

	/** Draw the tube */
	glColor3ub(R - 40, G - 40, B - 40);
	glBegin(GL_QUAD_STRIP);
	angle = 0.0;
	while (angle < 2 * PI) {
		x = radius * cos(angle);
		y = radius * sin(angle);
		glVertex3f(x, y, height);
		glVertex3f(x, y, 0.0);
		angle = angle + angle_stepsize;
	}
	glVertex3f(radius, 0.0, height);
	glVertex3f(radius, 0.0, 0.0);
	glEnd();
}

void drawTable() {
	//table
	glBegin(GL_QUADS);
	glColor3f(0.0f,0.0f,0.0f);
	glVertex3f(3.0f, 0.5f, 5.0f);
	glVertex3f(3.0f, 2.1f, 5.0f);
	glVertex3f(-3.0, 2.1f, 5.0f);
	glVertex3f(-3.0, 0.5f, 5.0f);
	// BACK
	glVertex3f(-3.0f, 0.5f, -5.0f);
	glVertex3f(-3.0f, 2.1f, -5.0f);
	glVertex3f(3.0f, 2.1f, -5.0f);
	glVertex3f(3.0f, 0.5f, -5.0f);

	// LEFT
	glVertex3f(-3.0f, 1.3f, 5.0f);
	glVertex3f(-3.0f, 2.1f, 5.0f);
	glVertex3f(-3.0f, 2.1f, -5.0f);
	glVertex3f(-3.0f, 1.3f, -5.0f);
	// RIGHT
	glVertex3f(3.0f, 1.3f, -5.0f);
	glVertex3f(3.0f, 2.1f, -5.0f);
	glVertex3f(3.0f, 2.1f, 5.0f);
	glVertex3f(3.0f, 1.3f, 5.0f);
	// TOP
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-3.0f, 2.0f, 5.0f);
	glVertex3f(3.0f, 2.0f, 5.0f);
	glVertex3f(3.0f, 2.0f, -5.0f);
	glVertex3f(-3.0f, 2.0f, -5.0f);
	// BOTTOM
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-3.0f, 1.5f, 5.0f);
	glVertex3f(-3.0f, 1.5f, -5.0f);
	glVertex3f(3.0f, 1.5f, -5.0f);
	glVertex3f(3.0f, 1.5f, 5.0f);
	//left legs
	//back
	glVertex3f(-3.0f, 0.0f, -5.0f);
	glVertex3f(-3.0f, 2.0f, -5.0f);
	glVertex3f(-2.5f, 2.0f, -5.0f);
	glVertex3f(-2.5f, 0.0f, -5.0f);
	glVertex3f(-3.0f, 0.0f, -4.5F);
	glVertex3f(-3.0f, 2.0f, -4.5F);
	glVertex3f(-3.0f, 2.0f, -5.0f);
	glVertex3f(-3.0f, 0.0f, -5.0f);
	//front
	glVertex3f(-3.0f, 0.0f, 5.0f);
	glVertex3f(-3.0f, 2.0f, 5.0f);
	glVertex3f(-2.5f, 2.0f, 5.0f);
	glVertex3f(-2.5f, 0.0f, 5.0f);
	glVertex3f(-3.0f, 0.0f, 4.5F);
	glVertex3f(-3.0f, 2.0f, 4.5F);
	glVertex3f(-3.0f, 2.0f, 5.0f);
	glVertex3f(-3.0f, 0.0f, 5.0f);
	//right legs
	// back
	glVertex3f(3.0f, 0.0f, -5.0f);
	glVertex3f(3.0f, 2.0f, -5.0f);
	glVertex3f(2.5f, 2.0f, -5.0f);
	glVertex3f(2.5f, 0.0f, -5.0f);
	glVertex3f(3.0f, 0.0f, -4.5F);
	glVertex3f(3.0f, 2.0f, -4.5F);
	glVertex3f(3.0f, 2.0f, -5.0f);
	glVertex3f(3.0f, 0.0f, -5.0f);
	//front
	glVertex3f(3.0f, 0.0f, 5.0f);
	glVertex3f(3.0f, 2.0f, 5.0f);
	glVertex3f(2.5f, 2.0f, 5.0f);
	glVertex3f(2.5f, 0.0f, 5.0f);
	glVertex3f(3.0f, 0.0f, 4.5F);
	glVertex3f(3.0f, 2.0f, 4.5F);
	glVertex3f(3.0f, 2.0f, 5.0f);
	glVertex3f(3.0f, 0.0f, 5.0f);

	glEnd();
}

void drawWalls() {
	
	glBegin(GL_QUADS);
	glColor3ub(91, 4, 4);
	//side 1
	glVertex3f(10.0f, 0.0f,-10.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	//side 2
	glVertex3f(10.0f, 0.0f,10.0f);
	glVertex3f(-10.0f, 0.0f, 10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);
	//side 3
	glVertex3f(-10.0f, 0.0f, 10.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	//side 4
	glVertex3f(10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, 10.0f);
	glVertex3f(10.0f, 10.0f, -10.0f);
	glEnd();
}

void drawRoof() {
	glBegin(GL_QUADS);
	glColor3ub(0, 0, 0);
	//row 1
	glVertex3f(10.0f, 10.0f,6.0f);
	glVertex3f(8.0f, 10.0f, 6.0f);
	glVertex3f(8.0f, 10.0f, 10.0f);
	glVertex3f(10.0f,10.0f, 10.0f);

	glVertex3f(10.0f, 10.0f, -2.0f);
	glVertex3f(8.0f, 10.0f, -2.0f);
	glVertex3f(8.0f, 10.0f, 2.0f);
	glVertex3f(10.0f, 10.0f,2.0f);

	glVertex3f(10.0f, 10.0f, -10.0f);
	glVertex3f(8.0f, 10.0f, -10.0f);
	glVertex3f(8.0f, 10.0f, -6.0f);
	glVertex3f(10.0f, 10.0f, -6.0f);
	//row 2
	glVertex3f(8.0f, 10.0f, 2.0f);
	glVertex3f(6.0f, 10.0f, 2.0f);
	glVertex3f(6.0f, 10.0f, 6.0f);
	glVertex3f(8.0f, 10.0f, 6.0f);

	glVertex3f(8.0f, 10.0f, -6.0f);
	glVertex3f(6.0f, 10.0f, -6.0f);
	glVertex3f(6.0f, 10.0f, -2.0f);
	glVertex3f(8.0f, 10.0f, -2.0f);
	//row 3
	glVertex3f(6.0f, 10.0f, 6.0f);
	glVertex3f(4.0f, 10.0f, 6.0f);
	glVertex3f(4.0f, 10.0f, 10.0f);
	glVertex3f(6.0f, 10.0f, 10.0f);

	glVertex3f(6.0f, 10.0f, -2.0f);
	glVertex3f(4.0f, 10.0f, -2.0f);
	glVertex3f(4.0f, 10.0f, 2.0f);
	glVertex3f(6.0f, 10.0f, 2.0f);

	glVertex3f(6.0f, 10.0f, -10.0f);
	glVertex3f(4.0f, 10.0f, -10.0f);
	glVertex3f(4.0f, 10.0f, -6.0f);
	glVertex3f(6.0f, 10.0f, -6.0f);
	//row 4
	glVertex3f(4.0f, 10.0f, 2.0f);
	glVertex3f(2.0f, 10.0f, 2.0f);
	glVertex3f(2.0f, 10.0f, 6.0f);
	glVertex3f(4.0f, 10.0f, 6.0f);

	glVertex3f(4.0f, 10.0f, -6.0f);
	glVertex3f(2.0f, 10.0f, -6.0f);
	glVertex3f(2.0f, 10.0f, -2.0f);
	glVertex3f(4.0f, 10.0f, -2.0f);

	//row 5
	glVertex3f(2.0f, 10.0f, 6.0f);
	glVertex3f(0.0f, 10.0f, 6.0f);
	glVertex3f(0.0f, 10.0f, 10.0f);
	glVertex3f(2.0f, 10.0f, 10.0f);

	glVertex3f(2.0f, 10.0f, -2.0f);
	glVertex3f(0.0f, 10.0f, -2.0f);
	glVertex3f(0.0f, 10.0f, 2.0f);
	glVertex3f(2.0f, 10.0f, 2.0f);

	glVertex3f(2.0f, 10.0f, -10.0f);
	glVertex3f(0.0f, 10.0f, -10.0f);
	glVertex3f(0.0f, 10.0f, -6.0f);
	glVertex3f(2.0f, 10.0f, -6.0f);
	//row 6
	glVertex3f(0.0f, 10.0f, 2.0f);
	glVertex3f(-2.0f, 10.0f, 2.0f);
	glVertex3f(-2.0f, 10.0f, 6.0f);
	glVertex3f(0.0f, 10.0f, 6.0f);

	glVertex3f(0.0f, 10.0f, -6.0f);
	glVertex3f(-2.0f, 10.0f, -6.0f);
	glVertex3f(-2.0f, 10.0f, -2.0f);
	glVertex3f(0.0f, 10.0f, -2.0f);

	//row7
	glVertex3f(-2.0f, 10.0f, 6.0f);
	glVertex3f(-4.0f, 10.0f, 6.0f);
	glVertex3f(-4.0f, 10.0f, 10.0f);
	glVertex3f(-2.0f, 10.0f, 10.0f);

	glVertex3f(-2.0f, 10.0f, -2.0f);
	glVertex3f(-4.0f, 10.0f, -2.0f);
	glVertex3f(-4.0f, 10.0f, 2.0f);
	glVertex3f(-2.0f, 10.0f, 2.0f);

	glVertex3f(-2.0f, 10.0f, -10.0f);
	glVertex3f(-4.0f, 10.0f, -10.0f);
	glVertex3f(-4.0f, 10.0f, -6.0f);
	glVertex3f(-2.0f, 10.0f, -6.0f);
	//row 8
	glVertex3f(-4.0f, 10.0f, 2.0f);
	glVertex3f(-6.0f, 10.0f, 2.0f);
	glVertex3f(-6.0f, 10.0f, 6.0f);
	glVertex3f(-4.0f, 10.0f, 6.0f);

	glVertex3f(-4.0f, 10.0f, -6.0f);
	glVertex3f(-6.0f, 10.0f, -6.0f);
	glVertex3f(-6.0f, 10.0f, -2.0f);
	glVertex3f(-4.0f, 10.0f, -2.0f);


	//row9
	glVertex3f(-6.0f, 10.0f, 6.0f);
	glVertex3f(-8.0f, 10.0f, 6.0f);
	glVertex3f(-8.0f, 10.0f, 10.0f);
	glVertex3f(-6.0f, 10.0f, 10.0f);

	glVertex3f(-6.0f, 10.0f, -2.0f);
	glVertex3f(-8.0f, 10.0f, -2.0f);
	glVertex3f(-8.0f, 10.0f, 2.0f);
	glVertex3f(-6.0f, 10.0f, 2.0f);

	glVertex3f(-6.0f, 10.0f, -10.0f);
	glVertex3f(-8.0f, 10.0f, -10.0f);
	glVertex3f(-8.0f, 10.0f, -6.0f);
	glVertex3f(-6.0f, 10.0f, -6.0f);
	//row 10
	glVertex3f(-8.0f, 10.0f, 2.0f);
	glVertex3f(-10.0f, 10.0f, 2.0f);
	glVertex3f(-10.0f, 10.0f, 6.0f);
	glVertex3f(-8.0f, 10.0f, 6.0f);

	glVertex3f(-8.0f, 10.0f, -6.0f);
	glVertex3f(-10.0f, 10.0f, -6.0f);
	glVertex3f(-10.0f, 10.0f, -2.0f);
	glVertex3f(-8.0f, 10.0f, -2.0f);


	glColor3ub(90, 76, 76);
	//row 1
	glVertex3f(10.0f, 10.0f, 2.0f);
	glVertex3f(8.0f, 10.0f, 2.0f);
	glVertex3f(8.0f, 10.0f, 6.0f);
	glVertex3f(10.0f, 10.0f, 6.0f);

	glVertex3f(10.0f, 10.0f, -6.0f);
	glVertex3f(8.0f, 10.0f, -6.0f);
	glVertex3f(8.0f, 10.0f, -2.00f);
	glVertex3f(10.0f, 10.0f, -2.0f);

	//row 2

	glVertex3f(8.0f, 10.0f, 6.0f);
	glVertex3f(6.0f, 10.0f, 6.0f);
	glVertex3f(6.0f, 10.0f, 10.0f);
	glVertex3f(8.0f, 10.0f, 10.0f);

	glVertex3f(8.0f, 10.0f, -2.0f);
	glVertex3f(6.0f, 10.0f, -2.0f);
	glVertex3f(6.0f, 10.0f, 2.0f);
	glVertex3f(8.0f, 10.0f, 2.0f);

	glVertex3f(8.0f, 10.0f, -10.0f);
	glVertex3f(6.0f, 10.0f, -10.0f);
	glVertex3f(6.0f, 10.0f, -6.0f);
	glVertex3f(8.0f, 10.0f, -6.0f);

	//row 3
	glVertex3f(6.0f, 10.0f, 2.0f);
	glVertex3f(4.0f, 10.0f, 2.0f);
	glVertex3f(4.0f, 10.0f, 6.0f);
	glVertex3f(6.0f, 10.0f, 6.0f);

	glVertex3f(6.0f, 10.0f, -6.0f);
	glVertex3f(4.0f, 10.0f, -6.0f);
	glVertex3f(4.0f, 10.0f, -2.00f);
	glVertex3f(6.0f, 10.0f, -2.0f);

	//row 4

	glVertex3f(4.0f, 10.0f, 6.0f);
	glVertex3f(2.0f, 10.0f, 6.0f);
	glVertex3f(2.0f, 10.0f, 10.0f);
	glVertex3f(4.0f, 10.0f, 10.0f);

	glVertex3f(4.0f, 10.0f, -2.0f);
	glVertex3f(2.0f, 10.0f, -2.0f);
	glVertex3f(2.0f, 10.0f, 2.0f);
	glVertex3f(4.0f, 10.0f, 2.0f);

	glVertex3f(4.0f, 10.0f, -10.0f);
	glVertex3f(2.0f, 10.0f, -10.0f);
	glVertex3f(2.0f, 10.0f, -6.0f);
	glVertex3f(4.0f, 10.0f, -6.0f);

	//row 5
	glVertex3f(2.0f, 10.0f, 2.0f);
	glVertex3f(0.0f, 10.0f, 2.0f);
	glVertex3f(0.0f, 10.0f, 6.0f);
	glVertex3f(2.0f, 10.0f, 6.0f);

	glVertex3f(2.0f, 10.0f, -6.0f);
	glVertex3f(0.0f, 10.0f, -6.0f);
	glVertex3f(0.0f, 10.0f, -2.00f);
	glVertex3f(2.0f, 10.0f, -2.0f);

	//row 6

	glVertex3f(0.0f, 10.0f, 6.0f);
	glVertex3f(-2.0f, 10.0f, 6.0f);
	glVertex3f(-2.0f, 10.0f, 10.0f);
	glVertex3f(0.0f, 10.0f, 10.0f);

	glVertex3f(0.0f, 10.0f, -2.0f);
	glVertex3f(-2.0f, 10.0f, -2.0f);
	glVertex3f(-2.0f, 10.0f, 2.0f);
	glVertex3f(0.0f, 10.0f, 2.0f);

	glVertex3f(0.0f, 10.0f, -10.0f);
	glVertex3f(-2.0f, 10.0f, -10.0f);
	glVertex3f(-2.0f, 10.0f, -6.0f);
	glVertex3f(0.0f, 10.0f, -6.0f);

	//row 7
	glVertex3f(-2.0f, 10.0f, 2.0f);
	glVertex3f(-4.0f, 10.0f, 2.0f);
	glVertex3f(-4.0f, 10.0f, 6.0f);
	glVertex3f(-2.0f, 10.0f, 6.0f);

	glVertex3f(-2.0f, 10.0f, -6.0f);
	glVertex3f(-4.0f, 10.0f, -6.0f);
	glVertex3f(-4.0f, 10.0f, -2.00f);
	glVertex3f(-2.0f, 10.0f, -2.0f);

	//row 8

	glVertex3f(-4.0f, 10.0f, 6.0f);
	glVertex3f(-6.0f, 10.0f, 6.0f);
	glVertex3f(-6.0f, 10.0f, 10.0f);
	glVertex3f(-4.0f, 10.0f, 10.0f);

	glVertex3f(-4.0f, 10.0f, -2.0f);
	glVertex3f(-6.0f, 10.0f, -2.0f);
	glVertex3f(-6.0f, 10.0f, 2.0f);
	glVertex3f(-4.0f, 10.0f, 2.0f);

	glVertex3f(-4.0f, 10.0f, -10.0f);
	glVertex3f(-6.0f, 10.0f, -10.0f);
	glVertex3f(-6.0f, 10.0f, -6.0f);
	glVertex3f(-4.0f, 10.0f, -6.0f);

	//row 9
	glVertex3f(-6.0f, 10.0f, 2.0f);
	glVertex3f(-8.0f, 10.0f, 2.0f);
	glVertex3f(-8.0f, 10.0f, 6.0f);
	glVertex3f(-6.0f, 10.0f, 6.0f);

	glVertex3f(-6.0f, 10.0f, -6.0f);
	glVertex3f(-8.0f, 10.0f, -6.0f);
	glVertex3f(-8.0f, 10.0f, -2.00f);
	glVertex3f(-6.0f, 10.0f, -2.0f);

	//row 10

	glVertex3f(-8.0f, 10.0f, 6.0f);
	glVertex3f(-10.0f, 10.0f, 6.0f);
	glVertex3f(-10.0f, 10.0f, 10.0f);
	glVertex3f(-8.0f, 10.0f, 10.0f);

	glVertex3f(-8.0f, 10.0f, -2.0f);
	glVertex3f(-10.0f, 10.0f, -2.0f);
	glVertex3f(-10.0f, 10.0f, 2.0f);
	glVertex3f(-8.0f, 10.0f, 2.0f);

	glVertex3f(-8.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -10.0f);
	glVertex3f(-10.0f, 10.0f, -6.0f);
	glVertex3f(-8.0f, 10.0f, -6.0f);

	glEnd();
}

void drawScoreboard() {
	glBegin(GL_QUADS);
	// TOP
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(4.0f, 3.0f, -1.0f);
	glVertex3f(3.0f, 3.0f, -1.0f);
	glVertex3f(3.0f, 3.0f, 1.0f);
	glVertex3f(4.0f, 3.0f, 1.0f);
	//bottom
	glVertex3f(4.0f, 2.0f, -5.0f);
	glVertex3f(3.0f, 2.0f, -5.0f);
	glVertex3f(3.0f, 2.0f, 5.0f);
	glVertex3f(4.0f, 2.0f, 5.0f);
	//right
	glVertex3f(4.0f, 2.0f, 1.0f);
	glVertex3f(3.0f, 2.0f, 1.0f);
	glVertex3f(3.0f, 3.0f, 1.0f);
	glVertex3f(4.0f, 3.0f, 1.0f);
	//left
	glVertex3f(4.0f, 2.0f, -1.0f);
	glVertex3f(3.0f, 2.0f, -1.0f);
	glVertex3f(3.0f, 3.0f, -1.0f);
	glVertex3f(4.0f, 3.0f,-1.0f);
	//back
	glVertex3f(4.0f, 2.0f, -1.0f);
	glVertex3f(4.0f, 3.0f, -1.0f);
	glVertex3f(4.0f, 3.0f, 1.0f);
	glVertex3f(4.0f, 2.0f, 1.0f);
	//screen
	glColor3ub(90, 76, 76);
	glVertex3f(3.0f, 2.2f, -0.8f);
	glVertex3f(3.0f, 2.8f, -0.8f);
	glVertex3f(3.0f, 2.8f, 0.8f);
	glVertex3f(3.0f, 2.2f, 0.8f);
	glEnd();
}

void drawScene(void) {

	// Clearing procedures 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(-1.0, 1.0, 1.0);
	glLoadIdentity();

	// Looking at the coordinate calculated by moving mouse/keyboard. 
	// The first three parameters: the camera position vector. 
	// The second three parameters: the point we're looking at 
	// Thee last three parameters: the UP vector 
	gluLookAt(x, y, z, x + xangle, y - yangle, z + zangle, 0.0f, y, 0.0f);

	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0f, 0.0f, 10.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f, 0.0f, 10.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f, 0.0f, -10.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0f, 0.0f, -10.0f);
	glEnd();

	drawWalls();
	drawRoof();
	drawTable();
	drawScoreboard();
	glTranslatef(0.0, 2.0, 0.0);
	glRotatef(-90.0, 1.0,0.0, 0.0);

	//STRIKER
	draw_cylinder1(0.6, 0.05, 255, 150, 100);
	draw_cylinder1(0.3, 0.5, 255, 150, 100);
	draw_cylinder2(0.6, 0.2, 255, 150, 100);

	//puck
	glTranslatef(0.0,2.0, 0.0);
	
	draw_cylinder2(.6025, 0.075, 255, 150, 100);
	draw_cylinder1(0.6, 0.05, 2, 17, 122);

	//STRIKER 2
	glRotatef(-10.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.5, 0.15);
	draw_cylinder1(0.6, 0.05, 255, 150, 100);
	draw_cylinder1(0.3, 0.5, 255, 150, 100);
	draw_cylinder2(0.6, 0.2, 255, 150, 100);


	// Swap front and back buffer
	glutSwapBuffers();

}

void resize(int w, int h) {

	// Setting ratio and new window size
	float ratio = w * 1.0 / h;
	ww = w;
	hh = h;

	// Setting the viewport
	glViewport(0, 0, w, h);

	// Setting the projection 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	//glOrtho(-10.0, 10.0, -10.0, 10.0, 0.0, 10.0); 

	// Resettign the modelview matrix 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Redisplay the scene
	glutPostRedisplay();

}

void update(int value) {

	// Timer function in which we redisplay the content of the screen each 1000/60 
	// msec, meaning that we want to achieve a 60fps configuration.
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, update, 0);
}

void setup(void) {

	// Clearing background color and setting up to black
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Other init procedures if needed

}

void main(int argc, char** argv) {

	// Init procedures
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1000, 720);

	// Creating a window 
	glutCreateWindow("Final Project.cpp");

	// Func callback
	glutReshapeFunc(resize);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(pressKey);
	glutPassiveMotionFunc(mouseMove);

	// here are the new entries
	glutTimerFunc(100, update, 0);

	// Setup func 
	setup();
	initGL();
	// start loop
	glutMainLoop();

}