#include <string>
#include <iostream>
#include <vector>
#include "room.h"
#include "door.h"
#include "game.h"

using std::string;
using std::endl;
using std::cerr;

Door::Door(string data, std::unordered_map<string,Room*>* rooms, Game* game) : Actor(game,"","Foo"){
  int index1 = data.find_first_of(";");
  int index2 = data.find_first_of(";", index1 + 1);
  int index3 = data.find_first_of(";", index2 + 1);
  int index4 = data.find_first_of(";", index3 + 1);
  int index5 = data.find_first_of(";", index4 + 1);
  int index6 = data.find_first_of(";", index5 + 1);

  string room1 = data.substr(0,index1);
  string room2 = data.substr(index1 + 1, index2 - index1 - 1);
  string dir1 = data.substr(index2 + 1, index3 - index2 - 1);
  string dir2;
  dir2 = data.substr(index3 + 1, index4 - index3 - 1);
  lockedMessage = data.substr(index4 + 1, index5 - index4 - 1);
  string names = data.substr(index5 + 1, index6 - index5 - 1);
  addNames(parseNames(names));
  if (_names.size() > 1) {
    id = _names[1];
  } else if (_names.size() > 0) {
    id = _names[0];
  } else {
    id = "door";
  }
  string flags = data.substr(index6 + 1);
  addFlags(parseNames(flags));
  
  r1 = (*rooms)[room1];
  r2 = (*rooms)[room2];
  d1 = dir1;
  d2 = dir2;
  r1->addExit(dir1,this);
  r2->addExit(dir2,this);
  g = game;

}


Door::Door(Room* room1, Room* room2, string dir1, string dir2, Game* game) : Actor(game,"Door","Bar") {
  room1->addExit(dir1,this);
  room2->addExit(dir2,this);
  r1 = room1;
  r2 = room2;
  d1 = dir1;
  d2 = dir2;
  g = game;
}

Room* Door::getConnectedRoom(Room* from){
  if (lockedMessage == "") {
    if(from == r1){
      return r2;
    }else if(from == r2){
      return r1;
    }else{
      g->push("You're in a strange, strange place...");
      return r1;
    }
  } else {
    return NULL;
  }
}

string Door::serialize() const{
  for (auto it = flags.begin(); it != flags.end();++it) {
    cerr << "Found flag: " << *it << endl;
  }


  string result = "DOOR:";
  result += r1->getName();
  result += ";";
  result += r2->getName();
  result += ";";
  result += d1;
  result += ";";
  result += d2;
  result += ";";
  result += getLockedMessage();
  result += ";";
  result += id;
  result += ";";
  result += getFlagsSerialize();

  result += "\n";
  return result;
}

string Door::getLockedMessage() const {
  return lockedMessage;
}

void Door::setLockedMessage(string s) {
  lockedMessage = s;
}
