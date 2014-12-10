#include <string>
#include <iostream>
#include "character.h"
#include "room.h"
#include "door.h"
#include "game.h"

using std::string;

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
      if (checkFlag("playerishere")) {
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
    cerr << "Inventory done, makingg health." << endl;
    health = stoi(split(data, ';')[i]);
    cerr << "Health done. " << endl;
  }


void Character::triggerEnter(Room* r) {
  doEvent("onEnter#"+r->getName(),"");
}

string Character::serialize() const{
  string result = Actor::serialize();
  result += ";";
  result += health;
  result += "\n";

  auto items = getInventory()->getItems();
  for (auto it = items.begin(); it != items.end();++it) {
    result += (*it)->serialize();
    result += "\n";
  }

  result += "ENDCHAR";
  return result;
}
