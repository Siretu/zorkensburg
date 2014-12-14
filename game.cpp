#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <stack>
#include "game.h"
#include "room.h"
#include "player.h"
#include "door.h"
#include "item.h"
#include "container.h"
#include <strings.h>
#include <functional>
#include "npc.h"
#include "actor.h"

typedef bool (*lambdas) (std::string);

using std::string;
using std::cerr;
using std::cout;
using std::endl;
//static string Game::WELCOME_MESSAGE = "";
Game::Game() {
  _status = "";
}


Game::Game(string filename) : Game() {
  load(filename);
}



bool strcomp(string a, string b){
  if(a.length() != b.length()) return false;
  if(a == b) return true;
  if(strcasecmp(a.c_str(), b.c_str()) == 0) return true; //UNIX/POSIX SPECIFIC - on win, use stricmp
  return false;
}


string Game::serialize() const{
  string result = "INTRODUCTION:";
  result += introduction;
  result += "\n";
  for (auto it = rooms.begin(); it != rooms.end(); ++it) {
    result += (*it)->serialize();
    result += "\n";
  }
  for (auto it = doors.begin(); it != doors.end(); ++it) {
    result += (*it)->serialize();
    result += "\n";
  }
  result += events;
  result += "\n";
  result += defines_string();
  return result;
}

Game::~Game(){
  cout << "Closing program, saving game state." << endl;
  save("autosave");
  clearold();
}

void Game::clearold(){
  events = "";
  for(auto it = rooms.begin(); it != rooms.end(); ++it) {
    delete *it;
  }
  for(auto it = doors.begin(); it != doors.end(); ++it) {
    delete *it;
  }
  //Antagande: Samtliga actors är i ett rum, eller ägda av en actor
  //i ett rum. Rummen ansvarar för att delete:a sina "ägda" actors.
  rooms.clear();
  doors.clear();
  actors.clear();  
}

void Game::save(string args) {
  if (args == "") {
    args = "save";
  }
  std::ofstream savefile;
  savefile.open(args);
  savefile << serialize();
  savefile.close();
  push("Saved as '" + args + "'");
}

void Game::load(string args) {
  // Clear potential old data
  clearold();


  if (args == "" || args == "load"){
    args = "save";
  }
  cout << "Reading from <"+args+">" << endl;
  std::ifstream loadfile;
  loadfile.open(args);
  string line = "";
  if (loadfile.is_open()) {
    Room* r;
    Character* p = NULL;
    std::stack<Container*> inv;
    std::unordered_map<string,Room*> rooms;
    while ( getline (loadfile,line) ) {
      if (line.substr(0,5) == "ROOM:") {
	r = new Room(line.substr(5),this);
	rooms[r->getName()] = r;
	addRoom(r);
      } else if (line.substr(0,7) == "PLAYER:") {
	player = new Player(line.substr(7),this,r);
	p = player;
	r->addActor(p);
	actors.push_back(p);
      } else if (line.substr(0,5) == "DOOR:") {
	Door* d = new Door (line.substr(5),&rooms,this);
	addDoor(d);
	actors.push_back(d);
      } else if (line.substr(0,7) == "ENDCHAR") {
	p = NULL;
      } else if (line.substr(0,5) == "ITEM:") {
	Item* i = new Item(this, line.substr(5));
	if (!inv.empty()) {
	  inv.top() -> addItem(i);
	} else if(p != NULL) {
	  p ->getInventory()->addItem(i);
	} else {
	  r -> addItem(i);
	}
	actors.push_back(i);
      } else if (line.substr(0,10) == "CONTAINER:") {
	Container* i = new Container(this, line.substr(10));
	if (!inv.empty()) {
	  inv.top() -> addItem(i);
	} else if(p != NULL) {
	  p ->getInventory()->addItem(i);
	} else {
	  r -> addItem(i);
	}
	inv.push(i);
	actors.push_back(i);
      } else if(line.substr(0,7) == "ENDCONT"){
	inv.pop();
      } else if(line.substr(0,6) == "EVENT:") {
	if(makeEvent(line.substr(6))){
	  events += line;
	  events += "\n";
	}
      } else if(line.substr(0,4) == "NPC:") {
	p = new NPC(line.substr(4),this,r);
	r->addActor(p);
	actors.push_back(p);
      } else if(line.substr(0,13) == "INTRODUCTION:") {
	introduction = line.substr(13);
	push(line.substr(13) + "\n");
      } else if(line.substr(0,7) == "DEFINE:"){
        auto spl = split(line.substr(7), ';');
	definitions[spl[0]] = spl[1];
	def_str += line;
	def_str += "\n";
      }
    }
    loadfile.close();
  }
  push(player->getLocation()->getDescription());
}

bool Game::makeEvent(string s){
  std::vector<string> content = split(s,';');
  string cmd = content[0];
  string operand = content[1];
  string target = content[2];
  string cond = content[3];

  Actor* op = NULL;
  Actor* tg = NULL;
   
  // Link string to actors
  for(auto it = actors.begin(); it != actors.end(); ++it) {
    if ((*it)->getID() == operand) {
      op = *it;
    }
    if ((*it)->getID() == target) {
      tg = *it;
    }
  }
  if (op == NULL || tg == NULL) {
    cerr <<"Invalid event: " << s << " . Couldn't find " << operand << " or " << target << endl;
    return false;
  }

  std::vector<string> actions (content.begin()+4,content.end());

  for (auto it = actions.begin(); it != actions.end(); ++it) {
    //cerr << "Got action: " << *it << endl;
    std::vector<string> arguments = split(*it,'#');
    string action = arguments[0];
    string message = arguments[1];
    std::function<bool(string,Character*)> a = NULL;
    Game* g = this;
    if (action == "LOCKMESSAGE") {
      a = [tg,message,g,cond](string s, Character* c) -> bool {
	if (tg->checkFlag(cond)) {
	  Door* target = dynamic_cast<Door*>(tg);
	  target->setLockedMessage(message);
	  return true;
	}
	return false;
      };
    } else if (action == "SET") {
      a = [tg,message,cond](string s, Character* c) -> bool {
	if (tg->checkFlag(cond)) {
	  tg->addFlag(message);
	  return true;
	}
	return false;
      };
    } else if (action == "MESSAGE") {
      a = [tg,message,g,cond](string s, Character* c) -> bool {
	if (tg->checkFlag(cond)) {
	  g->push(message);
	  return true;
	}
	return false;
      };
    } else if(action == "UNSET"){
      a = [tg,message,cond](string s, Character* c) -> bool {
	if (tg->checkFlag(cond)) {
	  tg->removeFlag(message);
	  return true;
	}
	return false;
      };
    } else if(action == "FAILMESSAGE") {
      a = [tg,message,cond,g](string s, Character* c) -> bool {
	if (!tg->checkFlag(cond)) {
	  g->push(message);
	}
	return false;
      };
    } else if(action == "FOLLOW") {
      a = [tg,cond,g](string s, Character* c) -> bool {
	if(tg->checkFlag(cond)) {
	  Character* target = dynamic_cast<Character*>(tg);
	  Room* l = target->getLocation();
	  Room* player_room = g->player->getLocation();
	  for(auto kv : l->getExits()) {
	    Room* connected_room = kv.second->getConnectedRoom(l);
	    if (kv.second->getConnectedRoom(l) == g->player->getLocation()){
	      target->enter(kv.first);
	    }
	    return true;
	  }
	}
	return false;
      };
    } else if(action == "QUIT") {
      a = [tg,cond,g](string s, Character* c) -> bool {
	if(tg->checkFlag(cond)) {
	  g->doQuit();
	}
      };
    } else if(action == "ATTACK") {
      a = [tg,cond,g](string s, Character* c) -> bool {
	if(tg->checkFlag(cond)) {
	  g->push("The " + tg->getName() + " attacks you and hits you in the side.");
	  g->player->damageHealth(35);
	}
      };
    } else if(action == "CREATEITEM") {
      auto found = s.find("CREATEITEM#");
      if (found != string::npos) {
	string itemdef = s.substr(found + 11);
	a = [tg,cond,g,itemdef](string s, Character* c) -> bool {
	  if(tg->checkFlag(cond)) {
	    Item* i = new Item(g,itemdef);
	    //Character* c = dynamic_cast<Character*>(tg);
	    g->player->getInventory()->addItem(i);
	    return true;
	  }
	  return false;
	};
	op->addEvent(cmd,a);
	break;
      }
    } else if(action == "DESTROYITEM") {
      /*a = [tg,cond,g,operand](string s, Character* c) -> bool {
	Character* foo = dynamic_cast<Character*>(tg);
	finder f = foo->getLocation()->find(operand,2);
	if (f.findNext() != NULL) {
	  cerr << "Found internal drink" << endl;
	  // This will cause problems if you destroy an item in a container carried by the PC
	  Item* i = dynamic_cast<Item*>(*f);
	  if (i != NULL) {
	    g->push("You consume the " + i->getName()); // Ugly hack
	    Character* con = dynamic_cast<Character*>(*f);
	    if (con != NULL) {
	      con->getInventory()->removeItem(i);
	    } else {
	      
	    }
	    delete i;
	    return true;
	  }
	} else {
	  cerr << "Looking in room" << endl;
	  std::unordered_set<Item*> inv = foo->getLocation()->getItems();
	  for (auto it = inv.begin();it != inv.end();++it) {
	    cerr << "Looking at " << (*it)->getName() << endl;
	    cerr << "Looking for " << operand << endl;
	    Item* i = *it;
	    if (i->hasName(operand)) {
	      cerr << "Removing " << i->getName() << " from " << foo->getLocation()->getName() << endl;
	      foo->getLocation()->removeItem(i);
	      cerr << "Done removing" << endl;
	      delete i;
	      cerr << "Done deleting" << endl;
	      return true;
	    } else {
	      cerr << "Didn't match" << endl;
	    }
	  }
	  }
	g->push("There's no " + operand + " to consume.");
	return false;
	};*/
    a = [tg,cond,g,operand](string s, Character* c) -> bool {
      Character* foo = dynamic_cast<Character*>(tg);
      finder f = foo->getLocation()->find(operand,2,foo);
      if (f.findNext() != NULL) {
	// This will cause problems if you destroy an item in a container carried by the PC
	Item* i = dynamic_cast<Item*>(*f);
	if (i != NULL) {
	  g->push("You consume the " + i->getName()); // Ugly hack
	  foo->getInventory()->removeItem(i);
	  delete i;
	  return true;
	} else {
	  return false;
	}
      }
      return false;
    };
    }
    if(a != NULL){
      op->addEvent(cmd,a);
    }
  }
  return true;
} 


string interval(string s, int fr, int to){
  return s.substr(fr, to-fr);
}
std::vector<string> split(string n, char f) {
  std::vector<string> result;
  if (n != "") {
    int index = -1;
    int prevIndex = -1;
    string nextName = n;
    do {
      index = nextName.find_first_of(f, index + 1);
      string foo;
      if (index != -1) {
	foo = nextName.substr(prevIndex + 1,index - prevIndex - 1);
      }else{
	foo = nextName.substr(prevIndex + 1);
      }
      result.push_back(foo);
      prevIndex = index;
    } while(index != -1);
  }
  return result;
}

void Game::removeActor(Actor* a){
  auto it = std::find(actors.begin(),actors.end(),a);
  if (it != actors.end()) {
    actors.erase(it);
    delete a;
  }
}


string join(std::vector<string> s, char on) {
  string result = "";
  if (s.size() > 0) {
    for (auto it = s.begin();it != s.end();++it) {
      result += on;
      result += *it;
    }
    result.erase(0,1);
  }
  return result;
}

string join(std::unordered_set<string> s, char on)  {
  std::vector<string> v (s.begin(), s.end());
  return join(v,on);
}
