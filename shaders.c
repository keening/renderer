#include "include/shaders.h"
#include "include/common.h"
#include "include/glad.h"

#define TEXT_MAX 512

uint32_t create_shader_from_source(char const *filename, char *type)
{
	FILE *fptr;
	int file_len;
	char *src;

	fptr = fopen(filename, "rb");
	fseek(fptr, 0, SEEK_END);
	file_len = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	src = (char *)malloc(file_len + 1);
	fread(src, 1, file_len, fptr);
	src[file_len] = 0;
	fclose(fptr);

	const char *shader_src = src;
	uint32_t shader;

	if (!strcmp(type, "vertex")) {
		shader = glCreateShader(GL_VERTEX_SHADER);
	} else {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader); 

	check_compile_errors(shader, type);
	
	free(src);

	return shader;
}

uint32_t create_shader_program(char const *vertex_path, char const *frag_path)
{
	uint32_t vertex, fragment, id;
	vertex = create_shader_from_source(vertex_path, "vertex");
	fragment = create_shader_from_source(frag_path, "fragment");

	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);

	glLinkProgram(id);
	check_compile_errors(id, "program");

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return id;
}


void check_compile_errors(uint32_t shader, char *type)
{
	int success;
	char info_log[TEXT_MAX];
	if (!strcmp(type, "program")) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);	

		if (!success) {
			glGetShaderInfoLog(shader, TEXT_MAX, NULL, info_log);
			printf("Shader compilation error for %s:%s\n", type, info_log);
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, TEXT_MAX, NULL, info_log);
			printf("Program linking error for %s:%s\n", type, info_log);
		}
	}
}
