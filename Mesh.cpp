#define _USE_MATH_DEFINES

#include "Mesh.hpp"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

glm::vec3 point( const float r, const float u, const float v )
{
	glm::vec3 point;
	point.x = ( r + cos( u / 2 ) * sin( v ) - sin( u / 2 ) * sin( 2 * v ) ) * cos( u );
	point.y = ( r + cos( u / 2 ) * sin( v ) - sin( u / 2 ) * sin( 2 * v ) ) * sin( u );
	point.z = sin( u / 2 ) * sin( v ) + cos( u / 2 ) * sin( 2 * v );
	return point;
}

glm::vec3 normal( const float u, const float v )
{
	return glm::normalize( glm::vec3( cos( u ) * sin( v ), sin( u ) * sin( v ), cos( v ) ) );
}

MeshPtr makeKleinBottle( const float radius, const size_t N )
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	//std::vector<glm::vec2> texcoords;

	for( size_t i = 0; i < N; i++ ) { // [-pi, pi]
		float v1 = -( float ) M_PI + 2.0f * ( float ) M_PI * i / N;
		float v2 = -( float ) M_PI + 2.0f * ( float ) M_PI * ( i + 1 ) / N;

		for( size_t j = 0; j < N; j++ ) { // [0, 2pi]
			float u1 = 2.0f * ( float ) M_PI * j / N;
			float u2 = 2.0f * ( float ) M_PI * ( j + 1 ) / N;

			glm::vec3 point1, point2, point3, point4;
			point1 = point( radius, u1, v1 );
			point2 = point( radius, u2, v1 );
			point3 = point( radius, u1, v2 );
			point4 = point( radius, u2, v2 );

			// Первый треугольник
			vertices.push_back( point1 );
			vertices.push_back( point2 );
			vertices.push_back( point3 );

			//texcoords.push_back( glm::vec2( ( float ) j / N, 1.0f - ( float ) i / M ) );
			//texcoords.push_back( glm::vec2( ( float ) ( j + 1 ) / N, 1.0f - ( float ) i / M ) );
			//texcoords.push_back( glm::vec2( ( float ) ( j + 1 ) / N, 1.0f - ( float ) ( i + 1 ) / M ) );

			//Второй треугольник

			vertices.push_back( point4 );
			vertices.push_back( point2 );
			vertices.push_back( point3 );

			// Todo: Ниже проблема в стыке. Пока не работаю с освещением такой вариант устраивает.
			// Потом нужно будет переделать.
			normals.push_back( glm::normalize( point1 ) ); //normal( u1, v1 ) );
			normals.push_back( glm::normalize( point2 ) ); //normal( u2, v1 ) );
			normals.push_back( glm::normalize( point3 ) ); //normal( u2, v2 ) );

			normals.push_back( glm::normalize( point4 ) ); //normal( u1, v1 ) );
			normals.push_back( glm::normalize( point2 ) ); //normal( u2, v2 ) );
			normals.push_back( glm::normalize( point3 ) ); //normal( u1, v2 ) );


			//texcoords.push_back( glm::vec2( ( float ) j / N, 1.0f - ( float ) i / M ) );
			//texcoords.push_back( glm::vec2( ( float ) ( j + 1 ) / N, 1.0f - ( float ) ( i + 1 ) / M ) );
			//texcoords.push_back( glm::vec2( ( float ) j / N, 1.0f - ( float ) ( i + 1 ) / M ) );
		}
	}

	//----------------------------------------

	DataBufferPtr buf0 = std::make_shared<DataBuffer>( GL_ARRAY_BUFFER );
	buf0->setData( vertices.size() * sizeof( float ) * 3, vertices.data() );

	DataBufferPtr buf1 = std::make_shared<DataBuffer>( GL_ARRAY_BUFFER );
	buf1->setData( normals.size() * sizeof( float ) * 3, normals.data() );

	/*DataBufferPtr buf2 = std::make_shared<DataBuffer>( GL_ARRAY_BUFFER );
	buf2->setData( texcoords.size() * sizeof( float ) * 2, texcoords.data() );*/

	MeshPtr mesh = std::make_shared<Mesh>();
	mesh->setAttribute( 0, 3, GL_FLOAT, GL_FALSE, 0, 0, buf0 );
	mesh->setAttribute( 1, 3, GL_FLOAT, GL_FALSE, 0, 0, buf1 );
	//mesh->setAttribute( 2, 2, GL_FLOAT, GL_FALSE, 0, 0, buf2 );
	mesh->setPrimitiveType( GL_TRIANGLES );
	mesh->setVertexCount( vertices.size() );

	return mesh;
}