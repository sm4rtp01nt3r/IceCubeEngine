#include <iostream>

#include "Util.h"
#include "Window.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Triangle.h"
#include "Quad.h"
#include "Cube.h"
#include "Camera.h"
#include "TextRenderer.h"
#include "Light.h"
#include "LightType.h"
#include "DirectionalLight.h"
#include "PointLight.h"

GLboolean firstMouse = true;
GLdouble lastX, lastY;
ICE::Camera camera;
static void mouseCallback(GLdouble posX, GLdouble posY);

int main() {
	GLuint width = 800;
	GLuint height = 600;

	ICE::Window window("test", width, height);
	window.toggleFPSDisplay();
	camera = ICE::Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), 4.0f, 0.1f, 45.0f, 0.1f, 100.0f, width, height);
	window.setCursorPosCallback(mouseCallback);

	ICE::ResourceManager::loadResources("resources.txt");

	ICE::TextRenderer::init(width, height);
	ICE::Triangle::init();
	ICE::Quad::init();
	ICE::Cube::init();

	//ICE::Light light(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), ICE::LightType::DIRECTIONAL);
	ICE::DirectionalLight light(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(1.0f));
	ICE::PointLight pointLight1(glm::vec3(-1.0f, 0.0f, 3.0f), glm::vec3(1.0f), 0.09f, 0.032f);
	std::vector<ICE::Light> lights;
	lights.push_back(light);
	lights.push_back(pointLight1);

	glm::vec3 a = glm::vec3(1.0f, 0.0f, 0.0f) * ICE::ResourceManager::materials["emerald"].ambient;

	ICE::Shader shader(ICE::Util::readFileToString("shaders/vert.glsl"), ICE::Util::readFileToString("shaders/frag.glsl"));
	ICE::Triangle t(shader, &camera, ICE::ResourceManager::materials["emerald"], glm::vec3(0.0f));
	ICE::Quad q(shader, &camera, ICE::ResourceManager::materials["emerald"], glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f), -55.0f);
	ICE::Quad q2(shader, &camera, ICE::ResourceManager::textures["container"], glm::vec3(0.0f, 0.0f, 0.0f));
	//ICE::Cube c(shader, &camera, ICE::ResourceManager::textures["container"], glm::vec3(0.0f));
	ICE::Cube c(shader, &camera, ICE::ResourceManager::materials["emerald"], glm::vec3(0.0f));
	ICE::Cube lightCube(shader, &camera, ICE::ResourceManager::textures["cube"], glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(0.5f));

	while (!window.shouldClose()) {
		GLfloat deltaTime = window.getDeltaTime();
		window.pollEvents();

		if (window.keys[GLFW_KEY_W]) {
			camera.handleKeyboardInput(ICE::Movement::FORWARD, deltaTime);
		}
		if (window.keys[GLFW_KEY_S]) {
			camera.handleKeyboardInput(ICE::Movement::BACKWARD, deltaTime);
		}
		if (window.keys[GLFW_KEY_A]) {
			camera.handleKeyboardInput(ICE::Movement::LEFT, deltaTime);
		}
		if (window.keys[GLFW_KEY_D]) {
			camera.handleKeyboardInput(ICE::Movement::RIGHT, deltaTime);
		}
		if (window.keys[GLFW_KEY_SPACE]) {
			camera.handleKeyboardInput(ICE::Movement::UPWARD, deltaTime);
		}
		if (window.keys[GLFW_KEY_LEFT_SHIFT]) {
			camera.handleKeyboardInput(ICE::Movement::DOWNWARD, deltaTime);
		}
		if (window.keys[GLFW_KEY_F1]) {
			window.keys[GLFW_KEY_F1] = false;
			window.toggleFPSDisplay();
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//t.draw();
		//q.draw();
		//q2.draw();

		//c.rotateBy(glm::vec3(0.0f, 1.0f, 1.0f), 1.0f);
		c.draw(lights);
		lightCube.draw(lights);

		window.update();
		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}

void mouseCallback(GLdouble positionX, GLdouble positionY) {
	if (firstMouse) {
		lastX = positionX;
		lastY = positionY;
		firstMouse = false;
	}

	GLfloat offsetX = positionX - lastX;
	GLfloat offsetY = lastY - positionY;
	lastX = positionX;
	lastY = positionY;
	camera.handleMouseInput(offsetX, offsetY);
}
