#include "../modelLoader.h"
class Ientity {
public:
  virtual void Tick() = 0;

  virtual ~Ientity() = default;
};
