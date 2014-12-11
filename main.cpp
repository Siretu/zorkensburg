#include <string>
#include <iostream>
#include <vector>
#include "parser.h"
#include "game.h"
#include "actor.h"
#include "player.h"
#include "room.h"
#include "door.h"

using std::cin;
using std::cout;
using std::endl;

int main() {

  Game g("worldfile");
  
  cout << "#############" << endl;
  cout << g.serialize() << endl;
  cout << "#############" << endl;
  
  while (g.status() == "") {
    for(int i = 0; i < g.getActors().size();i++) { //TODO: Use .nextActor instead.
      Actor* curr = g.getActors()[i];
      curr->action();
    }
  }
  cout << g.flush() << endl;
}
