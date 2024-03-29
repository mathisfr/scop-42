GLFW_FLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
SRCS = scop.cpp src/ft_loader.cpp src/ft_math.cpp src/ft_mesh.cpp src/ft_shader.cpp src/ft_light.cpp src/glad.c
OBJETS_TMP = $(SRCS:.cpp=.o)
OBJETS = $(OBJETS_TMP:.c=.o)

IMGUI_SRCS = imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp imgui/imgui.cpp \
				imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp
IMGUI_OBJETS = $(IMGUI_SRCS:.cpp=.o)

INCLUDE = -Iinclude -Iimgui -Iimgui/backends -Iinclude/glad

all: scop

scop: $(IMGUI_OBJETS) $(OBJETS)
	$(info $$OBJETS is [${OBJETS}])
	g++ -o $@ $^ $(INCLUDE) $(GLFW_FLAGS)

%.o: %.cpp
	g++ -o $@ -c $< $(INCLUDE)

%.o: %.c
	gcc -o $@ -c $< $(INCLUDE)

zip: models textures
	zip -r textures.zip ./textures/*.bmp
	zip -r models.zip ./models/*.obj
	rm -rf textures
	rm -rf models

unzip:
	unzip textures.zip
	unzip models.zip
	rm -f *.zip

clean:
	rm -f imgui.ini
	rm -f *.o
	rm -f **/*.o
	rm -f **/**/*.o

fclean: clean
	rm -f scop
	rm -f scop_debug

re: fclean scop
