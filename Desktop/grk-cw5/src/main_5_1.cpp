#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLuint programColor;
GLuint programTexture;
GLuint programTexture1;
GLuint planet0;
GLuint planet1;
GLuint planet2;
GLuint planet3;
GLuint sun;
GLuint texture_ship;
GLuint moon0;
GLuint moon1;
GLuint moon2;
GLuint moon3;
GLuint texture;
GLuint texture_back;
Core::Shader_Loader shaderLoader;


obj::Model shipModel;
obj::Model sphereModel;
obj::Model sunModel;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-8, 0, 0);
glm::vec3 cameraDir;

glm::vec3 SunVector;
glm::vec3 Planet0Vector;
glm::vec3 Planet1Vector;
glm::vec3 Planet2Vector;
glm::vec3 Planet3Vector;
glm::vec3 EarthVector;
glm::vec3 MoonVector;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	float turboSpeed = 2.0f;
	switch (key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0, 1, 0)) * moveSpeed; break;
	case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0, 1, 0)) * moveSpeed; break;
	case 'f': cameraPos -= glm::cross(cameraDir, glm::vec3(1, 0, 1)) * moveSpeed; break;
	case 'v': cameraPos -= glm::cross(cameraDir, glm::vec3(-1, 0, -1)) * moveSpeed; break;
	case 't': cameraPos += cameraDir * turboSpeed; break;
	}
}


glm::mat4 createCameraMatrix()
{
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0, 1, 0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint text)
{
	GLuint program = programTexture;

	glUseProgram(program);

	Core::SetActiveTexture(text, "sampler", program, 0);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}
void drawObjectBack(obj::Model * model, glm::mat4 modelMatrix, GLuint text)
{
	GLuint program = programTexture1;

	glUseProgram(program);

	Core::SetActiveTexture(text, "sampler", program, 0);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void renderScene()
{
	float time = glutGet(GLUT_ELAPSED_TIME) / 8000.0f;
	SunVector = glm::vec3(0, 0, 0);
	Planet0Vector = glm::vec3(cos(time * 8) * 5, 0, sin(time * 8) * 5);
	Planet1Vector = glm::vec3(-cos(time * 8) * 17, 0, sin(time * 8) * 17);
	Planet2Vector = glm::vec3(cos(time * 8) * 14, 0, sin(time * 8) * 14);
	Planet3Vector = glm::vec3(-cos(time * 8) * 11, 0, sin(time * 8) * 11);
	MoonVector = glm::vec3(cos(time * 8) * 1, 0, sin(time * 8) * 1);
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);

	drawObjectTexture(&shipModel, glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0, -0.25f, 0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.25f)), texture_ship);
	drawObjectTexture(&sphereModel, glm::translate(SunVector) * glm::scale(glm::vec3(3.5f)) * glm::rotate(time * 10, glm::vec3(0, 1, 0))* glm::scale(glm::vec3(1.3f)), sun);
	drawObjectTexture(&sphereModel, glm::translate(Planet0Vector) * glm::translate(SunVector) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.3f)), planet1);
	drawObjectTexture(&sphereModel, glm::translate(Planet1Vector) * glm::translate(SunVector) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(2.0f)), planet3);
	drawObjectTexture(&sphereModel, glm::translate(Planet2Vector) * glm::translate(SunVector) * glm::rotate(time * 40, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.5f)), planet0);
	drawObjectTexture(&sphereModel, glm::translate(Planet3Vector) * glm::translate(SunVector) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f)), planet2);
	drawObjectTexture(&sphereModel, glm::translate(MoonVector) * glm::translate(Planet0Vector) * glm::translate(SunVector) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f)) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.35f)), moon1);
	drawObjectTexture(&sphereModel, glm::translate(MoonVector) * glm::translate(Planet1Vector) * glm::translate(SunVector) * glm::rotate(time * 30, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f)) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.35f)), moon0);
	drawObjectTexture(&sphereModel, glm::translate(MoonVector) * glm::translate(Planet2Vector) * glm::translate(SunVector) * glm::rotate(time * 40, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f)) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.35f)), moon2);
	drawObjectTexture(&sphereModel, glm::translate(MoonVector) * glm::translate(Planet3Vector) * glm::translate(SunVector) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(1.0f)) * glm::rotate(time * 50, glm::vec3(0, 1, 0)) * glm::scale(glm::vec3(0.35f)), moon3);
	drawObjectBack(&sphereModel, glm::scale(glm::vec3(70.0f)), texture_back);

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programTexture1 = shaderLoader.CreateProgram("shaders/shader_tex1.vert", "shaders/shader_tex1.frag");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel = obj::loadModelFromFile("models/spaceship.obj");
	texture = Core::LoadTexture("textures/grid.png");
	texture_ship = Core::LoadTexture("textures/ship.png");
	texture_back = Core::LoadTexture("textures/stars.png");
	planet0 = Core::LoadTexture("textures/planet1.png");
	planet1 = Core::LoadTexture("textures/earth.png");
	planet2 = Core::LoadTexture("textures/planet2.png");
	planet3 = Core::LoadTexture("textures/planet3.png");
	sun = Core::LoadTexture("textures/sun.png");
	moon0 = Core::LoadTexture("textures/moon.png");
	moon1 = Core::LoadTexture("textures/moon.png");
	moon2 = Core::LoadTexture("textures/moon.png");
	moon3 = Core::LoadTexture("textures/moon.png");
}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
	shaderLoader.DeleteProgram(programTexture1);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(600, 600);
	glutCreateWindow("OpenGL Pierwszy Program");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
