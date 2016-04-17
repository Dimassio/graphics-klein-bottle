#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

struct LightInfo {
	glm::vec3 position; //Будем здесь хранить координаты в мировой системе координат, а при копировании в юниформ-переменную конвертировать в систему виртуальной камеры
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

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
	ShaderProgramPtr shader;
	int detailed;
	int minDetailed;
	int maxDetailed;

	TexturePtr worldTexture;
	GLuint sampler;
	ShaderProgramPtr markerShader;
	//Переменные для управления положением одного источника света
	float lr;
	float phi;
	float theta;
	LightInfo light;
	MeshPtr marker; //Меш - маркер для источника света

	void createMesh();
	void drawMesh();
};