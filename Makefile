GLFW_FLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
SRCS = scop.cpp src/ft_loader.cpp src/ft_math.cpp src/ft_mesh.cpp src/ft_shader.cpp src/glad.c
OBJETS = $(SRCS:.cpp=.o)
OBJETS = $(SRCS:.c=.o)

IMGUI_SRCS = imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui.cpp \
				imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp
IMGUI_OBJETS = $(IMGUI_SRCS:.cpp=.o)

INCLUDE = -Iinclude -Iimgui -Iimgui/backends -Iinclude/glad

all: scop

scop: $(IMGUI_OBJETS) $(OBJETS)
	g++ -o $@ $^ $(INCLUDE) $(GLFW_FLAGS)

scop_debug: $(IMGUI_OBJETS) $(OBJETS)
	g++ -o $@ $^ $(INCLUDE) $(GLFW_FLAGS) -g -DDEBUG

%.o: %.cpp
	g++ -o $@ -c $< $(INCLUDE)

%.o: %.c
	gcc -o $@ -c $< $(INCLUDE)

clean:
	rm **/*.o
	rm **/**/*.o

fclean: clean
	rm scop