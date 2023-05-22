#include <GL/glut.h>
#include <GL/glu.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <iostream>


float playerX 		   = 0.5f; // posição x inicial do jogador
float playerY 		   = -0.2f; // posição y inicial do jogador
float playerSpeed 	   = 0.1f; // velocidade de movimento do jogador
float playerDirection  = 0.0f;

int life 		   = 1;
int frameNumber    = 0;
float width  	   = 700;
float height 	   = 500;
float aspect 	   = width / height;
float x_min        = 0;
float x_max 	   = 7.5;
float snakeX 	   = playerX + 1;
float snakeY	   = 0;
float lifeX    	   = 0;
float lifeY    	   = 0;
float elapsedTime  = 0.0f;
bool isJumping     = false;
bool snakeVisible  = false;
bool lifeVisible   = false;
bool gameOver 	   = false;

// Estrutura para representar as coordenadas da cobra
struct Point {
    double x;
    float y;
};

std::vector<Point> snake;  // Vetor de pontos que representa a cobra
float snakeSpeed    = 0.05f;  // Velocidade de movimento da cobra

void updatePlayerPosition() {

    // atualiza a posição do jogador com base na velocidade e direção
    float dx = playerSpeed * cosf(playerDirection * 3.14159f / 180.0f);
    float dy = playerSpeed * sinf(playerDirection * 3.14159f / 180.0f);
    playerX += dx;

    float currentY =  playerY;
    playerY 	  += dy;

    if(playerY > 0.2){
    	playerY = currentY;
    }

    if(playerY < -0.2){
    	playerY = currentY;
    }

    if(playerX > x_max){
        playerX = x_min;
    }

}

void specialKeys(int key, int x, int y) {
    switch(key) {
       case GLUT_KEY_LEFT:
    	    playerDirection = 25.0f;
    	    updatePlayerPosition();
            break;
        case GLUT_KEY_RIGHT:
            playerDirection = 0.0f;
            updatePlayerPosition();
            break;
        case GLUT_KEY_UP:
            isJumping = true;
            break;
        case GLUT_KEY_DOWN:
        	playerDirection = -25.0f;
        	updatePlayerPosition();
            break;
    }
}

void drawSquare(int x, int y) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + 0.1, y);
    glVertex2f(x + 0.1, y + 0.35);
    glVertex2f(x, y + 0.35);
    glEnd();
}

void drawSnake(){

	snakeY -= 0.003;
	glColor3f(255, 0, 0);
	for (const auto& p : snake) {
		drawSquare(p.x, p.y + snakeY);
	}

	if(snakeY <= -1)
	{
		snakeVisible = false;
		snakeY	     = 0;
	}

}


void DrawCircle(float cx, float cy, float r, int num_segments)
{
	glBegin(GL_POLYGON);
	for(int i = 0; i < num_segments; i++)
	{
		float theta = 2.0f *3.1415926f * float(i) / float(num_segments);
		float x = r * cosf(theta);
		float y = r * sinf(theta);

		glVertex2f(x + cx, y + cy);
	}

	glEnd();
}

void drawPlayer()
{
	// Desenhar o corpo
	glColor3f(1,1,0);
	glBegin(GL_POLYGON);
	glVertex2f(playerX, playerY);
	glVertex2f(playerX + 0.1f, playerY);
	glVertex2f(playerX + 0.1f, playerY - 0.2f);
	glVertex2f(playerX, playerY - 0.2f);
	glEnd();

	// Desenhar a cabeça
	DrawCircle(playerX + 0.05f, playerY + 0.05f, 0.05, 1000);

	// Braço direito
	glBegin(GL_POLYGON);
	glVertex2f(playerX + 0.02f, playerY - 0.02f);
	glVertex2f(playerX + 0.18f, playerY - 0.08f);
	glVertex2f(playerX + 0.2f, playerY - 0.05f);
	glVertex2f(playerX + 0.05f, playerY);
	glEnd();

	// Braço esquerdo
	glBegin(GL_POLYGON);
	glVertex2f(playerX - 0.09f, playerY - 0.08f);
	glVertex2f(playerX + 0.04f, playerY - 0.03f);
	glVertex2f(playerX + 0.05f, playerY);
	glVertex2f(playerX - 0.1f, playerY - 0.05f);
	glEnd();

	// Perna Direita
	glBegin(GL_POLYGON);
	glVertex2f(playerX + 0.05f, playerY - 0.2f); // C
	glVertex2f(playerX + 0.13f, playerY - 0.29f); // D
	glVertex2f(playerX + 0.18f, playerY - 0.29f); // B
	glVertex2f(playerX + 0.1f, playerY - 0.2f); //A
	glEnd();

	// Perna Esquerda
	glBegin(GL_POLYGON);
	glVertex2f(playerX - 0.08f, playerY - 0.29f); // E
	glVertex2f(playerX - 0.03f, playerY - 0.29f); // G
	glVertex2f(playerX + 0.05f, playerY - 0.2f); // C
	glVertex2f(playerX, playerY - 0.2f); //F
	glEnd();
}

void drawCloud()
{
    glPushMatrix();

    int num_segments = 1000;
    glColor3f(1.0f, 1.0f, 1.0f);

    // First cloud
    DrawCircle(0.8, 2.5, 0.1, num_segments); // - ponta
    DrawCircle(0.95, 2.55, 0.15, num_segments);
    DrawCircle(1.1, 2.5, 0.1, num_segments); // - ponta

    // Second cloud
    DrawCircle(2.8, 2.6, 0.1, num_segments); // - ponta
    DrawCircle(3, 2.75, 0.11, num_segments);
    DrawCircle(2.9, 2.67, 0.11, num_segments);
    DrawCircle(3.15, 2.62, 0.12, num_segments); // - ponta
    DrawCircle(2.98, 2.65, 0.11, num_segments);
    glBegin(GL_POLYGON);
    glVertex2f(2.82, 2.5);
    glVertex2f(2.82, 2.6);
    glVertex2f(3.15, 2.5);
    glVertex2f(3.15, 2.6);
    glEnd();

    // Last cloud
    DrawCircle(5.8, 3.2, 0.12, num_segments); // - ponta
    DrawCircle(5.95, 3.3, 0.12, num_segments);
    DrawCircle(6.1, 3.35, 0.14, num_segments);
    DrawCircle(6.25, 3.3, 0.12, num_segments);
    DrawCircle(6.4, 3.2, 0.12, num_segments); // - ponta
    DrawCircle(6.25, 3.14, 0.14, num_segments);
    DrawCircle(6.1, 3.15, 0.13, num_segments);
    DrawCircle(5.95, 3.14, 0.14, num_segments);

    glPopMatrix();
}

void drawGrass()
{

	glPushMatrix();

	int num_segments = 1000;
	glColor3f(0, 1, 0);

	// First
	DrawCircle(3.15, 0.09, 0.25, num_segments);
	DrawCircle(3.3, 0.11, 0.3, num_segments);
	DrawCircle(3.4, 0.06, 0.28, num_segments);
	DrawCircle(3.6, -0.03, 0.28, num_segments);

	// Last
	DrawCircle(5.99, 0.12, 0.3, num_segments);
	DrawCircle(6.17, 0.09, 0.23, num_segments);
	DrawCircle(6.32, -0.02, 0.22, num_segments);

	glPopMatrix();
}

void drawSun() {

	glColor3f(1,1,0);

	DrawCircle(3.7, 0.45, 0.28, 1000);
}

void drawWindmill(float cx, float cy, float size)
{
	glColor3f(0.5, 0.5, 0.55);
	glBegin(GL_POLYGON);
	glVertex2f(cx, 0);
	glVertex2f(cx + 0.12, 0);
	glVertex2f(cx + 0.08, cy);
	glVertex2f(cx + 0.04, cy);
	glEnd();

	glColor3f(0.0f, 0.3f, 0.6f);
	DrawCircle(cx + 0.03, cy - 0.05, 0.08, 1000);

	glPushMatrix();
	glTranslatef(cx + 0.03, cy - 0.05, 0);
	glRotatef(float(frameNumber)/5, 0, 0, 1);
	for (int i = 0; i < 3; i++) {
		glRotated(120, 0, 0, 1);
	    glBegin(GL_POLYGON);
	    glVertex2f(0,0);
	    glVertex2f(0, 0.05f);
	    glVertex2f(size, 0);
	    glVertex2f(0, -0.05f);
	    glEnd();
	}

	glPopMatrix();
}

void drawFloor()
{
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(7.5, 0);
	glVertex2f(7.5, -0.1);
	glVertex2f(0, -0.1);
	glEnd();

	glColor3f(0.4f, 0.4f, 0.5f);
	glBegin(GL_QUADS);
	glVertex2f(0, -0.1);
	glVertex2f(7.5, -0.1);
	glVertex2f(7.5, -0.5);
	glVertex2f(0, -0.5);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(0, -0.5);
	glVertex2f(7.5, -0.5);
	glVertex2f(7.5, -1);
	glVertex2f(0, -1);
	glEnd();
}

void checkSnakeCollision() {


	if(snake[0].x >= (playerX - 0.02) && snake[0].x <= (playerX + 0.1) && (snake[0].y + snakeY) > -1){
		life--;
		snakeVisible = false;
	}

	//std::cout << "Start player Y: " << playerX - 0.03 << std::endl;
	//std::cout << "Snake head x: " << snake[0].x << std::endl;
	//std::cout << "End player Y: " << playerX + 0.1 << std::endl;
	//std::cout << "Snake head y: " << snake[0].y + snakeY << std::endl;
	//std::cout << "End Snake head x: " << snake[0].x << std::endl;

}
void checkLifeCollision(){
	if(playerX >= lifeX - 0.3 && playerX <= lifeX + 0.3){
		life++;
	  	lifeVisible = false;
	}
}

void drawLife(){

	glColor3f(218, 165, 32);
	DrawCircle(lifeX, lifeY, 0.05, 1000);
}

void gameOverScreen() {
	glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(2.5f, 2.5f);

    std::string gameOverMsg = "Game Over";
    for (const char& letter : gameOverMsg) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);
    }
}

void livesScreen() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(0.1f, 3.5f);

    std::string livesMsg = "Vidas: " + std::to_string(life);
    for (const char& letter : livesMsg) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, letter);
    }
}


void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // Fills the scene with blue.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// First mountain with snow
	glColor3f(0.5, 0.5, 0.55);
	glBegin(GL_POLYGON);
	glVertex2f(0.5f, 0);
	glVertex2f(1.5f, 1.5f);
	glVertex2f(2.5, 0);
	glEnd();

	// Second mountain with snow
	glColor3f(0.0f, 0.3f, 0.6f);
	glBegin(GL_POLYGON);
	glVertex2f(1.5, 0);
	glVertex2f(2.25, 0.9);
	glVertex2f(3, 0);
	glEnd();

	drawSun();

	drawWindmill(2.9f, 1.5f, 0.9f);
	drawWindmill(3.7f, 0.85f, 0.9f);

	// Mountain
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(0.8, 0);
	glVertex2f(1.4f, 0.35f);
	glVertex2f(2.7, 0.35f);
	glVertex2f(3, 0);
	glEnd();

	// Mountain
	glBegin(GL_POLYGON);
	glVertex2f(3.8, 0);
	glVertex2f(4.3f, 0.35f);
	glVertex2f(5.8, 0.35f);
	glVertex2f(6.2f, 0);
	glEnd();

	// Third mountain with snow
	glColor3f(0.5, 0.5, 0.55);
	glBegin(GL_POLYGON);
	glVertex2f(4.8, 0);
	glVertex2f(5.5f, 1);
	glVertex2f(6.2, 0);
	glEnd();

	drawWindmill(6.3f, 0.85f, 0.75f);

	// Snow
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(5.2, 0.59f);
	glVertex2f(5.5f, 1);
	glVertex2f(5.8, 0.59f);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(5.2, 0.59);
	glVertex2f(5.28, 0.5);
	glVertex2f(5.36, 0.59);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(5.45, 0.59);
	glVertex2f(5.53, 0.52);
	glVertex2f(5.61, 0.59);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(5.61, 0.59);
	glVertex2f(5.69, 0.52);
	glVertex2f(5.77, 0.59);
	glEnd();

	drawCloud();
	drawFloor();

	if(isJumping)
	{
		glPushMatrix();
		glTranslatef(0, playerY + 0.7f, 0);
	}

	drawPlayer();
	if(isJumping)
	{
		isJumping = false;
		glPopMatrix();
	}

	if (snakeVisible) {
		drawSnake();
		checkSnakeCollision();
	}

	if (lifeVisible) {
	    // Verificar se passaram 5 segundos
	    if (elapsedTime >= 5 * 60) {  // 5 segundos (assumindo que a taxa de atualização é de 60 frames por segundo)
	        lifeVisible = false;
	    }

	    checkLifeCollision();

	    if (lifeVisible) {
	       drawLife();
	    }
	}

	if(life == 0){
		gameOver = true;
		gameOverScreen();
		glutSwapBuffers();
		return;
	}

	livesScreen();

	glutSwapBuffers();
	glutPostRedisplay();
}

void init() {
	glClearColor(0.5f, 0.5f, 1, 1);
	// The next three lines set up the coordinates system.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( x_min,x_max, -1.0, 4.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void doFrame(int v) {

	if(gameOver){
		return;
	}

    frameNumber++;

    if (frameNumber % 200 == 0)
    {
    	float randomValue = (std::sin(frameNumber * 0.1) + 1.0) * 0.5;
    	// Definir se a cobra deve ser mostrada ou escondida
    	if (randomValue < 0.6 && !snakeVisible) {
    		snakeVisible  = true;
    		snakeX 		  = playerX + 1;
    		snake.clear();
    		for (int i = 0; i < 5; i++) {
    			Point p = {snakeX, -0.5};
    			snake.push_back(p);
    		}
    	}

    	if(randomValue >= 0.6 && !lifeVisible){
    		lifeVisible  = true;
    		lifeX 		 = playerX + 1.5;
    		if(lifeX > x_max){
    			lifeX    = 1.5;
    		}

    		lifeY 		 = std::sin(frameNumber * 0.1f) * -0.2f - 0.3f;
    		elapsedTime  = 0;
    	}

    }

    if(lifeVisible){
    	elapsedTime++;
    }

    glutPostRedisplay();
    glutTimerFunc(20,doFrame,0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(100,100);
    glutCreateWindow("2D Animation");

    init();

    glutSpecialFunc(specialKeys);
    glutDisplayFunc(display);

    glutTimerFunc(20,doFrame,0);
    glutMainLoop();
    return 0;
}
