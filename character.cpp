#include <string>
#include <iostream>
#include "character.h"
#include "room.h"
#include "door.h"
#include "game.h"

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
      return true;
    } else {
      g->push(exit->getLockedMessage());
      return true;
    }
  }  
}
