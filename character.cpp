#include <string>
#include <iostream>
#include "character.h"
#include "room.h"
#include "door.h"
#include "game.h"
#include "player.h"
#include "container.h"

using std::string;
using std::cerr;
using std::endl;

bool Character::enter(std::string dir) {
  Door* exit = location->getExit(dir);
  if (exit == NULL) {
    return false;
  } else {
    Room* newLocation = exit->getConnectedRoom(location);
    if (newLocation != NULL) {
      location->removeActor(this);
      location = newLocation;
      location->addActor(this);
      if (g->player->getLocation() == location) {
	addFlag("playerishere");
      } else if(checkFlag("playerishere")){
	removeFlag("playerishere");
      }
      triggerEnter(newLocation);
      return true;
    } else {
      g->push(exit->getLockedMessage());
      return true;
    }
  }
}

//number of ';'-separated words used by the constructor
const int Character::C_USED_WORDS = Actor::C_USED_WORDS + 1;

Character::Character(Game* instance, Room* l, std::string data) : Actor(instance,data), location(l) {
    _inventory = new Container(instance);
    int i = Actor::C_USED_WORDS;
    health = stoi(split(data, ';')[i]);
  }

Character::~Character(){
  /*  for(auto it = _inventory.begin(); it != _inventory.end(); ++it){
    delete *it;
    }*/
  delete _inventory;
}

void Character::triggerEnter(Room* r) {
  doEvent("onEnter#"+r->getName(),"");
}

string Character::serialize() const{
  string result = Actor::serialize();
  result += ";";
  result += std::to_string(health);
  result += "\n";

  auto items = getInventory()->getItems();
  for (auto it = items.begin(); it != items.end();++it) {
    result += (*it)->serialize();
    result += "\n";
  }

  result += "ENDCHAR";
  return result;
}
