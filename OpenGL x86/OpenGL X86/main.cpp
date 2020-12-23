#include "libs.h"

/*
*	Link Repo
*	https://github.com/notslimboy/OpenGL.git
*  
*
*/

Vertex Vertices[] = 
{
	// positions						// colors						// texcoords
	glm::vec3(-0.5f, 0.5f, 0.f),		glm::vec3(1.f, 0.f, 0.f),		glm::vec2(0.f, 1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),		glm::vec3(0.f, 1.f, 0.f),		glm::vec2(0.f, 0.f),
	glm::vec3(0.5f, -0.5f, 0.f),		glm::vec3(0.f, 0.f, 1.f),		glm::vec2(1.f, 0.f),
	glm::vec3(0.5f, 0.5f, 0.f),			glm::vec3(1.f, 1.f, 0.f),		glm::vec2(1.f, 1.f)
};
unsigned nrOfVertices = sizeof(Vertices) / sizeof(Vertex);

GLuint Indices[] = 
{
	0, 1, 2,	// triangle1
	0, 2, 3		// triangle2
};
unsigned nrOfIndices = sizeof(Indices) / sizeof(GLuint);

// pressed key for input
void updateInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) 
{
	glViewport(0, 0, fbW, fbH);
}

bool loadShaders(GLuint &program) 
{
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;

	string temp = "";
	string src = "";
	ifstream in_file;

	// vertex core
	in_file.open("vertex_core.glsl");

	if (in_file.is_open()) 
	{
		while (getline(in_file, temp))
			src += temp + "\n";
	}
	else 
	{
		cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
		loadSuccess = false;
	}
	
	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		cout << infoLog << "\n";

		loadSuccess = false;
	}

	temp = "";
	src = "";

	// fragment core
	in_file.open("fragment_core.glsl");

	if (in_file.is_open()) 
	{
		while (getline(in_file, temp))
			src += temp + "\n";
	}
	else 
	{
		cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		cout << infoLog << "\n";

		loadSuccess = false;
	}

	// program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
		cout << infoLog << "\n";

		loadSuccess = false;
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

int main() 
{
	// inisialisasi glfw
	glfwInit();

	// elemen-elemen window
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "4210181030_Raka Arya Pratama", NULL, NULL);

	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	//glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	//glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);

	// inisialisasi glew
	glewExperimental = GL_TRUE;

	// error checking
	if (glewInit() != GLEW_OK) 
	{
		cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	// OpenGL option
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// inisialisasi shader
	GLuint core_program;
	if (!loadShaders(core_program))
		glfwTerminate();

	// VAO
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	// set attribute
	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// colors
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	// texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	// bind VAO
	glBindVertexArray(0);

	// inisialisasi texture
	// texture 0
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image("Images/hack.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);

	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);


	// TODO uncomment this to activate texture 1

	// texture 1
	int image_width1 = 0;
	int image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("Images/wood.jpeg", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image1) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
	{
		cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image1);

	// Change Translate, Rotate, Scale at once
	glm::mat4 ModelMatrix(1.f);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f,0.f,0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f) , glm::vec3(1.f,0.f,0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix,  glm::vec3(1.f));

	glUseProgram(core_program);


	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

	glUseProgram(0);

	// main loop
	while (!glfwWindowShouldClose(window)) 
	{
		// update input
		glfwPollEvents();

		// update
		updateInput(window);

		// clear
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// use program
		glUseProgram(core_program);

		// update uniform
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);
		
		// Move, Rotate, Scale update per frame
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.f));
		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));

		// activate texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// bind vertex
		glBindVertexArray(VAO);

		// draw
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		// end draw
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// end of program
	glfwDestroyWindow(window);
	glfwTerminate();

	// delete program
	glDeleteProgram(core_program);

	return 0;
}