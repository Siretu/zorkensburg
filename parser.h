#ifndef PARSER_GUARD
#define PARSER_GUARD

#include <string>
#include <unordered_map>

class Game; // Forward Declaration
class Player; // Forward Declaration
class Parser {
 protected:
  Game* g;
  Player* p;
  typedef bool (Player::*func) (std::string);
  std::unordered_map<std::string,func> verbs;
 public:
  Parser(Player* player, Game* g);
  
  bool parse(std::string s);

};


#endif
