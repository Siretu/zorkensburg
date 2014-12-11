#ifndef ACTOR_GUARD
#define ACTOR_GUARD
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include "game.h"

using std::string;
using std::cerr;
using std::endl;

class Character; //Forward Declaration

class Actor {
 protected:
  std::vector<string> _names;
  string _desc;
  std::unordered_map<string,std::vector<std::function<bool(string, Character*)>>> events;
  std::unordered_set<string> flags;

 public:
  bool hidden = false;
  Game* g;
  static const int C_USED_WORDS;

  Actor(Game* instance, string data);
  virtual ~Actor();

  virtual bool action();
  
  virtual std::string serialize() const;

  virtual std::unordered_set<Actor*> getContained() const;
  
  virtual bool removeItem(Actor*);

  void addNames(std::vector<string> s);
  void addName(string s);
  std::vector<string> getNames() const;
  string getName() const;
  bool hasName(string s) const;

  void setDesc(string s);
  string getDesc() const;

  bool doEvent(string s,string t);
  void addEvent(string s, std::function<bool(string,Character*)> l);

  void addFlags(std::vector<string> s);
  std::unordered_set<string> getFlags() const;
  void addFlag(string s);
  void removeFlag(string s);
  bool checkFlag(string s) const;
  
  string getID() const;

};

#endif
