#ifndef __VALVES_H
#define __VALVES_H

bool get_valve_status(int id);
void set_valve_status(int id, bool status);
void init_valves(void);

#endif
