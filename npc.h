#include <string>

#include "character.h"

using std::string;

class NPC : public Character {
 protected:
 public:
  NPC(string data, Game* instance, Room* location) : Character(instance,location,data) {}

  static const int C_USED_WORDS;
  
  virtual string serialize() const;
  virtual void death() {}
  virtual bool drop(std::string args) {}
  virtual bool go(std::string args) {}
  virtual bool attack(std::string args) {}
  virtual bool pick_up(std::string args) {}
  

};
