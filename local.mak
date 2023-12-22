CC = gcc
CXX = g++ -std=c++14

# LOCAL_ROOT = C:/Users/rahul/Desktop/UNH/cs770/local-mingw/ 
LOCAL_ROOT = C:\Users\rahul\Desktop\UNH\cs770\local-mingw

glad_inc = $(LOCAL_ROOT)/deps
c_files = $(LOCAL_ROOT)/deps/glad.c
INCLUDES = -I$(glad_inc) -I/usr/local/include -I$(LOCAL_ROOT)/include

CFLAGS = -Wall -ggdb -g $(INCLUDES)
CXXFLAGS = -Wall -ggdb -g $(INCLUDES)

LIBRARIES = -L$(LOCAL_ROOT)/lib
LDFLAGS = $(LIBRARIES) -lglfw3dll -lopengl32
