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
  string result = "";
  for (auto it = rooms.begin(); it != rooms.end(); ++it) {
    result += (*it)->serialize();
  }
  for (auto it = doors.begin(); it != doors.end(); ++it) {
    result += (*it)->serialize();
  }
  result += events;
  return result;
}

Game::~Game(){
  cerr << "auto saving on close" << endl;
  save("autosave");
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

void Game::clearold(){
  events = "";
  for(auto it = rooms.begin(); it != rooms.end(); ++it) {
    delete *it;
  }
  for(auto it = doors.begin(); it != doors.end(); ++it) {
    delete *it;
  }
  rooms.clear();
  doors.clear();
  actors.clear();
  
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
  string line;
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
      }
    }
    loadfile.close();
  }
  push(player->getLocation()->getDescription());
}

bool Game::makeEvent(string s){
  std::vector<string> content = split(s,';');
  /*int i1 = s.find_first_of(";");
  int i2 = s.find_first_of(";", i1 + 1);
  int i3 = s.find_first_of(";", i2 + 1);
  int i4 = s.find_first_of(";", i3 + 1);

  string cmd = s.substr(0,i1);
  string operand = s.substr(i1 + 1, i2 - i1 - 1);
  string target = s.substr(i2 + 1, i3 - i2 - 1);
  string cond = s.substr(i3 + 1, i4 - i3 - 1);*/
  string cmd = content[0];
  string operand = content[1];
  string target = content[2];
  string cond = content[3];
  cerr<< "Found event: " << cmd << " | " << operand << " | " << target << " | " << cond << endl;

  //string cond = interval(s, i3 + 1, i4);
  Actor* op = NULL;
  Actor* tg = NULL;
   
  // Link string to actors
  for(auto it = actors.begin(); it != actors.end(); ++it) {
    if ((*it)->id == operand) {
      op = *it;
    }
    if ((*it)->id == target) {
      tg = *it;
    }
  }
  if (op == NULL || tg == NULL) {
    cerr <<"Invalid event: " << s << " . Couldn't find " << operand << " or " << target << endl;
    return false;
  }

  std::vector<string> actions (content.begin()+4,content.end());

  for (auto it = actions.begin(); it != actions.end(); ++it) {
    std::vector<string> arguments = split(*it,'#');
    string action = arguments[0];
    string message = arguments[1];
    std::function<bool(string)> a = NULL;
    Game* g = this;
    if (action == "LOCKMESSAGE") {
      a = [tg,message,g,cond](string s) -> bool {
	if (tg->checkFlag(cond)) {
	  Door* target = dynamic_cast<Door*>(tg);
	  target->setLockedMessage(message);
	  return true;
	}
	return false;
      };
    } else if (action == "SET") {
      a = [tg,message,cond](string s) -> bool {
	if (tg->checkFlag(cond)) {
	  tg->addFlag(message);
	  return true;
	}
	return false;
      };
    } else if (action == "MESSAGE") {
      a = [tg,message,g,cond](string s) -> bool {
	if (tg->checkFlag(cond)) {
	  g->push(message);
	  return true;
	}
	return false;
      };
    } else if(action == "UNSET"){
      a = [tg,message,cond](string s) -> bool {
	if (tg->checkFlag(cond)) {
	  tg->removeFlag(message);
	  return true;
	}
	return false;
      };
    } else if(action == "FAILMESSAGE") {
      a = [tg,message,cond,g](string s) -> bool {
	if (!tg->checkFlag(cond)) {
	  g->push(message);
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
