#ifndef ROOM_GUARD
#define ROOM_GUARD
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <iostream>
#include <functional>
#include "actor.h"

using std::string;
using std::endl;
using std::deque;
using std::unordered_set;

class Door; // Forward Declaration
class Game; // Forward Declaration
class Item; // Forward Declaration



struct finder{
  string target;
  int type; // 0 = all, 1 = item, 2 = character, 3 = Doors (NYI, might not be needed)
  unordered_set<Actor*> visited;
  deque<Actor*> bfsq;
  std::unordered_map<Actor*, Actor*> trackback;
  Actor* current;
  Actor* operator*() const{return current;};
  Actor* findNext();
};

class Room {
 protected:
  Game* g;
  std::unordered_map<string,Door*> exits;
  std::unordered_set<Actor*> actors;
  string name;
  string description;
  bool dark;
  std::unordered_set<Item*> items;
 public:
  Room(string data, Game* g);
  Room(string name, string description);
  ~Room();

  bool addExit(string s, Door* d);
  bool addExits(std::unordered_map<string,Door*> exits);
  Door* getExit(string s);
  
  void setDark(bool d) {dark = d;}
  bool isDark() const {
    if (!dark) return false;
    finder f = find("",0);
    while(f.findNext() != NULL) {
      if ((*f)->checkFlag("lightsource")) {
	return false;
      }
    }
    return true;
  }
  
  void setDescription(string s) {description = s;}
  string getDescription() const ;

  void setName(string s) {name = s;}
  string getName() const {return name;}

  std::unordered_set<Actor*> getActors() const {return actors;}
  void addActor(Actor* a) {actors.insert(a);}
  void removeActor(Actor* a) {actors.erase(a);}
  
  std::unordered_set<Item*> getItems() const {return items;}
  void addItem(Item* a) {items.insert(a);}
  void removeItem(Item* a) {items.erase(a);}
  
  finder find(string s, int type, Actor* start = NULL) const;  

  string serialize() const;

};

#endif
