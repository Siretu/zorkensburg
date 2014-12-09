#include <string>
#include <iostream>
#include "room.h"
#include "actor.h"
#include "game.h"
#include "item.h"
#include "door.h"

using std::string;

Room::Room(string data, Game* game) {
  auto d = split(data,';');
  name = d[0];
  description = d[1];
  dark = (d[2] == "1");

  g = game;
}

Room::~Room() {
  std::cerr << "Destroyed room: " << name << std::endl;
}

Room::Room(string n, string d) : name(n), description(d) {}

bool Room::addExit(string s, Door* d) {
  exits[s] = d;
  return true;
}

Door* Room::getExit(string s){
  std::unordered_map<string,string> short_names;
  short_names["west"] = "w";
  short_names["east"] = "e";
  short_names["north"] = "n";
  short_names["south"] = "s";
  short_names["up"] = "u";
  short_names["down"] = "d";
  auto it2 = short_names.find(s);
  if (it2 != short_names.end()) {
    s = short_names[s];
  }
  auto it = exits.find(s);
  
  if(it!=exits.end()) {
    return exits[s];
  }
  return NULL;
}

string Room::serialize() const {
  string result = "ROOM:";
  result += name;
  result += ";";
  result += description;
  result += ";";
  result += dark ? "1" : "0";

  for (auto it = actors.begin(); it != actors.end(); ++it) {
    result += "\n";
    result += (*it)->serialize();
  }
  for (auto it = items.begin(); it != items.end(); ++it) {
    result += "\n";
    result += (*it)->serialize();
  }
  return result;
}

string Room::getDescription() const{
  if (isDark()) {
    return "It is pitch black. You are likely to be eaten by a grue.";
  }
  string result = "";
  result = description;
  std::vector<Item*> visibleItems;
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (!(*it)->hidden) {
      visibleItems.push_back(*it);
    }
  }
  if (!visibleItems.empty()) {
    result += "\nItems: ";
    for (auto it = visibleItems.begin(); it != visibleItems.end(); ++it) {
      result += "\n* " + (*it)->print(1);
    }
  }
  return result;
}

finder Room::find(string s, int type, Actor* start) const{
  finder f;
  f.target = s;
  f.type = type;
  f.current = NULL;
  if (start == NULL) {
    for(auto it = actors.begin(); it != actors.end(); ++it){
      f.bfsq.push_back(*it);
      f.trackback[*it] = NULL;
    }
    if (f.type != 1) {
      std::unordered_set<Item*> items = getItems();
      for(auto it = items.begin(); it != items.end(); ++it){
	if ((*it)->hidden && f.type != 0) continue;
	f.bfsq.push_back(*it);
	f.trackback[*it] = NULL;
      }
    }
    if (f.type == 0) {
      for(auto it = exits.begin();it != exits.end(); ++it) {
	f.bfsq.push_back((*it).second);
	f.trackback[(*it).second] = NULL;
      }
    }
  } else {
    f.bfsq.push_back(start);
    f.trackback[start] = NULL;
  }
  return f;
}
/*
bool strcomp(string a, string b){
  if(a.length() != b.length()) return false;
  if(a == b) return true;
  if(strcasecmp(a.c_str(), b.c_str()) == 0) return true; //UNIX/POSIX SPECIFIC - on win, use stricmp
  return false;
}
*/
Actor* finder::findNext(){
  while (!bfsq.empty()) {
    current = bfsq.front();
    bfsq.pop_front();
    if(visited.count(current)) continue;
    if(type != 1) {
      auto temp = current->getContained();
      for(auto it = temp->begin();it != temp->end();++it){
	if ((*it)->hidden /*&& type != 0*/) continue;
	bfsq.push_back(*it);
	trackback[*it] = current;
      }
    }
    visited.insert(current);
    Item* a;
    if(type == 2) {
      a = dynamic_cast<Item*>(current);
    }
    if(target == "" || (current->hasName(target) && (type < 2 || a != NULL)) ){
      return current;
    }
  }
  return NULL;
}

