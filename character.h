#ifndef CHARACTER_GUARD
#define CHARACTER_GUARD

#include <string>
#include <unordered_set>
#include "actor.h"
#include "container.h"

class Room; // Foward Declaration

class Character : public Actor{

 protected:
  Container* _inventory;
  Room* location;
  int health;

 public:
 Character(Game* instance, Room* l, std::string s) : Actor(instance,s,s), location(l) {}

  virtual bool action() = 0;

  virtual bool drop(std::string args) = 0;
  virtual bool go(std::string args) = 0;
  virtual bool attack(std::string args) = 0;
  virtual bool pick_up(std::string args) = 0;
  

  virtual std::string serialize() const = 0;  

  bool enter(std::string dir);

  Room* getLocation() const {return location;}
  
  Container* getInventory() const {return _inventory;}

  virtual std::unordered_set<Actor*>* getContained() const {return _inventory->getContained();}

};
#endif
