#include <string>
#include "item.h"
#include "game.h"
#include <iostream>


using std::cerr;
using std::endl;
using std::string;

Item::Item(Game* game, string data) : Actor(game,data){
  auto d = split(data,';');
  hidden = (d[3] == "1");
}


string Item::print(int indent) const{
  return getName();
}

string Item::serialize() const{
  string result = "ITEM:";
  result += Actor::serialize();
  result += ";";
  result += hidden ? "1" : "0";
  return result;
}
