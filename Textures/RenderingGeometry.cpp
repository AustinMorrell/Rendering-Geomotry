#include <stb_image.h>
#include "app.h"

RenderingGeometryApplication::RenderingGeometryApplication()
{
	glfwInit();

	window = glfwCreateWindow(1280, 720, "Rendering Geometry", nullptr, nullptr);

	if (window == nullptr)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	view = glm::lookAt(glm::vec3(15, 15, 15), glm::vec3(0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);
}

bool RenderingGeometryApplication::start()
{
	/*createPlane();*/
	createCube();
	/*createSphere(2, 30, 30);*/

	const char* vsSource;
	std::string vs = ReadFromFile("vsInfo.txt");
	vsSource = vs.c_str();

	const char* fsSource;
	std::string fs = ReadFromFile("fsInfo.txt");
	fsSource = fs.c_str();

	int success = GL_FALSE;
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);
	glLinkProgram(m_programID);

	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_programID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);
		delete[] infoLog;
	}
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	return true;
}

bool RenderingGeometryApplication::update()
{
	while (glfwWindowShouldClose(window) == false && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return true;
	}
	return false;
}

void RenderingGeometryApplication::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);

	unsigned int projectionViewUniform = glGetUniformLocation(m_programID, "projectionViewWorldMatrix");
	m_projectionViewMatrix = projection * view;
	glUniformMatrix4fv(projectionViewUniform, 1, false, glm::value_ptr(m_projectionViewMatrix));

	drawPlane();
	/*drawCube();*/
	/*drawSphere();*/

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void RenderingGeometryApplication::destroy()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

std::string RenderingGeometryApplication::ReadFromFile(std::string text)
{
	std::string data;
	std::string container;
	std::ifstream file(text);

	if (file.is_open())
	{
		while (std::getline(file, data))
		{
			container += data + "\n";
		}
		file.close();
	}
	return container;
}

void RenderingGeometryApplication::createPlane()
{
	Vertex vertices[4];
	unsigned int indices[4] = { 0,2,1,3 };

	p_indicesCounter = 4;

	vertices[0].position = glm::vec4(-2, 0, -2, 1);
	vertices[1].position = glm::vec4(2, 0, -2, 1);
	vertices[2].position = glm::vec4(-2, 0, 2, 1);
	vertices[3].position = glm::vec4(2, 0, 2, 1);

	vertices[0].color = glm::vec4(1, 0, 0, 1);
	vertices[1].color = glm::vec4(0, 1, 0, 1);
	vertices[2].color = glm::vec4(0, 0, 1, 1);
	vertices[3].color = glm::vec4(1, 1, 1, 1);

	glGenBuffers(1, &p_vbo);
	glGenBuffers(1, &p_ibo);
	glGenVertexArrays(1, &p_vao);
	glBindVertexArray(p_vao);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderingGeometryApplication::drawPlane()
{
	glBindVertexArray(p_vao);
	glDrawElements(GL_TRIANGLE_STRIP, p_indicesCounter, GL_UNSIGNED_INT, 0);
}

void RenderingGeometryApplication::createCube()
{
	Vertex vertices[8];
	unsigned int indices[17] = { 0,1,2,3,6,7,4,5,0,1,5,3,7,6,4,2,0 };

	c_indicesCounter = 17;

	vertices[0].position = glm::vec4(-2, 4, -2, 1);
	vertices[1].position = glm::vec4(2, 4, -2, 1);
	vertices[2].position = glm::vec4(-2, 4, 2, 1);
	vertices[3].position = glm::vec4(2, 4, 2, 1);
	vertices[4].position = glm::vec4(-2, 6, -2, 1);
	vertices[5].position = glm::vec4(2, 6, -2, 1);
	vertices[6].position = glm::vec4(-2, 6, 2, 1);
	vertices[7].position = glm::vec4(2, 6, 2, 1);

	vertices[0].color = glm::vec4(1, 0, 0, 1);
	vertices[1].color = glm::vec4(0, 1, 0, 1);
	vertices[2].color = glm::vec4(0, 0, 1, 1);
	vertices[3].color = glm::vec4(1, 1, 1, 1);
	vertices[4].color = glm::vec4(1, 0, 0, 1);
	vertices[5].color = glm::vec4(0, 1, 0, 1);
	vertices[6].color = glm::vec4(0, 0, 1, 1);
	vertices[7].color = glm::vec4(1, 1, 1, 1);

	glGenBuffers(1, &c_vbo);
	glGenBuffers(1, &c_ibo);
	glGenVertexArrays(1, &c_vao);
	glBindVertexArray(c_vao);

	glBindBuffer(GL_ARRAY_BUFFER, c_vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, c_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 17 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void RenderingGeometryApplication::drawCube()
{
	glBindVertexArray(c_vao);
	glDrawElements(GL_TRIANGLE_STRIP, c_indicesCounter, GL_UNSIGNED_INT, 0);
}

Vertex* RenderingGeometryApplication::generateHalfSphereVertices(unsigned int np, const int &rad)
{

}

Vertex* RenderingGeometryApplication::generateSphereVertices(const unsigned int &sides, const unsigned int &mirid, Vertex* &halfSphere)
{

}

unsigned int* RenderingGeometryApplication::generateSphereIndicies(const unsigned int &vertices, const unsigned int &mirid)
{

}

void RenderingGeometryApplication::createSphere(const int radius, const unsigned int verts, const unsigned int halfSpheres)
{

}

void RenderingGeometryApplication::drawSphere()
{
	glBindVertexArray(s_vao);
	glDrawElements(GL_TRIANGLE_STRIP, s_indicesCounter, GL_UNSIGNED_INT, 0);
}