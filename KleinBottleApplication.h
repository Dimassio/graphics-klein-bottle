#pragma once
#include "Mesh.hpp"
#include "ShaderProgram.hpp"
#include "Application.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

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

	void createMesh();
	void drawMesh();
};