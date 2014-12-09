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

typedef bool (*lambdas) (std::string);

class Actor {
 protected:
  std::vector<string> _names;
  string _desc;
  std::unordered_map<string,std::vector<std::function<bool(string)>>> events;
  std::unordered_set<string> flags;

 public:
  bool hidden = false;
  string id;
  Game* g;
 Actor(Game* instance, string n, string d) : _desc(d), g(instance), id(n){
    _names = parseNames(n);
  }
 Actor(Game* instance) : Actor(instance,"Some actor","Just another actor description") {}

  virtual bool action() = 0;
  
  virtual std::string serialize() const = 0;

  virtual std::unordered_set<Actor*>* getContained() const {
    return new std::unordered_set<Actor*>;
  }
  
  std::vector<string> parseNames(string n) {
    return split(n,'#');
  }

  virtual bool removeItem(Actor*) {return false;}

  void addFlags(std::vector<string> s) {
    for(auto it = s.begin(); it != s.end(); ++it) {
      addFlag(*it);
    }
  }

  void addNames(std::vector<string> s) {
    for(auto it = s.begin(); it != s.end(); ++it) {
      addName(*it);
    }
  }

  void addName(string s) {_names.push_back(s);}

  std::vector<string> getNames() const {return _names;}

  string getName() const {
    if (_names.size() > 0) {
      return _names[0];
    } else {
      return "";
    }
  };
  bool hasName(string s) {
    for (auto it = _names.begin(); it != _names.end(); ++it) {
      if (strcomp(*it,s)) {
	return true;
      }
    }
    return false;
  }

  void setDesc(string s) {_desc = s;}
  string getDesc() const {return _desc;}

  bool doEvent(string s,string t) {
    auto it = events.find(s);
    bool didsmth = false;
    if(it!=events.end()) {
      std::vector<std::function<bool(string)>> e = (*it).second;
      for (auto it2 = e.begin(); it2 != e.end();++it2) {
	if ((*it2)(t)) {
	  didsmth = true;
	}
      }
      return didsmth;
    } else {
      return false;
    }
  }
  void addEvent(string s, std::function<bool(string)> l) {
    events[s].push_back(l);
  }

  std::unordered_set<string> getFlags() const {return flags;}
  void addFlag(string s) {flags.insert(s);}
  void removeFlag(string s) {flags.erase(s);}
  bool checkFlag(string s) const{
    if (s == "") return true;
    std::vector<string> f = split(s,'&');
    for (auto it = f.begin(); it != f.end(); ++it) {
      string temp = *it;
      if (temp[0] == '!') {
	temp = temp.substr(1);
	if (flags.find(temp) != flags.end()) return false;
      } else {
	if (flags.find(temp) == flags.end()) return false;
      }
    }
    return true;
  }

  string getNamesSerialize() const{
    string result = getName();
    if (_names.size() > 0) {
      for (auto it = ++(_names.begin());it != _names.end();++it) {
	result += "#";
	result += *it;
      }
    }
    return result;
  }

  string getFlagsSerialize() const{
    string result = "";
    if (flags.size() > 0) {
      for (auto it = flags.begin();it != flags.end();++it) {
	result += "#";
	result += *it;
      }
      result.erase(0,1);
    }
    return result;
  }
  
};

#endif
