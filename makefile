# 2019-12-15

TARGET=game
CXX=g++
CXXFLAGS=-Wall -std=c++17 -g -Og
RCXXFLAGS=-Wall -std=c++17 -O3
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
.PHONY=all clean document run

all: $(TARGET)

clean:
	rm -f .*.o $(TARGET)

document:
	doxygen Doxyfile

run: $(TARGET)
	./$(TARGET)

$(TARGET): .entity.o .IHIFT.o .player.o .projectile.o .utils.o .monster.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

release: .rel_entity.o .rel_IHIFT.o .rel_player.o .rel_projectile.o .rel_utils.o 
	$(CXX) $(RCXXFLAGS) $(LDFLAGS) $^ -o $(TARGET)
	strip $(TARGET)

.entity.o: entity.cpp entity.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.rel_entity.o: entity.cpp entity.hpp
	$(CXX) $(RCXXFLAGS) -c $< -o $@

.IHIFT.o: IHIFT.cpp entity.hpp player.hpp projectile.hpp utils.hpp monster.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.rel_IHIFT.o: IHIFT.cpp entity.hpp player.hpp projectile.hpp utils.hpp
	$(CXX) $(RCXXFLAGS) -c $< -o $@

.player.o: player.cpp player.hpp projectile.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.rel_player.o: player.cpp player.hpp projectile.hpp utils.hpp
	$(CXX) $(RCXXFLAGS) -c $< -o $@

.projectile.o: projectile.cpp projectile.hpp utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.rel_projectile.o: projectile.cpp projectile.hpp utils.hpp
	$(CXX) $(RCXXFLAGS) -c $< -o $@

.utils.o: utils.cpp utils.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.rel_utils.o: utils.cpp utils.hpp
	$(CXX) $(RCXXFLAGS) -c $< -o $@

.monster.o: monster.cpp monster.hpp projectile.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
