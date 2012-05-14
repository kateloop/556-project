#include "Net.h"
#include "util.h"
#include <algorithm>

/*constructor*/
Net::Net (string name, int id, int pinNum, int llx, int lly, int tWidth, int tHeight) :
  name(name),
  id(id),
  pinNum(pinNum),
  llx(llx),
  lly(lly),
  tWidth(tWidth),
  tHeight(tHeight),
  ofl(0)
{
}
	
/*Public functions*/
void Net::addPin(point3d pin)
{
  pPins.push_back(pin);
  gPins.push_back(ptog(pin, llx, lly, tWidth, tHeight));
}

void Net::printRoute(FILE *of)
{
  fprintf(of, "%s %d %d\n", name.c_str(), id, r.size());
  
  for (int i = 0; i < r.size(); i++) {
    edge e = r[i];
    point3d p1 = gtop(e.first, llx, lly, tWidth, tHeight);
    point3d p2 = gtop(e.second, llx, lly, tWidth, tHeight);
    fprintf(of, "(%d,%d,%d)-(%d,%d,%d)\n", p1.x, p1.y, p1.z,
	   p2.x, p2.y, p2.z);
  }
  fprintf(of, "!\n");
}

void Net::printInput()
{
  printf("%s %d %d x\n", name.c_str(), id, pinNum);
  for (int i = 0; i < pPins.size(); i++)
    printf("%d %d %d\n", pPins[i].x, pPins[i].y, pPins[i].z);
  for (int i = 0; i < gPins.size(); i++)
    printf("%d %d %d\n", gPins[i].x, gPins[i].y, gPins[i].z);
}

void Net::setRoute(route r)
{
  this->r = r;
}

vector<point3d> &Net::getGPins()
{
  return gPins;
}

string Net::getName()
{
  return name;
}

route Net::getRoute()
{
  return r;
}

bool pinSortCompare (point3d p1, point3d p2) {
   if (p1.x < p2.x) {
      return true;
   } else if (p2.x < p1.x) {
      return false;
   } else {
      // p2.x == p1.x
      if (p1.y < p2.y) {
         return true;
      } else {
         return false;
      }
   } 
}

void Net::reorderPins() {
   sort (gPins.begin(), gPins.end(), pinSortCompare);  	
}

void Net::setOfl(int ofl) {
  this->ofl = ofl;
}

int Net::getOfl() {
  return ofl;
}

