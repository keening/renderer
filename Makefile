OBJS = glad.c shaders.c textures.c main.c 

OBJ_NAME = renderer

all : $(OBJS)
	gcc $(OBJS) -Wall -lglfw -lGL -ldl -lcglm -lm -o $(OBJ_NAME)
