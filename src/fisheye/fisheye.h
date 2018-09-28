/*
use input image of six directions to generate fisheye output
*/
#ifndef FISHEYE_H__
#define FISHEYE_H__


#define IMAGE_SIZE 1048576

#include<string>

#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shader.h"
#include"camera.h"
#include"texture.h"

#include"../vf.h"

class Fisheye {
public:
	//size of the original image
	const GLuint WIDTH = 512, HEIGHT = 512;
	int SCREEN_WIDTH, SCREEN_HEIGHT;

	Camera camera;

	bool keys[1024];
	GLfloat lastX = 400, lastY = 300;
	bool firstMouse = true;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	GLFWwindow *window;
	vector<Shader> shaders;
	GLuint cubemapVAO, cubemapVBO;

	const string bin_postfix[6] = {
		"_90_0.bin",
		"_270_0.bin",
		"_0_90.bin",
		"_0_270.bin",
		"_0_0.bin",
		"_180_0.bin"
	};

public:
	int init() {
		std::cout << "Initialize gl" << std::endl;
		camera = (glm::vec3(0.0f, 0.0f, 3.0f));
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//create glfw window
		window = glfwCreateWindow(WIDTH, HEIGHT, "cubemap"
			, nullptr, nullptr);
		if (nullptr == window) {
			std::cerr << "failed to create glfw window" << std::endl;
			glfwTerminate();
			return EXIT_FAILURE;
		}

		glfwMakeContextCurrent(window);
		glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		//initialize glew
		if (GLEW_OK != glewInit()) {
			std::cerr << "failed to initialize glew!" << std::endl;
			return EXIT_FAILURE;
		}

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		glEnable(GL_DEPTH_TEST);

		shaders.push_back(Shader("shaders/cubemap.vs", "shaders/cubemap_top.frag"));
		shaders.push_back(Shader("shaders/cubemap.vs", "shaders/cubemap_bottom.frag"));
		shaders.push_back(Shader("shaders/cubemap.vs", "shaders/cubemap_front.frag"));
		shaders.push_back(Shader("shaders/cubemap.vs", "shaders/cubemap_back.frag"));
		shaders.push_back(Shader("shaders/cubemap.vs", "shaders/cubemap_left.frag"));
		shaders.push_back(Shader("shaders/cubemap.vs", "shaders/cubemap_right.frag"));

		GLfloat cubemapVertices[] = {
			// Positions
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		glGenVertexArrays(1, &cubemapVAO);
		glGenBuffers(1, &cubemapVBO);
		glBindVertexArray(cubemapVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices),
			&cubemapVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE
			, 3 * sizeof(GLfloat), (GLvoid *)0);
		glBindVertexArray(0);
		glfwHideWindow(window);
		return 0;
	}

	int getVF(const string &filename, unsigned char *data, string &output) {
		vector<string> faces;
		for (int i = 0; i < 6; i++) {
			string s = filename + bin_postfix[i];
			faces.push_back(s);
		}
		GLuint cubemapTexture = TextureLoading::loadBinary(faces);

		glm::mat4 projection = glm::perspective(
			camera.GetZoom()
			, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT)
			, 0.1f
			, 1000.0f
		);

		for (int i = 0; i < 6; i++) {
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 model;
			glm::mat4 view = camera.GetViewMatrix();

			// Change depth function so depth test passes when values 
			// are equal to depth buffer's content
			glDepthFunc(GL_LEQUAL);
			shaders[i].Use();
			view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

			glUniformMatrix4fv(glGetUniformLocation(shaders[i].Program, "view")
				, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(
				glGetUniformLocation(shaders[i].Program, "projection")
				, 1, GL_FALSE, glm::value_ptr(projection)
			);

			glBindVertexArray(cubemapVAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);

			glReadnPixels(0, 0, WIDTH, HEIGHT, GL_RED, GL_UNSIGNED_BYTE
				, WIDTH * HEIGHT * sizeof(unsigned char), data);
			output += calculate(data);

			glfwSwapBuffers(window);
		}
		glDeleteTextures(1, &cubemapTexture);
		return 0;
	}

	Fisheye() {
		init();
	}

	~Fisheye() {
		glfwTerminate();
	}
};

#endif // !FISHEYE_H__