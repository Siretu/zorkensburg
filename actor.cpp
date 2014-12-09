#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>
#include "game.h"
#include "actor.h"

using std::string;
using std::cerr;
using std::endl;

Actor::Actor(Game* instance, string data) : g(instance){
  std::vector<string> d = split(data,';');
  auto d1 = split(d[0],'#');
  auto d2 = split(d[1],'#');
  addNames(d1);
  addFlags(d2);
  cerr << "Parsing desc" << endl;
  cerr << data << endl;
  _desc = d[2];
  cerr << "Done parsing desc" << endl;
}

bool Actor::action() {
  doEvent("onAction#","");
}
  
std::string Actor::serialize() const {
  string result = "";
  result += join(_names,'#');
  result += ";";
  result += join(flags,'#');
  result += ";";
  result += getDesc();
  return result;
}

std::unordered_set<Actor*>* Actor::getContained() const {
  return new std::unordered_set<Actor*>;
}

bool Actor::removeItem(Actor*) {return false;}

void Actor::addFlags(std::vector<string> s) {
  for(auto it = s.begin(); it != s.end(); ++it) {
    addFlag(*it);
  }
}

void Actor::addNames(std::vector<string> s) {
  for(auto it = s.begin(); it != s.end(); ++it) {
    addName(*it);
  }
}

void Actor::addName(string s) {_names.push_back(s);}

std::vector<string> Actor::getNames() const {return _names;}

string Actor::getName() const {
  if (_names.size() > 0) {
    return _names[0];
  } else {
    return "";
  }
};
bool Actor::hasName(string s) const{
  for (auto it = _names.begin(); it != _names.end(); ++it) {
    if (strcomp(*it,s)) {
      return true;
    }
  }
  return false;
}

void Actor::setDesc(string s) {_desc = s;}
string Actor::getDesc() const {return _desc;}

bool Actor::doEvent(string s,string t) {
  auto it = events.find(s);
  bool didsmth = false;
  if(it!=events.end()) {
    std::vector<std::function<bool(string,Character*)>> e = (*it).second;
    for (auto it2 = e.begin(); it2 != e.end();++it2) {
      if ((*it2)(t,NULL)) {
	didsmth = true;
      }
    }
    return didsmth;
  } else {
    return false;
  }
}
void Actor::addEvent(string s, std::function<bool(string,Character*)> l) {
  events[s].push_back(l);
}

std::unordered_set<string> Actor::getFlags() const {return flags;}
void Actor::addFlag(string s) {flags.insert(s);}
void Actor::removeFlag(string s) {flags.erase(s);}
bool Actor::checkFlag(string s) const{
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

string Actor::getID() const {
  if (_names.size() > 1) {
    return _names[1];
  } else if (_names.size() > 0) {
    return _names[0];
  } else {
    return "";
  }
}

