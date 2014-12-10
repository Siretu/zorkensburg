#include <string>
#include <iostream>
#include "character.h"
#include "room.h"
#include "door.h"
#include "game.h"
#include "player.h"

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


void Character::triggerEnter(Room* r) {
  doEvent("onEnter#"+r->getName(),"");
}

string Character::serialize() const{
  string result = Actor::serialize();
  result += "\n";

  auto items = getInventory()->getItems();
  for (auto it = items.begin(); it != items.end();++it) {
    result += (*it)->serialize();
    result += "\n";
  }

  result += "ENDCHAR";
  return result;
}
