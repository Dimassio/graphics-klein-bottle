#include "KleinBottleApplication.h"

void CKleinBottleApplication::MakeScene()
{
	Application::MakeScene();

	//Создаем меш
	createMesh();

	//Создаем шейдерную программу        
	shader = std::make_shared<ShaderProgram>();
	shader->createProgram( "296ZvonarevData/shader.vert", "296ZvonarevData/shader.frag" );
}

void CKleinBottleApplication::Update()
{
	Application::Update();
	// тут можно вращать фигуру нашу
}

void CKleinBottleApplication::Draw()
{
	Application::Draw();

	int width, height;
	glfwGetFramebufferSize( window, &width, &height );

	glViewport( 0, 0, width, height );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Устанавливаем шейдер
	shader->use();

	//Устанавливаем общие юниформ-переменные
	shader->setMat4Uniform( "viewMatrix", camera.viewMatrix );
	shader->setMat4Uniform( "projectionMatrix", camera.projMatrix );

	//Рисуем первый меш
	drawMesh();
}

void CKleinBottleApplication::HandleKey( int key, int scancode, int action, int mod )
{
	if( key == GLFW_KEY_KP_ADD ) {
		detailed = std::min( maxDetailed, detailed + 10 );
		createMesh();
		drawMesh();
	}
	if( key == GLFW_KEY_KP_SUBTRACT ) {
		detailed = std::max( minDetailed, detailed - 10 );
		createMesh();
		drawMesh();
	}
	Application::HandleKey( key, scancode, action, mod );
}

void CKleinBottleApplication::createMesh()
{
	bottle = makeKleinBottle( 3.0f, detailed );
	bottle->setModelMatrix( glm::translate( glm::mat4( 1.0f ), glm::vec3( -5.0f, 0.0f, -1.5f ) ) );
}

void CKleinBottleApplication::drawMesh()
{
	shader->setMat4Uniform( "modelMatrix", bottle->modelMatrix() );
	bottle->draw();
}