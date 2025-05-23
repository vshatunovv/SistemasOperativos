# Nombre del ejecutable
TARGET = backup

# Compilador
CXX = g++
CXXFLAGS = -fopenmp -std=c++17

# Bibliotecas necesarias
LIBS = -lz -lssl -lcrypto

# Archivos fuente
SRCS = main.cpp \
       gestor_archivos.cpp \
       compresor.cpp \
       encriptador.cpp \
       restaurador.cpp \
       utils.cpp

# Archivos objeto (reemplaza .cpp por .o)
OBJS = $(SRCS:.cpp=.o)

# Regla por defecto
all: $(TARGET)

# Regla de compilacion
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Regla para archivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(TARGET) $(OBJS)
