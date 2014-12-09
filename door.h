#ifndef DOOR_GUARD
#define DOOR_GUARD
#include <string>
#include <unordered_map>
#include "actor.h"

using std::string;

class Room; //Forward declaration
class Game; // Forward Declaration

class Door : public Actor {
 protected:
  Game* g;
  string lockedMessage;
  
 public:
  Room* r1;
  Room* r2;
  string d1;
  string d2;

  Door(string data, std::unordered_map<string,Room*>* rooms, Game* g);

  Room* getConnectedRoom(Room* current) const;
  
  string serialize() const;

  string getLockedMessage() const;
  void setLockedMessage(string s);

  virtual std::unordered_set<Actor*>* getContained() const {
    return new std::unordered_set<Actor*>;
  }

  virtual bool action(){}

};
#endif
