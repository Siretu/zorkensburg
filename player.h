#ifndef PLAYER_GUARD
#define PLAYER_GUARD

#include "character.h"

class Parser;

using std::string;

class Player : public Character {
 protected:
  Parser* p;
 public:
  static const int C_USED_WORDS;

  Player(string s,Game* g,Room* r);

  ~Player();
  
  void death();
  bool action();

  bool drop(string s);
  bool go(string s);
  bool attack(string s);
  bool pick_up(string s);
  bool look(string s);
  bool examine(string s);
  bool inventory(string s);

  string serialize() const;
};


#endif
