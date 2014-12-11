#include <string>
#include <iostream>
#include <vector>
#include "room.h"
#include "door.h"
#include "game.h"

using std::string;
using std::endl;
using std::cerr;

Door::Door(string data, std::unordered_map<string,Room*>* rooms, Game* game) : Actor(game,data){
  std::vector<string> d = split(data,';');
  string room1 = d[3];
  string room2 = d[4];
  d1 = d[5];
  d2 = d[6];
  lockedMessage = d[7];
  
  r1 = (*rooms)[room1];
  r2 = (*rooms)[room2];

  r1->addExit(d1,this);
  r2->addExit(d2,this);

}

Door::~Door(){
  
}

Room* Door::getConnectedRoom(Room* from) const{
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
  string result = "DOOR:";
  result += Actor::serialize();
  result += ";";
  result += r1->getName();
  result += ";";
  result += r2->getName();
  result += ";";
  result += d1;
  result += ";";
  result += d2;
  result += ";";
  result += getLockedMessage();

  return result;
}

string Door::getLockedMessage() const {
  return lockedMessage;
}

void Door::setLockedMessage(string s) {
  lockedMessage = s;
}
