GLFW_FLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
SRCS = src/ft_loader.cpp src/ft_math.cpp src/ft_mesh.cpp src/ft_shader.cpp src/glad.c

IMGUI_DIR = imgui
IMGUI_SRCS = $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
IMGUI_SRCS += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

scop: scop.cpp $(IMGUI_DIR)/imgui.cpp
	g++ -o $@ $^ $(GLFW_FLAGS) -Iinclude -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends $(SRCS) $(IMGUI_SRCS)
scop_debug: scop.cpp $(IMGUI_DIR)/imgui.cpp
	g++ -fsanitize=address -o $@ $^ $(GLFW_FLAGS) -DDEBUG -Iinclude -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends $(SRCS) $(IMGUI_SRCS)
remake:
	rm scop
	make scop
remaked:
	rm scopd
	make scopd
