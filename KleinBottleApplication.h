#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

struct LightInfo {
	// Будем здесь хранить координаты в мировой системе координат, 
	// а при копировании в юниформ-переменную конвертировать в систему виртуальной камеры
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

const size_t NumberOfLights = 3;

class CKleinBottleApplication: public Application {
public:
	CKleinBottleApplication():
		Application(), detailed( 30 ), minDetailed( 10 ), maxDetailed( 200 )
	{
	}

	void MakeScene() override;
	void Update() override;
	void Draw() override;
	void HandleKey( int key, int scancode, int action, int mod ) override;

private:
	MeshPtr bottle;
	MeshPtr marker;

	// Для бутылки Клейна
	ShaderProgramPtr shader;
	// Для перемещающегося и фиксированного источников света
	ShaderProgramPtr markerShader;

	int detailed;
	int minDetailed;
	int maxDetailed;

	TexturePtr worldTexture;
	GLuint sampler;

	//Переменные для управления движущимся источника света
	float lightR;
	float phi;
	float theta;

	// Источники света
	LightInfo lights[NumberOfLights];

	void createMesh();
	void drawMesh();
	// Перемещаем источник света на delta радиан
	void moveLightSource( const float delta );
};