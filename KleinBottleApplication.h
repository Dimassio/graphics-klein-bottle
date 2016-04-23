#pragma once
#include "Mesh.hpp"
#include "Texture.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

struct LightInfo {
	// ����� ����� ������� ���������� � ������� ������� ���������, 
	// � ��� ����������� � �������-���������� �������������� � ������� ����������� ������
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

	// ��� ������� ������
	ShaderProgramPtr shader;
	// ��� ��������������� � �������������� ���������� �����
	ShaderProgramPtr markerShader;

	int detailed;
	int minDetailed;
	int maxDetailed;

	TexturePtr worldTexture;
	GLuint sampler;

	//���������� ��� ���������� ���������� ��������� �����
	float lightR;
	float phi;
	float theta;

	// ��������� �����
	LightInfo lights[NumberOfLights];

	void createMesh();
	void drawMesh();
	// ���������� �������� ����� �� delta ������
	void moveLightSource( const float delta );
};