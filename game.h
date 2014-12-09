#ifndef GAME_GUARD
#define GAME_GUARD
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>

class Player; // Forward Declaration
class Actor; // Forward Declaration
class Room; // Forward Declaration
class Door; // Forward Declaration

using std::string;
using std::vector;
using std::unordered_set;

class Game {
 protected:
  bool makeEvent(string s);
  
  string _status;
  std::string buffer;
  std::vector<Actor*> actors;
  std::vector<Room*> rooms;
  std::unordered_set<Door*> doors;
  string events = "";
 public:

  Player* player;

  Game();
  Game(string filename);
  ~Game();
  std::string flush() {
    std::string temp = buffer;
    buffer = "";
    return temp;
  }
  
  void push(std::string output) {
    buffer += output;
    buffer += "\n";
  }

  void doQuit(){_status="quit";}
  void setStatus(string s) {_status = s;}
  string status() const{return _status;}
  
  bool bufferEmpty() const {return (buffer == "");}
  
  std::vector<Actor*> getActors() {
    return actors;
  }

  void addActor(Actor* a) {
    actors.push_back(a);
  }

  void addRoom(Room* r) {rooms.push_back(r);
  }

  void addDoor(Door* d) {doors.insert(d);}
  void removeDoor(Door* d) {doors.erase(d);}
  std::unordered_set<Door*> getDoors() {return doors;}


  string serialize() const;
  
  void clearold();
  void save(string s);
  void load(string s);
};

string interval(string s, int fr, int to);
vector<string> split(string s, char on) ;
string join(vector<string> s, char on) ;
string join(unordered_set<string> s, char on) ;

bool strcomp(string a, string b);
bool confirm(string s);
#endif
