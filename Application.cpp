#include "Application.hpp"
#include "Common.h"

//Функция обратного вызова для обработки нажатий на клавиатуре
void keyCallback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
	Application* app = ( Application* ) glfwGetWindowUserPointer( window );
	app->HandleKey( key, scancode, action, mods );
}

void windowSizeChangedCallback( GLFWwindow* window, int width, int height )
{
}

void mouseButtonPressedCallback( GLFWwindow* window, int button, int action, int mods )
{
}

void mouseCursosPosCallback( GLFWwindow* window, double xpos, double ypos )
{
	Application* app = ( Application* ) glfwGetWindowUserPointer( window );
	app->HandleMouseMove( xpos, ypos );
}

void scrollCallback( GLFWwindow* window, double xoffset, double yoffset )
{
	Application* app = ( Application* ) glfwGetWindowUserPointer( window );
	app->HandleScroll( xoffset, yoffset );
}

Application::Application( bool hasGUI ):
	oldTime( 0.0 ),
	phiAng( 0.0 ),
	thetaAng( 0.0 ),
	r( 5.0 ),
	hasGUI( hasGUI ),
	oldXPos( 0 ),
	oldYPos( 0 )
{
}

Application::~Application()
{
	if( hasGUI ) {
		ImGui_ImplGlfwGL3_Shutdown();
	}
	glfwTerminate();
}

void Application::Start()
{
	InitContext();

	InitGL();

	if( hasGUI ) {
		InitGUI();
	}

	MakeScene();

	Run();
}

void Application::InitContext()
{
	if( !glfwInit() ) {
		std::cerr << "ERROR: could not start GLFW3\n";
		exit( 1 );
	}

#ifdef USE_CORE_PROFILE
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#endif

	int count;
	GLFWmonitor** monitors = glfwGetMonitors( &count );

	if( count == 2 ) {
		// Если 2 монитора, то запускаем приложение в полноэкранном режиме на 2м мониторе   
		// Создаем графический контекст (окно)
		window = glfwCreateWindow( 1920, 1080, "DZvonarev 296 group: Klein Bottle", monitors[1], NULL );
	} else {
		window = glfwCreateWindow( 800, 600, "DZvonarev 296 group: Klein Bottle", NULL, NULL );
	}
	if( !window ) {
		std::cerr << "ERROR: could not open window with GLFW3\n";
		glfwTerminate();
		exit( 1 );
	}
	// Выбираем этот контекст
	glfwMakeContextCurrent( window );

	glfwSwapInterval( 0 ); //Отключаем вертикальную синхронизацию

	//Регистрируем указатель на данный объект, 
	//чтобы потом использовать его в функциях обратного вызова
	glfwSetWindowUserPointer( window, this );

	//Регистрирует функцию обратного вызова для обработки событий клавиатуры
	// ресайза, мыши, движения курсора и скролла
	glfwSetKeyCallback( window, keyCallback );
	glfwSetWindowSizeCallback( window, windowSizeChangedCallback );
	glfwSetMouseButtonCallback( window, mouseButtonPressedCallback );
	glfwSetCursorPosCallback( window, mouseCursosPosCallback );
	glfwSetScrollCallback( window, scrollCallback );
}

void Application::InitGL()
{
	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString( GL_RENDERER ); //Получаем имя рендерера
	const GLubyte* version = glGetString( GL_VERSION ); //Получаем номер версии
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported: " << version << std::endl;

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
}

void Application::MakeScene()
{
	// Устанавливаем камеру
	camera.viewMatrix = glm::lookAt( glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec3( 0.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	// Перспективное проецирование (чем дальше объект, тем он меньше выглядит)
	camera.projMatrix = glm::perspective( glm::radians( 45.0f ), 4.0f / 3.0f, 0.1f, 100.f );
}

void Application::Run()
{
	// Главный цикл
	while( !glfwWindowShouldClose( window ) ) //Пока окно не закрыто
	{
		glfwPollEvents(); //Проверяем события ввода, и вызываем соответствующую функцию обратного вызова

		Update(); //Обновляем сцену и положение виртуальной камеры

		if( hasGUI ) {
			UpdateGUI();
		}

		Draw(); //Рисуем один кадр

		if( hasGUI ) {
			DrawGUI();
		}

		glfwSwapBuffers( window ); //Переключаем передний и задний буферы
	}
}

void Application::HandleKey( int key, int scancode, int action, int mods )
{
	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_ESCAPE ) {
			glfwSetWindowShouldClose( window, GL_TRUE );
		}
	}
}

void Application::HandleMouseMove( double xpos, double ypos )
{
	int state = glfwGetMouseButton( window, GLFW_MOUSE_BUTTON_LEFT );
	if( state == GLFW_PRESS ) {
		double dx = xpos - oldXPos;
		double dy = ypos - oldYPos;

		phiAng -= dx * 0.005;
		thetaAng += dy * 0.005;
	}

	oldXPos = xpos;
	oldYPos = ypos;
}

void Application::HandleScroll( double xoffset, double yoffset )
{
	r += r * yoffset * 0.05;
}

void Application::Update()
{
	double dt = glfwGetTime() - oldTime;
	oldTime = glfwGetTime();

	//-----------------------------------------

	double speed = 1.0;

	if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS ) {
		phiAng -= speed * dt;
	}
	if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS ) {
		phiAng += speed * dt;
	}
	if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS ) {
		thetaAng += speed * dt;
	}
	if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS ) {
		thetaAng -= speed * dt;
	}
	if( glfwGetKey( window, GLFW_KEY_R ) == GLFW_PRESS ) {
		r += r * dt;
	}
	if( glfwGetKey( window, GLFW_KEY_F ) == GLFW_PRESS ) {
		r -= r * dt;
	}

	//-----------------------------------------

	thetaAng = glm::clamp( thetaAng, -glm::pi<double>() * 0.49, glm::pi<double>() * 0.49 );

	//Вычисляем положение виртуальной камеры в мировой системе координат по формуле сферических координат
	glm::vec3 pos = glm::vec3( glm::cos( phiAng ) * glm::cos( thetaAng ),
							   glm::sin( phiAng ) * glm::cos( thetaAng ),
							   glm::sin( thetaAng ) + 0.5f ) * ( float ) r;

	//Обновляем матрицу вида
	camera.viewMatrix = glm::lookAt( pos, glm::vec3( 0.0f, 0.0f, 0.5f ), glm::vec3( 0.0f, 0.0f, 1.0f ) );

	//-----------------------------------------

	int width, height;
	glfwGetFramebufferSize( window, &width, &height );

	//Обновляем матрицу проекции на случай, если размеры окна изменились
	camera.projMatrix = glm::perspective( glm::radians( 45.0f ), ( float ) width / height, 0.1f, 100.f );
}

void Application::Draw()
{
}

//====================================================

void Application::InitGUI()
{
	ImGui_ImplGlfwGL3_Init( window, false );
}

void Application::UpdateGUI()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void Application::DrawGUI()
{
	ImGui::Render();
}