#include <string>
#include <iostream>
#include <unordered_set>
#include "container.h"
#include "item.h"

using std::string;
using std::cerr;
using std::endl;

string Container::print(int indent) const {
  string result = getName();
  if (!items.empty()) {
    result += " containing: {";
    for(auto it = items.begin();it != items.end();++it) {
      result += "\n ";
      for(int i = 0; i < indent; i++) {
	result += "  ";
      }
      result += "- ";
      result += (*it)->print(indent + 1);
    }
    result += "\n ";
    for(int i = 0; i < indent; i++) {
      result += "  ";
    }
    result += "}";
  } else {
    result += " (empty)";
  }
  return result;
}

const int Container::C_USED_WORDS = Item::C_USED_WORDS;
Container::Container(Game* g, std::string s) : Item(g,s){
    
}


string Container::serialize() const{
  string result = "CONTAINER:";
  string temp = Item::serialize();
  temp.erase(0,5);
  result += temp;
  result += "\n";

  for (auto it = items.begin(); it != items.end();++it) {
    result += (*it)->serialize();
    result += "\n";
  }

  result += "ENDCONT";
  return result;
}

std::unordered_set<Actor*>* Container::getContained() const {
  std::unordered_set<Actor*>* result = new std::unordered_set<Actor*>;
  for (auto it = items.begin();it != items.end();++it){
    result->insert(*it);
  }
  return result;
}
