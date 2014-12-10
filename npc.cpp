#include <string>
#include "npc.h"

using std::string;

const int NPC::C_USED_WORDS = Character::C_USED_WORDS;

string NPC::serialize() const {
  string result = "NPC:";
  result += Character::serialize();
  return result;
}
