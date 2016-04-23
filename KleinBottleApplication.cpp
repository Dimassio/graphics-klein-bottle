#include "KleinBottleApplication.h"
#include "Texture.hpp"
#include "Mesh.hpp"

void CKleinBottleApplication::MakeScene()
{
	Application::MakeScene();

	// Создаем меш
	createMesh();
	// Создаем источники света
	marker = makeSphere( 0.1f );

	// Шейдерная программа для фигуры    
	shader = std::make_shared<ShaderProgram>();
	shader->createProgram( "296ZvonarevData/manyLights.vert", "296ZvonarevData/manyLights.frag" );
	
	// Шейдерная программа для источника света
	markerShader = std::make_shared<ShaderProgram>();
	markerShader->createProgram( "296ZvonarevData/marker.vert", "296ZvonarevData/marker.frag" );
	
	//=========================================================
	//Положение источника света
	// todo: установить положениях ВСЕХ источников света
	lightR = 3.0;
	phi = 0;
	theta = glm::pi<float>() * 0.5f;

	// Характеристики света
	// Фиксированный:
	lights[0].position = glm::vec3( glm::cos( 6.0f ) * glm::cos( glm::pi<float>() * 0.5f ), 
									glm::sin( 0.0f ) * glm::cos( glm::pi<float>() * 0.5f ), 
									glm::sin( glm::pi<float>() * 0.5f ) ) * 6.0f;
	lights[0].ambient = glm::vec3( 0.2, 0.2, 0.2 );
	lights[0].diffuse = glm::vec3( 0.8, 0.8, 0.8 );
	lights[0].specular = glm::vec3( 1.0, 1.0, 1.0 );

	// Летающий:
	lights[1].position = glm::vec3( glm::cos( phi ) * glm::cos( theta ),
									glm::sin( phi ) * glm::cos(theta ),
									glm::sin( theta ) ) * lightR;
	lights[1].ambient = glm::vec3( 0.2, 0.2, 0.2 );
	lights[1].diffuse = glm::vec3( 0.8, 0.8, 0.8 );
	lights[1].specular = glm::vec3( 1.0, 1.0, 1.0 );

	// На камере:
	lights[2].position = glm::vec3( glm::cos( phiAng ) * glm::cos( thetaAng ),
									glm::sin( phiAng ) * glm::cos( thetaAng ),
									glm::sin( thetaAng ) ) * (float) r;
	lights[2].ambient = glm::vec3( 0.2, 0.2, 0.2 );
	lights[2].diffuse = glm::vec3( 0.8, 0.0, 0.0 );
	lights[2].specular = glm::vec3( 1.0, 1.0, 1.0 );

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
	// todo: update movable coords 
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

	// Обновляем координаты движущегося источника
	lights[1].position = glm::vec3( glm::cos( phi ) * glm::cos( theta ),
									glm::sin( phi ) * glm::cos( theta ),
									glm::sin( theta ) ) * ( float ) lightR;
	// Обновляем координаты источника на камере
	lights[2].position = glm::vec3( glm::cos( phiAng ) * glm::cos( thetaAng ),
									glm::sin( phiAng ) * glm::cos( thetaAng ),
									glm::sin( thetaAng ) ) * ( float ) r;
	// Положение уже в системе виртуальной камеры
	for( size_t i = 0; i < NumberOfLights; ++i ) {
		std::string currLight = "light[" + std::to_string( i ) + "]";

		glm::vec3 lightPosCamSpace = glm::vec3( camera.viewMatrix * glm::vec4( lights[i].position, 1.0 ) );

		//копируем положение уже в системе виртуальной камеры
		shader->setVec3Uniform( currLight + ".pos", lightPosCamSpace );
		shader->setVec3Uniform( currLight + ".La", lights[i].ambient );
		shader->setVec3Uniform( currLight + ".Ld", lights[i].diffuse );
		shader->setVec3Uniform( currLight + ".Ls", lights[i].specular );
	}

	glActiveTexture( GL_TEXTURE0 );  // текстурный юнит 0        
	worldTexture->bind();

	glBindSampler( 0, sampler );

	//Загружаем на видеокарту матрицы модели мешей и запускаем отрисовку
	shader->setIntUniform( "diffuseTex", 0 );
	shader->setMat4Uniform( "modelMatrix", bottle->modelMatrix() );
	shader->setMat3Uniform( "normalToCameraMatrix", glm::transpose( glm::inverse( glm::mat3( camera.viewMatrix * bottle->modelMatrix() ) ) ) );

	//Рисуем бутылку
	drawMesh();

	//Рисуем маркеры для всех источников света, кроме камеры		
	{
		markerShader->use();

		for( size_t i = 0; i < NumberOfLights; ++i ) {
			markerShader->setMat4Uniform( "mvpMatrix", camera.projMatrix * camera.viewMatrix * glm::translate( glm::mat4( 1.0f ), lights[i].position ) );
			markerShader->setVec4Uniform( "color", glm::vec4( lights[i].diffuse, 1.0f ) );
			marker->draw();
		}
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