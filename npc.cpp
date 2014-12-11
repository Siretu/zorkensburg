#include <string>
#include "npc.h"
#include "room.h"

using std::string;

const int NPC::C_USED_WORDS = Character::C_USED_WORDS;

string NPC::serialize() const {
  string result = "NPC:";
  result += Character::serialize();
  return result;
}

void NPC::death() {
  g->push("The " + getName() + " dies and instantly turns to dust. Leaving no treasure behind");
  location->removeActor(this);
  g->removeActor(this);
}
