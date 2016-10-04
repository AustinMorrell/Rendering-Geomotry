#pragma once
#include "Gizmos.h"
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "FlyCamera.h"
#define PI 3.14159265358979323846

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
};

class App
{
public:
	virtual bool start() = 0;
	virtual bool update() = 0;
	virtual void draw() = 0;
	virtual void destroy() = 0;
};

class RenderingGeometryApplication : public App {
public:
	RenderingGeometryApplication();
	bool start() override;
	bool update() override;
	void draw() override;
	void destroy() override;
	std::string ReadFromFile(std::string text);

	void createPlane();
	void drawPlane();
	void createCube();
	void drawCube();
	Vertex* generateHalfSphereVertices(unsigned int np, const int &rad);
	Vertex* generateSphereVertices(const unsigned int &sides, const unsigned int &mirid, Vertex* &halfSphere);
	unsigned int* generateSphereIndicies(const unsigned int &vertices, const unsigned int &mirid);
	std::vector<unsigned int> indicesHolder;
	void createSphere(const int radius, const unsigned int verts, const unsigned int halfSpheres);
	void drawSphere();
	double DeltaTime;
private:
	glm::mat4 m_projectionViewMatrix;
	GLFWwindow* window;
	FlyCamera Cam;
	double pastTime = 0;
	double newTime;
	unsigned int m_programID;
	unsigned int p_vao;
	unsigned int p_vbo;
	unsigned int p_ibo;
	unsigned int c_vao;
	unsigned int c_vbo;
	unsigned int c_ibo;
	unsigned int s_vao;
	unsigned int s_vbo;
	unsigned int s_ibo;
	int p_indicesCounter;
	int c_indicesCounter;
	int s_indicesCounter;
};