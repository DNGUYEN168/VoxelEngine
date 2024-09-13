#include "ReadShadertoGL.hpp"
#include "ChunkIMP.hpp"
#include "World.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

// glm inserts 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// glew / glfw 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <random>
#include <iostream>
#include <array>
#include <vector>
#include <sstream>
#include <string>


//camera info
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse info 
bool firstmouse = true;
float lastX = 400, lastY = 300; // half 800 for x | half 600 for y 
float fov = 1.0f;

//Euler angles 
float yaw = -90;
float pitch = 1;


bool draw_lines = true;


// color height 
float y = 1.0f;
const char* vrtshd = "glShaderSourceCode\\colorcube.vert";
const char* frgshd = "glShaderSourceCode\\colorcube.frag";

// TODO: CHANGE THIS SECTION 

void render(GLuint VAO, int indexCount) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	GLenum errorCode = glGetError();
	if (errorCode != GL_NO_ERROR) { std::cout << "whoopsies: " << errorCode << std::endl; }
	glBindVertexArray(0);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
// call back function to take mouse input (mouse x,y)
{
	if (firstmouse) // is there a mouse input yet?
	{
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	// MATHY PART 
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	const float cameraSpeed = 10 * deltaTime;; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		cameraPos.y += .05;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos.y -= .05;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		draw_lines = false;

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		draw_lines = true;

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void startIMGUI(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


int Create_world(void)
{

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "YATTTAAAAAAA", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	// CREATE PROGRAM SHADER
	Shader programShader(vrtshd, frgshd);

	World* testworld = new World(25,25);


	

	testworld->CreateWorld();

	
	//startIMGUI(window);
	while (!glfwWindowShouldClose(window))
	{
		// INPUT COMMANDS 
		processInput(window);

		// RENDERING COMMANDS (DRAWING TO SCREEN)
		// R G B values and alpha
		glClearColor(1.000f, 0.941f, 0.961f, .50f); // decides the color 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::stringstream ss;
		ss << (int)cameraPos.x << " " << (int)cameraPos.y << " " << (int)cameraPos.z;
		std::string strPos = ss.str();
		const char* c = strPos.c_str();

		/*test_chunk.CreateChunkMesh();
		test_chunk2.CreateChunkMesh();*/

		/*ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();*/



		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame; // calcs a consistent framerate for our players 
		lastFrame = currentFrame;

		programShader.use();

		glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);


		model = glm::translate(model, glm::vec3(0.0f, 0.0, 0.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 300.0f);


		programShader.setMat4("model", model);
		programShader.setMat4("view", view);
		programShader.setMat4("projection", projection);

		if (draw_lines) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
		else{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
		

		
		testworld->drawMesh();

		/*ImGui::Begin("Cube x y z");
		ImGui::End();*/

		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSetWindowTitle(window, c);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	/*ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();*/
	glfwTerminate();
	return 0;
}


