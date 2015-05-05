#ifndef __BARTENDER_H
#define __BARTENDER_H

void write_lcd(int selected);
void fill_glass(int sel);
void fill_glass_debug(int sel);
void init_weight(void);
int get_raw(void);
int get_weight(void);
int get_average(int n);
void fill_liquid(int valve, int target_weight);

#endif
