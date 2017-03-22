#Makefile for the project
EXECUTABLE = eztrace

#parts

DATA_OBJECTS =					data/datanode.o \
								data/image.o \
								data/json.o \
								data/stepdocument.o
LIB_OBJECTS =					lib/glm.o \
								lib/lodepng.o
MATH_OBJECTS =					math/math.o \
								math/matrix4x4.o \
								math/vector3.o \
								math/vector4.o
RAYTRACER_OBJECTS =				raytracer/camera.o \
								raytracer/hit.o \
								raytracer/material.o \
								raytracer/pointlight.o \
								raytracer/ray.o \
								raytracer/scene.o
RAYTRACER_RENDERING_OBJECTS =	raytracer/rendering/rendermodel.o \
								raytracer/rendering/phongshadingmodel.o
RAYTRACER_SHAPES_OBJECTS = 		raytracer/shapes/shape.o \
								raytracer/shapes/sphere.o \
								raytracer/shapes/triangle.o

OBJECTS = 						$(DATA_OBJECTS) \
								$(LIB_OBJECTS) \
								$(MATH_OBJECTS) \
								$(RAYTRACER_OBJECTS) \
								$(RAYTRACER_RENDERING_OBJECTS) \
								$(RAYTRACER_SHAPES_OBJECTS) \
								core.o main.o

#compiler settings
COMPILER = g++
FLAGS = -std=c++14 -O3 -Wall -fomit-frame-pointer -ffast-math -flto
#FLAGS = -std=c++14 -Wall -g
LIBRARIES = -lm -lpthread

#directory structure
SOURCE_DIRECTORY = src
BUILD_DIRECTORY = build
STRUCTURE_FLAG = $(BUILD_DIRECTORY)/structure.flag

BUILD_FILES = $(addprefix $(BUILD_DIRECTORY)/,$(OBJECTS))

#build executable command
$(EXECUTABLE): $(STRUCTURE_FLAG) $(BUILD_FILES)
	$(COMPILER) $(FLAGS) $(BUILD_FILES) -o $@ $(LIBRARIES)

#dependencies
-include $(BUILD_FILES:.o=.d)

#ensure folder structure
$(STRUCTURE_FLAG):
	mkdir -p $(filter-out %./,$(addprefix $(BUILD_DIRECTORY)/,$(dir $(OBJECTS))))
	touch $(STRUCTURE_FLAG)

#rebuild-commands
clean:
	rm -rf build/
	rm -rf $(EXECUTABLE) $(EXECUTABLE).exe

rebuild: clean $(EXECUTABLE)

restructure:
	rm $(STRUCTURE_FLAG)

#build and run commands
run: $(EXECUTABLE)
	./$(EXECUTABLE)

#compile objects rule
$(BUILD_DIRECTORY)/%.o: $(SOURCE_DIRECTORY)/%.cpp
	$(COMPILER) $(FLAGS) -o $@ -c $<
	$(COMPILER) -MM -MT $(BUILD_DIRECTORY)/$*.o src/$*.cpp > $(BUILD_DIRECTORY)/$*.d