#include <string>
#include "item.h"
#include "game.h"
#include <iostream>


using std::cerr;
using std::endl;
using std::string;

Item::Item(Game* game, string data) : Item(game, "","asd"){
  int index1 = data.find_first_of(";");
  int index2 = data.find_first_of(";",index1+1);
  addNames(parseNames(data.substr(0,index1)));
  
  _desc = data.substr(index1 + 1, index2 - index1 - 1);
  if (_names.size() > 1) {
    id = _names[1];
  } else if (_names.size() > 0) {
    id = _names[0];
  } else {
    id = "";
  }
  hidden = (data.substr(index2+1) == "1");
}


string Item::print(int indent) const{
  return getName();
}

string Item::serialize() const{
  string result = "ITEM:";
  result += getNamesSerialize();
  result += ";";
  result += _desc;
  result += ";";
  result += hidden ? "1" : "0";
  result += "\n";
  return result;
}
