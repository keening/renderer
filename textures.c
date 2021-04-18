#include "include/common.h"
#include "include/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

uint32_t generate_and_bind_texture(void)
{
	uint32_t texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	return texture;
}

void load_texture(char *filename, uint32_t id, char *uniform_name)
{
	int32_t width, height, nr_channels;
	stbi_set_flip_vertically_on_load(1);

	unsigned char *data = stbi_load(filename, &width, &height, &nr_channels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		printf("Failed to load texture\n");
	}

	stbi_image_free(data);

	glUseProgram(id);
	glUniform1i(glGetUniformLocation(id, uniform_name), 0);
}
