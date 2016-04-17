#pragma once

#include "Common.h"

struct CameraInfo {
	// Перевести координаты вершин из мировой системы координат в систему координат виртуальной камеры
	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;
};

class Application {
public:
	Application( bool hasGUI = true );
	~Application();

	/**
	Запускает приложение
	*/
	void Start();

	/**
	Обрабатывает нажатия кнопок на клавитуре.
	См. сигнатуру GLFWkeyfun библиотеки GLFW
	*/
	virtual void HandleKey( int key, int scancode, int action, int mods );

	/**
	Обрабатывает движение мышки
	*/
	virtual void HandleMouseMove( double xpos, double ypos );

	/**
	Обрабатывает колесико мыши
	*/
	virtual void HandleScroll( double xoffset, double yoffset );

protected:
	/**
	Инициализирует графический контекст
	*/
	virtual void InitContext();

	/**
	Настраивает некоторые параметры OpenGL
	*/
	virtual void InitGL();

	/**
	Инициализирует графический интерфейс пользователя
	*/
	virtual void InitGUI();

	/**
	Создает трехмерную сцену
	*/
	virtual void MakeScene();

	/**
	Запускает цикл рендеринга
	*/
	void Run();

	/**
	Выполняет обновление сцены и виртуальной камеры
	*/
	virtual void Update();

	/**
	Выполняет обновление графического интерфейса пользователя
	*/
	virtual void UpdateGUI();

	/**
	Отрисовывает один кадр
	*/
	virtual void Draw();

	/**
	Отрисовывает графический интерфейс пользователя
	*/
	virtual void DrawGUI();

	//---------------------------------------------

	GLFWwindow* window; //Графичекое окно

	CameraInfo camera;

	//Положение виртуальный камеры задается в сферических координат
	double phiAng;
	double thetaAng;
	double r;

	//Время на предыдущем кадре
	double oldTime;

	//Положение курсора мыши на предыдущем кадре
	double oldXPos;
	double oldYPos;

	bool hasGUI;
};
