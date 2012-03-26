#include "Net.h"
#include "util.h"

/*constructor*/
Net::Net (string name, int id, int pinNum) :
  name(name),
  id(id),
  pinNum(pinNum)
{
}
	
/*Public functions*/
void Net::addPin(point3d pin, int llx, int lly, int tWidth, int tHeight)
{
  pPins.push_back(pin);
  gPins.push_back(ptog(pin, llx, lly, tWidth, tHeight));
}

void Net::printInput()
{
  printf("%s %d %d x\n", name.c_str(), id, pinNum);
  for (int i = 0; i < pPins.size(); i++)
    printf("%d %d %d\n", pPins[i].x, pPins[i].y, pPins[i].z);
  for (int i = 0; i < gPins.size(); i++)
    printf("%d %d %d\n", gPins[i].x, gPins[i].y, gPins[i].z);
}

void Net::addRoute(route r)
{
  routes.push_back(r);
}

vector<point3d> &Net::getGPins()
{
  return gPins;
}
