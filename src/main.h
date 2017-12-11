#pragma once
#include "vmath/ray.h"

//functions
const char *find_data_file(const char *fname);
void disp();
void render();
int glutMainCompose();
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);
void keyb_up(unsigned char key, int x, int y);
void mouse(int bn, int state, int x, int y);
void motion(int x, int y);
void passive_motion(int x, int y);
void double_click(int x, int y);
unsigned int load_texture(const char *fname);
int parse_args(int argc, char **argv);
Ray calc_mouse_ray(int x, int y);
