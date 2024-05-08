#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();
	
	
	//Movimientos personajes
	//Depresso
	GLfloat getgiroIzDepress() { return giroIzDepress; }
	GLfloat getgiroDeDepress() { return giroDeDepress; }
	GLfloat getmovDepress() { return movDepress; }
	//Sonic
	GLfloat getgiroIzSonic() { return giroIzSonic; }
	GLfloat getgiroDeSonic() { return giroDeSonic; }
	GLfloat getmovSonic() { return movSonic; }

	//Camara
	GLfloat getopcion() { return opcion; }

	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	

	//Movimiento Personajes
	//Depresso
	GLfloat giroIzDepress;
	GLfloat giroDeDepress;
	GLfloat movDepress;
	//Sonic
	GLfloat giroIzSonic;
	GLfloat giroDeSonic;
	GLfloat movSonic;
	//Camara opcion
	GLfloat opcion;

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

