#include "KleinBottleApplication.h"
#include "Texture.hpp"
#include "Mesh.hpp"

void CKleinBottleApplication::MakeScene()
{
	Application::MakeScene();

	//Создаем меш
	createMesh();

	marker = makeSphere( 0.1f );

	//Создаем шейдерную программу        
	shader = std::make_shared<ShaderProgram>();
	shader->createProgram( "296ZvonarevData/texture.vert", "296ZvonarevData/texture.frag" );

	markerShader = std::make_shared<ShaderProgram>();
	markerShader->createProgram( "296ZvonarevData/marker.vert", "296ZvonarevData/marker.frag" );

	//=========================================================
	//Положение источника света
	lr = 3.0;
	phi = 0.0;
	theta = glm::pi<float>() * 0.25f;

	// Характеристики света
	light.position = glm::vec3( glm::cos( phi ) * glm::cos( theta ), glm::sin( phi ) * glm::cos( theta ), glm::sin( theta ) ) * ( float ) lr;
	light.ambient = glm::vec3( 0.2, 0.2, 0.2 );
	light.diffuse = glm::vec3( 0.8, 0.8, 0.8 );
	light.specular = glm::vec3( 1.0, 1.0, 1.0 );

	//=========================================================
	//Загрузка и создание текстур
	worldTexture = loadTexture( "296ZvonarevData/earth_global.jpg" );

	//=========================================================
	//Инициализация сэмплера, объекта, который хранит параметры чтения из текстуры
	glGenSamplers( 1, &sampler );
	glSamplerParameteri( sampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glSamplerParameteri( sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glSamplerParameteri( sampler, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glSamplerParameteri( sampler, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

void CKleinBottleApplication::Update()
{
	Application::Update();
	// тут можно вращать фигуру нашу
	// todo: 2й источник света перемещается на delta радиан
}

void CKleinBottleApplication::Draw()
{
	Application::Draw();

	int width, height;
	glfwGetFramebufferSize( window, &width, &height );

	// Параметры экранной системы координат
	glViewport( 0, 0, width, height );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//Устанавливаем шейдер
	shader->use();

	//Устанавливаем общие юниформ-переменные
	shader->setMat4Uniform( "viewMatrix", camera.viewMatrix );
	shader->setMat4Uniform( "projectionMatrix", camera.projMatrix );

	light.position = glm::vec3( glm::cos( phi ) * glm::cos( theta ), glm::sin( phi ) * glm::cos( theta ), glm::sin( theta ) ) * ( float ) lr;
	glm::vec3 lightPosCamSpace = glm::vec3( camera.viewMatrix * glm::vec4( light.position, 1.0 ) );


	shader->setVec3Uniform( "light.pos", lightPosCamSpace ); //копируем положение уже в системе виртуальной камеры
	shader->setVec3Uniform( "light.La", light.ambient );
	shader->setVec3Uniform( "light.Ld", light.diffuse );
	shader->setVec3Uniform( "light.Ls", light.specular );

	glActiveTexture( GL_TEXTURE0 );  //текстурный юнит 0        
	worldTexture->bind();

	glBindSampler( 0, sampler );

	// todo: what?
	shader->setIntUniform( "diffuseTex", 0 );
	shader->setMat4Uniform( "modelMatrix", bottle->modelMatrix() );
	shader->setMat3Uniform( "normalToCameraMatrix", glm::transpose( glm::inverse( glm::mat3( camera.viewMatrix * bottle->modelMatrix() ) ) ) );

	//Рисуем бутылку
	drawMesh();

	//Рисуем маркеры для всех источников света		
	{
		markerShader->use();
		markerShader->setMat4Uniform( "mvpMatrix", camera.projMatrix * camera.viewMatrix * glm::translate( glm::mat4( 1.0f ), light.position ) );
		markerShader->setVec4Uniform( "color", glm::vec4( light.diffuse, 1.0f ) );
		marker->draw();
	}

	//Отсоединяем сэмплер и шейдерную программу
	glBindSampler( 0, 0 );
	glUseProgram( 0 );

}

void CKleinBottleApplication::HandleKey( int key, int scancode, int action, int mod )
{
	if( key == GLFW_KEY_KP_ADD ) {
		detailed = std::min( maxDetailed, detailed + 10 );
		createMesh();
	}
	if( key == GLFW_KEY_KP_SUBTRACT ) {
		detailed = std::max( minDetailed, detailed - 10 );
		createMesh();
	}
	Application::HandleKey( key, scancode, action, mod );
}

void CKleinBottleApplication::createMesh()
{
	bottle = makeKleinBottle( 3.0f, detailed );
	// Матрица переноса на (-5; 0; -1.5)
	bottle->setModelMatrix( glm::translate( glm::mat4( 1.0f ), glm::vec3( -5.0f, 0.0f, -1.5f ) ) );
}

void CKleinBottleApplication::drawMesh()
{
	shader->setMat4Uniform( "modelMatrix", bottle->modelMatrix() );
	bottle->draw();
}