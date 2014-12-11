#include <string>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "player.h"
#include "parser.h"
#include "game.h"
#include "door.h"
#include "room.h"
#include "item.h"
#include "container.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;


Player::Player(string data, Game* instance, Room* r) : Character(instance,r,data) {
  p = new Parser(this,instance);
}

Player::~Player(){
  delete p;
}

// TODO: Implement death
void Player::death() {
  g->push("You die.");
  g->doQuit();
}

bool Player::action() {
  string input;
  do {
    cout << g->flush() << endl;
    if (cin.eof()) {
      g->doQuit();
      return false;
    }
    cout << "> ";
    std::getline(cin,input);
    removeFlag("looking");
  } while (!p->parse(input));
  return true;
}

bool Player::drop(string s) {
  finder f = location->find(s, 2, this);
  if(f.findNext() == NULL) {
    g->push("You don't have any '" + s + "'");
    return false;
  } else {
    Item* i = dynamic_cast<Item*>(*f);
    i->removeFlag("pickedup");
    if(i != NULL) {
      location->addItem(i);
      getInventory()->removeItem(i);
    }
    g->push("Dropped " + s);
  }
  return true;
}

bool Player::go(string args) {
  Room* oldLocation = location;
  if (!enter(args)) {
    g->push("No exit for direction: " + args);
  } else if (location != oldLocation) {
    auto temp = oldLocation->getActors();
    for (auto it = temp.begin(); it != temp.end();++it){
      (*it)->removeFlag("playerishere");
    }
    temp = location->getActors();
    for (auto it = temp.begin(); it != temp.end();++it){
      (*it)->addFlag("playerishere");
    }
    if(!oldLocation->isDark() && location->isDark()) {
      g->push("You have moved into a dark place.\n\n");
    }
    addFlag("looking");
    g->push(location->getDescription());
  }
}

bool Player::attack(string args) {
  std::unordered_set<Item*> is = getInventory()->getItems();
  Item* weapon = NULL;
  for (auto it = is.begin();it != is.end();++it){
    if ((*it)->hasName("weapon")) {
      weapon = *it;
    }
  }
  if (weapon == NULL) {
    g->push("You have no weapon!");
    return false;
  } else {
    finder f = location->find(args,1);
    if(f.findNext() != NULL) {
      Character* c = dynamic_cast<Character*>(*f);
      if (c != NULL) {
	g->push("Found " + c->getName() + " to attack.");
	g->push("You attack the " +c->getName());
	c->damageHealth(5);
      }
    }
  }


  return false;
}
bool Player::pick_up(string args) {
  finder f = location->find(args, 2);
  if(f.findNext() == NULL){
    if(strcomp(args.substr(0,3), "up ")){
      return pick_up(args.substr(3));
    } else {
      g->push("Didn't find "+args+" to pick up.");
      return false;
    }
  }

  Actor* containing = f.trackback[*f];
  Item* i = dynamic_cast<Item*>(*f);
  if (i != NULL) {
    if(containing != NULL) {
      if (!containing->removeItem(i)) {
	Container* container = dynamic_cast<Container*>(containing);
	if(container != NULL) {
	  container->removeItem(i);
	}
      }
    } else {
      location->removeItem(i);
    }
    i->addFlag("pickedup");
    getInventory()->addItem(i);
  }
  g->push("Found "+(*f)->getName()+", picking up.");
  
  return true;
}

bool Player::look(string args) {
  addFlag("looking");
  g->push("You look around");
  g->push(location->getDescription());
}

bool Player::examine(string args) {
  finder f = location->find(args,0);
  if (f.findNext() != NULL) {
    g->push("You examine " + (*f)->getName() + ":\n");
    g->push((*f)->getDesc());
  } else {
    g->push("Couldn't find any "+args);
  }
  
}

bool Player::inventory(string args) {
  Container* inv = getInventory();
  if (inv == NULL || inv->getItems().size() == 0) {
    g->push("You look into your inventory and see nothing.");
  } else {
    g->push("You look into your inventory and see:");
    std::unordered_set<Item*> items = inv->getItems();
    for(auto it = items.begin();it != items.end();++it) {
      g->push("* " + (*it)->print(1));
    }
  }
}

string Player::serialize() const {
  string result = "PLAYER:";
  result += Character::serialize();

  return result;
}
