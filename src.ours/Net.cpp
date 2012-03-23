#include "Net.h"

/*constructor*/
Net::Net (string name, int id, int pinNum) :
  name(name),
  id(id),
  pinNum(pinNum)
{
}
	
/*Public functions*/
void Net::addPin(point3d pin)
{
  pins.push_back(pin);
}

void Net::printInput()
{
  printf("%s %d %d x\n", name.c_str(), id, pinNum);
  for (int i = 0; i < pins.size(); i++)
    printf("%d %d %d\n", pins[i].x, pins[i].y, pins[i].z);
}

