#ifndef ITEM_GUARD
#define ITEM_GUARD
#include <string>
#include "actor.h"

class Item : public Actor {
 protected:
 public:
  Item(Game* g, std::string data);
  
  bool action(){}
  std::string serialize() const;

  virtual std::string print(int indent) const;
  
};


#endif
