#ifndef CHARACTER_GUARD
#define CHARACTER_GUARD

#include <algorithm>
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
  static const int C_USED_WORDS;
  Character(Game* instance, Room* l, std::string data);

  virtual void death() = 0;

  virtual bool drop(std::string args) = 0;
  virtual bool go(std::string args) = 0;
  virtual bool attack(std::string args) = 0;
  virtual bool pick_up(std::string args) = 0;
  
  virtual std::string serialize() const;

  bool enter(std::string dir);

  Room* getLocation() const {return location;}
  
  Container* getInventory() const {return _inventory;}

  virtual std::unordered_set<Actor*>* getContained() const {return _inventory->getContained();}

  void triggerEnter(Room* l);
  
  int getHealth() {return health;}
  void setHealth(int h) {health = h;}
  void damageHealth(int i) {
    health = std::max(0,health-i);
    if (health <= 0) {
      death();
    }
  }
};
#endif
