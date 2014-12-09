#include <string>
#include "npc.h"

using std::string;

string NPC::serialize() const {
  string result = "NPC:";
  result += Character::serialize();
  return result;
}
