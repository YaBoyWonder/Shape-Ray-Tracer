include local.mak

TARGET1 = caster
cpp_files1 = caster_main.cpp caster.cpp program.cpp \
 vertex_buffer.cpp element_buffer.cpp vertex_array.cpp mesh.cpp \
 caster_view.cpp caster_controller.cpp camera.cpp hit.cpp material.cpp \
 window.cpp shape.cpp triangle.cpp sphere.cpp cylinder.cpp light.cpp \
 image.cpp texture.cpp gl_error.cpp log.cpp scene_reader.cpp tokenizer.cpp

objects1 = $(cpp_files1:.cpp=.o) $(c_files:.c=.o)

all: $(TARGET1)

$(TARGET1): $(objects1)
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY : clean
clean :
	-rm -f $(TARGET1) $(objects1)
