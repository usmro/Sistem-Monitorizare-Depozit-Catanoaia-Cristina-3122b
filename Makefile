CXX      = "C:/Program Files/CodeBlocks/MinGW/bin/g++"
CXXFLAGS = -std=c++17 -Wall -Wextra -static-libgcc -static-libstdc++ -static

SRCS = main.cpp \
       src/Produs.cpp \
       src/ProdusPerisabil.cpp \
       src/ProdusElectronic.cpp \
       src/ProdusISBN.cpp \
       src/Sofer.cpp \
       src/StaffDepozit.cpp \
       src/Vehicul.cpp \
       src/Furnizor.cpp \
       src/Depozit.cpp \
       src/UI.cpp \
       src/FileManager.cpp

TARGET = depozit.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: all
	./$(TARGET)

test:
	$(CXX) $(CXXFLAGS) tests/test_depozit.cpp \
	    src/Produs.cpp src/ProdusPerisabil.cpp src/ProdusElectronic.cpp \
	    src/ProdusISBN.cpp src/Sofer.cpp src/StaffDepozit.cpp src/Vehicul.cpp \
	    src/Furnizor.cpp src/Depozit.cpp src/FileManager.cpp \
	    -o test_depozit.exe
	./test_depozit.exe

clean:
	del depozit.exe test_depozit.exe 2>NUL & exit 0

.PHONY: all run test clean
