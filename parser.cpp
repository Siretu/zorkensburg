#include "parser.h"
#include "player.h"
#include <string>
//#include <strings.h>
#include <locale>
#include <iostream>
#include "game.h"
#include "room.h"

using std::string;
using std::locale;

Parser::Parser(Player* player, Game* game) : g(game), p(player){
  verbs["drop"] = &Player::drop;
  verbs["go"] = &Player::go;
  verbs["attack"] = &Player::attack;
  verbs["look"] = &Player::look;
  verbs["inventory"] = &Player::inventory;
  verbs["examine"] = &Player::examine;
  verbs["pickup"] = &Player::pick_up;
  verbs["pick"] = &Player::pick_up;
  verbs["take"] = &Player::pick_up;
  verbs["drop"] = &Player::drop;
}

bool Parser::parse(std::string s) {
  std::locale loc;
  
  int space = s.find_first_of(" ");
  std::string command = s.substr(0,space);
  //command: to lowercase
  for (string::size_type i=0; i<command.length(); ++i){
    command[i] = std::tolower(command[i],loc);
  }
  std::string args = s.substr(space+1);
  
  if(strcomp(command, "quit")){
    g->doQuit();
    return true;
  }
  if(strcomp(command, "save")) {
    g->save(args);
    return true;
  }
  if(strcomp(command, "load")) {
    g->load(args);
    //g = new Game(args);
    return true;
  }

  auto it = verbs.find(command);
  if (it != verbs.end()) {
    (p->*verbs[command])(args);
    return true;
  } else if (command.find_first_of('#') == -1){
    int space2 = args.find_first_of(" ");
    std::string target = args.substr(0,space2);
    finder f = (p->getLocation())->find(target,0);
    while(f.findNext() != NULL) {
      if((*f)->doEvent(command,args)) {
	return true;
      }
    }
  }
  if (g->bufferEmpty()) {
    g->push("I don't understand that.");
  }
  return false;

}
