default: game.exe

game.exe: main.cpp parser.cpp parser.h game.h actor.h room.h door.h item.h character.h player.h player.cpp door.cpp room.cpp character.cpp game.cpp makefile container.h container.cpp item.cpp npc.h npc.cpp actor.cpp
	g++ -o game.exe -D_GLIBCXX_DEBUG -static -g -std=c++11 main.cpp parser.cpp parser.h game.h actor.h door.h room.h item.h character.h player.h player.cpp door.cpp room.cpp character.cpp game.cpp container.h container.cpp item.cpp npc.h npc.cpp actor.cpp
	./game.exe || echo sure



