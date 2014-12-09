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
  
  
  /*Room* r1 = new Room("Living room","You're standing in a nice living room.");
  Room* r2 = new Room("Kitchen","You're standing in an ugly kitchen.");
 
  g.addActor(new Player(&g, r1));
  
  Door d (r1,r2,"w","e",&g);*/
  //g.push(g.WELCOME_MESSAGE); ---- statics?
  while (g.status() == "") {
    for(int i = 0; i < g.getActors().size();i++) { //TODO: Use .nextActor instead.
      Actor* curr = g.getActors()[i];
      curr->action();
    }
  }
}
