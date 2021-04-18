unsigned int create_shader_from_source(const char *filename, char *type);
unsigned int create_shader_program(char const *vertex_path, char const *frag_path);
void check_compile_errors(unsigned int shader, char *type);
