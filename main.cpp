#include"libs.h"
#include"Shader.h"
#include"texture.h"

//init frame rendering
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//int mouse
bool firstMouse = true;
float lastX;
float lastY;
float yaw = -90.f;
float pitch = 0.f;
//init camera
glm::vec3 camPosition(0.f, 0.f, 3.f);
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
glm::vec3 worldUp(0.f, 1.f, 0.f);
float fov = 45.f;

glm::vec3 cubePositions[] = {
  glm::vec3(0.0f,  0.0f,  0.0f),
  glm::vec3(2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3(2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3(1.3f, -2.0f, -2.5f),
  glm::vec3(1.5f,  2.0f, -2.5f),
  glm::vec3(1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

Vertex vertices[] = {
	glm::vec3(-0.5f , 0.5f , 0.5f) , glm::vec3(1.0f , 0.f , 0.f) , glm::vec2(0.0f , 1.f),//左上0
	glm::vec3(-0.5f , -0.5f , 0.5f) , glm::vec3(0.f , 1.f , 0.f) , glm::vec2(0.0f , 0.f),//左下1
	glm::vec3(0.5f , -0.5f , 0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(1.0f , 0.f),//右下2
	glm::vec3(0.5f , 0.5f , 0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(1.0f , 1.0f),//右上3

	glm::vec3(-0.5f , 0.5f , -0.5f) , glm::vec3(1.0f , 0.f , 0.f) , glm::vec2(0.0f , 1.f),//左上4
	glm::vec3(-0.5f , -0.5f , -0.5f) , glm::vec3(0.f , 1.f , 0.f) , glm::vec2(0.0f , 0.f),//左下5
	glm::vec3(0.5f , -0.5f , -0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(1.0f , 0.f),//右下6
	glm::vec3(0.5f , 0.5f , -0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(1.0f , 1.0f),//右上7

	glm::vec3(-0.5f , 0.5f , 0.5f) , glm::vec3(1.0f , 0.f , 0.f) , glm::vec2(1.0f , 1.f),//左上8
	glm::vec3(-0.5f , -0.5f , 0.5f) , glm::vec3(0.f , 1.f , 0.f) , glm::vec2(1.0f , 0.f),//左下9

	glm::vec3(-0.5f , -0.5f , 0.5f) , glm::vec3(0.f , 1.f , 0.f) , glm::vec2(0.0f , 1.f),//左下10
	glm::vec3(0.5f , -0.5f , 0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(1.0f , 1.f),//右下11

	glm::vec3(0.5f , -0.5f , 0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(0.0f , 0.f),//右下12
	glm::vec3(0.5f , 0.5f , 0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(0.0f , 1.0f),//右上13

	glm::vec3(-0.5f , 0.5f , 0.5f) , glm::vec3(1.0f , 0.f , 0.f) , glm::vec2(0.0f , 0.f),//左上14
	glm::vec3(0.5f , 0.5f , 0.5f) , glm::vec3(0.f , 0.f , 1.f) , glm::vec2(1.0f , 0.0f),//右上15


};
unsigned nrOfvertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] = {
	0,1,2,
	0,2,3,
	4,5,8,5,9,8,
	10,5,11,11,5,6,
	13,12,6,7,13,6,
	14,15,4,4,15,7,
	7,6,4,
	4,6,5
};
unsigned nrOfindices = sizeof(indices) / sizeof(GLuint);

void framebuffer_resize_callback(GLFWwindow *window, int fbw, int fbh) {
	glViewport(0, 0, fbw, fbh); //1, 2 param. (-1 ~ 1) will normalized 3,4 param
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camPosition += deltaTime * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camPosition -= deltaTime * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * deltaTime;

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

int main() {
	//init GLFW
	glfwInit();
	//CREATE WINDOW
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	int frameBufferWIDTH = 0;
	int frameBufferHEIGHT = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "dickdick", NULL, NULL);

	glfwGetFramebufferSize(window, &frameBufferWIDTH, &frameBufferHEIGHT);

	glfwMakeContextCurrent(window); // IMPORTANT FOR GLEW;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback); //call function when windows size is changed.

	//init GLEW (NEED WINDOW AND OPENGL CONTEXT)
	glewExperimental = GL_TRUE;
	//if error
	if (glewInit() != GLEW_OK) {
		std::cout << "Faild initialize glew" << "\n";
		throw "fail glew";
		glfwTerminate();
	}
	//opengl option
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//shader init
	//GLuint core_program;//unsigned integer as id

	Shader core_program(4, 4, "vertex_core.glsl", "fragment_core.glsl");

	glEnable(GL_DEPTH_TEST);

	//VAO VBO EBO
	GLuint VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//GLint arrtiblocation = glGetAttribLocation(core_program , "vertex_position")
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	//un bind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//TEXTURE INITIAL
	//unsigned int kappa = loadTexture("image/kappa.png");
	unsigned int ph = loadTexture("image/n.png");

	//matrix
	glm::vec3 pos(0.f);
	glm::vec3 rot(0.f);
	glm::vec3 scale(1.f);


	//set camera
	glm::mat4 view(1.f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	view = glm::lookAt(camPosition, cameraFront + camPosition, worldUp);


	//projection(視野)
	float nearPlane = 0.1f;
	float farPlane = 100.f;

	glm::mat4 projection(1.f);
	projection = glm::perspective(glm::radians(fov),
		static_cast<float>(frameBufferWIDTH) / frameBufferHEIGHT,
		nearPlane,
		farPlane
	);

	//bind uniform
	core_program.use();
	core_program.setInt("texture0", 0);
	core_program.setInt("texture1", 1);
	//core_program.setMat4("matrix",matrix);

	//trans
	glm::mat4 matrix[10];
	for (int i = 0; i < 10; ++i) {
		matrix[i] = glm::mat4(1.0f);
		matrix[i] = glm::translate(matrix[i], pos);
		matrix[i] = glm::rotate(matrix[i], glm::radians(rot.x), glm::vec3(1.f, 0.f, 0.f));
		matrix[i] = glm::rotate(matrix[i], glm::radians(rot.y), glm::vec3(0.f, 1.f, 0.f));
		matrix[i] = glm::rotate(matrix[i], glm::radians(rot.z), glm::vec3(0.f, 0.f, 1.f));
		matrix[i] = glm::scale(matrix[i], scale);
		matrix[i] = glm::translate(matrix[i], cubePositions[i]);
		core_program.setMat4("matrix", matrix[i]);
	}
	core_program.setMat4("view", view);
	core_program.setMat4("projection", projection);
	core_program.unuse();





	//MAIN LOOP
	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = (currentFrame - lastFrame)*5.5f;
		lastFrame = currentFrame;

		processInput(window);

		//clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.f);//RGBA
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//active texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ph);
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, kappa);*/

		//use program
		core_program.use();

		//update uniform for mixing 2 texture.
		/*float chv = abs(sin(glfwGetTime()));
		core_program.setFloat("change",chv );*/

		//view
		view = glm::lookAt(camPosition, cameraFront + camPosition, cameraUp);
		core_program.setMat4("view", view);

		//proj
		glfwGetFramebufferSize(window, &frameBufferWIDTH, &frameBufferHEIGHT);
		projection = glm::perspective(glm::radians(fov),
			static_cast<float>(frameBufferWIDTH / frameBufferHEIGHT),
			nearPlane,
			farPlane);
		core_program.setMat4("projection", projection);


		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, nrOfindices, GL_UNSIGNED_INT, 0);

		//trans 10 matries
		/*for (unsigned int i = 0; i < 10; ++i)
		{
			float angle = (0.5f * i)+1;
			matrix[i] = glm::rotate(matrix[i], glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			core_program.setMat4("matrix", matrix[i]);

			glDrawElements(GL_TRIANGLES, nrOfindices, GL_UNSIGNED_INT, 0);
		}*/

		//draw ARRAY
		//glDrawArrays(GL_TRIANGLES, 0, nrOfvertices);
		//glDrawElements(GL_TRIANGLES, nrOfindices, GL_UNSIGNED_INT, 0);

		//EOF
		glfwSwapBuffers(window);
		glFlush();

		//update input
		glfwPollEvents();

	}
	//END of program
	//DeleteProgram

	//glDeleteBuffers(1, &EBO);
	glfwDestroyWindow(window);

	core_program.unuse();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}