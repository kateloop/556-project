#include "net.h"

/*constructor*/
net::net (char *name_in, int id_in, int pinNum_in, point3d *pins_in) {
	name = name_in;
	id = id_in;
	pinNum = pinNum_in;
	pins = pins_in;
}
	
/*Public functions*/
void add_route (route route) {
	routes[routeNum] = route;
	routeNum++;	
}

