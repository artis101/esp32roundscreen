#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

LV_IMG_DECLARE(loading_screen); // assets/falloutboy.png
LV_IMG_DECLARE(Sakura_Tree_1); // assets/Sakura_Tree_1.png
LV_IMG_DECLARE(Sakura_Tree_2); // assets/Sakura_Tree_2.png
LV_IMG_DECLARE(Sakura_Tree_3); // assets/Sakura_Tree_3.png

void ui_loading_screen_init();
void init_ui();

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
