#ifndef CONTAINER_GUARD
#define CONTAINER_GUARD
#include <unordered_set>
#include <string>

#include "item.h"

class Container : public Item {
 protected:
  std::unordered_set<Item*> items;

 public:
  static const int C_USED_WORDS;
  
  Container(Game* g, std::string s);

  Container(Game* g) : Container(g,"Container;;Default Container;0"){};
  
  ~Container();

  std::unordered_set<Item*> getItems() const {return items;}
  void addItem(Item* a) {items.insert(a);}
  void removeItem(Item* a) {items.erase(a);}

  std::string getName() const {return _names[0] + " (Container)";}
  
  virtual std::string print(int indent) const;

  std::string serialize() const;

  std::unordered_set<Actor*> getContained() const;

};


#endif
