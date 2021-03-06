#include "include/common.h"
#include "include/glad.h"

#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#define SCR_WIDTH  800
#define SCR_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow *window);

extern uint32_t create_shader_program(char const *vertex_path, char const *frag_path);
extern uint32_t generate_and_bind_texture(void);
extern void load_texture(char *filename, uint32_t id, char *uniform_name);

vec3 camera_pos = {0.0f, 0.0f, 3.0f}; vec3 camera_front = {0.0f, 0.0f, -1.0f}; vec3 camera_up = {0.0f, 1.0f, 0.0f};

float delta_time = 0.0f, last_frame = 0.0f;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Renderer", NULL, NULL);

	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	uint32_t id = create_shader_program("shaders/vertex_source.glsl", "shaders/frag_source.glsl");

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 cube_positions[] = {
		{ 0.0f,  0.0f,  0.0f},
		{ 2.0f,  5.0f, -15.0f}, 
		{-1.5f, -2.2f, -2.5f}, 
		{-3.8f, -2.0f, -12.3f},
		{ 2.4f, -0.4f, -3.5f},  
		{-1.7f,  3.0f, -7.5f},  
		{ 1.3f, -2.0f, -2.5f},  
		{ 1.5f,  2.0f, -2.5f}, 
		{ 1.5f,  0.2f, -1.5f}, 
		{-1.3f,  1.0f, -1.5f}  
	};

	uint32_t VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	/* texture part */
	uint32_t texture1 = generate_and_bind_texture();
	load_texture("resources/textures/container.jpg", id, "texture1");

	/* enable z-buffer */
	glEnable(GL_DEPTH_TEST);

	/* projection matrix */
	mat4 projection = {
		    { 1.0f, 0.0f, 0.0f, 0.0f },
		    { 0.0f, 1.0f, 0.0f, 0.0f },
		    { 0.0f, 0.0f, 1.0f, 0.0f },
		    { 0.0f, 0.0f, 0.0f, 1.0f },
	};

	glm_perspective(glm_rad(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);
	glUniformMatrix4fv(glGetUniformLocation(id, "projection"), 1, GL_FALSE, (float *)projection); 

	/* main loop */
	while (!glfwWindowShouldClose(window)) {

		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		process_input(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glUseProgram(id);

		/* setup camera */
		mat4 view;
		vec3 camera_sum;
		glm_vec3_add(camera_pos, camera_front, camera_sum);
		glm_lookat(camera_pos, camera_sum, camera_up, view);
		glUniformMatrix4fv(glGetUniformLocation(id, "view"), 1, GL_FALSE, (float *)view); 

		glBindVertexArray(VAO);
		/* spawn multiple cubes */
		for(unsigned int i = 0; i < 10; i++) {
			/* make multiple cubes */
			mat4 model = {
				{ 1.0f, 0.0f, 0.0f, 0.0f},
				{ 0.0f, 1.0f, 0.0f, 0.0f},
				{ 0.0f, 0.0f, 1.0f, 0.0f},
				{ 0.0f, 0.0f, 0.0f, 1.0f}
			};
			glm_translate(model, cube_positions[i]);
			float angle = 20.0f * i;
			glm_rotate(model, glm_rad(angle), (vec3){1.0f, 0.3f, 0.5f});
			glUniformMatrix4fv(glGetUniformLocation(id, "model"), 1, GL_FALSE, (float *)model); 

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}

void process_input(GLFWwindow *window)
{
	float camera_speed = 2.5f * delta_time;
	vec3 camera_cross, camera_mul;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, 1);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm_vec3_scale(camera_front, camera_speed, camera_mul);
		glm_vec3_add(camera_pos, camera_mul, camera_pos);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm_vec3_scale(camera_front, camera_speed, camera_mul);
		glm_vec3_sub(camera_pos, camera_mul, camera_pos);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		glm_cross(camera_front, camera_up, camera_cross);
		glm_normalize(camera_cross);
		glm_vec3_scale(camera_cross, camera_speed, camera_cross);
		glm_vec3_sub(camera_pos, camera_cross, camera_pos);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		glm_cross(camera_front, camera_up, camera_cross);
		glm_normalize(camera_cross);
		glm_vec3_scale(camera_cross, camera_speed, camera_cross);
		glm_vec3_add(camera_pos, camera_cross, camera_pos);
	}



}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
