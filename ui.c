#include "ui.h"

/* Screen object declarations */
static lv_obj_t *ui_splash_screen;
static lv_obj_t *ui_splash_screen_image;

static lv_obj_t *ui_sakura_tree_1_screen;
static lv_obj_t *ui_sakura_tree_1_image;
static lv_obj_t *ui_sakura_tree_1_circle;

static lv_obj_t *ui_sakura_tree_2_screen;
static lv_obj_t *ui_sakura_tree_2_image;
static lv_obj_t *ui_sakura_tree_2_circle;

static lv_obj_t *ui_sakura_tree_3_screen;
static lv_obj_t *ui_sakura_tree_3_image;
static lv_obj_t *ui_sakura_tree_3_circle;

/* Constants */
#define SCREEN_TRANSITION_TIME 500
#define CIRCLE_PADDING 30

#define BACKGROUND_COLOR 0x6C526A
#define CIRCLE_COLOR 0xA57FA1

/* Forward declarations of static functions */
static void _ui_screen_change(lv_obj_t *target, lv_scr_load_anim_t fademode, int spd, int delay);
static void ui_background_init(lv_obj_t *screen, lv_coord_t width, lv_coord_t height);

/* Screen transition helper */
static void _ui_screen_change(lv_obj_t *target, lv_scr_load_anim_t fademode, int spd, int delay) {
    lv_scr_load_anim(target, fademode, spd, delay, false);
}

/* Event handlers */
static void ui_event_splash_screen(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE && lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP) {
        _ui_screen_change(ui_sakura_tree_1_screen, LV_SCR_LOAD_ANIM_MOVE_TOP, SCREEN_TRANSITION_TIME, 0);
    }
}

static void ui_event_sakura_screen_1(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_BOTTOM) {
            _ui_screen_change(ui_splash_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, SCREEN_TRANSITION_TIME, 0);
        } else if (dir == LV_DIR_LEFT) {
            _ui_screen_change(ui_sakura_tree_2_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_TRANSITION_TIME, 0);
        }
    }
}

static void ui_event_sakura_screen_2(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_BOTTOM) {
            _ui_screen_change(ui_splash_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, SCREEN_TRANSITION_TIME, 0);
        } else if (dir == LV_DIR_LEFT) {
            _ui_screen_change(ui_sakura_tree_3_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, SCREEN_TRANSITION_TIME, 0);
        } else if (dir == LV_DIR_RIGHT) {
            _ui_screen_change(ui_sakura_tree_1_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_TRANSITION_TIME, 0);
        }
    }
}

static void ui_event_sakura_screen_3(lv_event_t *e) {
    lv_event_code_t event_code = lv_event_get_code(e);

    if (event_code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_get_act());
        if (dir == LV_DIR_BOTTOM) {
            _ui_screen_change(ui_splash_screen, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, SCREEN_TRANSITION_TIME, 0);
        } else if (dir == LV_DIR_RIGHT) {
            _ui_screen_change(ui_sakura_tree_2_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, SCREEN_TRANSITION_TIME, 0);
        }
    }
}

static void ui_background_init(lv_obj_t *screen, lv_coord_t width, lv_coord_t height) {
    /* Set screen background color */
    lv_obj_set_style_bg_color(screen, lv_color_hex(BACKGROUND_COLOR), LV_PART_MAIN);
    
    /* Create and configure circle background */
    lv_obj_t *circle = lv_obj_create(screen);
    lv_coord_t max_dimension = (width > height) ? width : height;
    lv_coord_t circle_size = max_dimension + CIRCLE_PADDING;
    lv_obj_set_size(circle, circle_size, circle_size);
    lv_obj_align(circle, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_radius(circle, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(circle, lv_color_hex(CIRCLE_COLOR), 0);
    lv_obj_set_style_border_width(circle, 0, 0);

    /* Create three horizontal stripes over the circle */
    lv_obj_t *stripe;
    int i;
    
    // First group: 5 stripes spaced 7px apart
    for(i = -1; i < 4; i++) {
        stripe = lv_obj_create(screen);
        lv_obj_set_size(stripe, circle_size, 2);
        lv_obj_align(stripe, LV_ALIGN_CENTER, 0, i * 7);
        lv_obj_set_style_bg_color(stripe, lv_color_hex(BACKGROUND_COLOR), 0);
        lv_obj_set_style_border_width(stripe, 0, 0);
        lv_obj_clear_flag(stripe, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_move_background(stripe);
    }

    // Two closely spaced stripes
    int y_pos = i * 7; // Continue from last position
    
    stripe = lv_obj_create(screen);
    lv_obj_set_size(stripe, circle_size, 2);
    lv_obj_align(stripe, LV_ALIGN_CENTER, 0, y_pos + 4);
    lv_obj_set_style_bg_color(stripe, lv_color_hex(BACKGROUND_COLOR), 0);
    lv_obj_set_style_border_width(stripe, 0, 0);
    lv_obj_clear_flag(stripe, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_background(stripe);

    stripe = lv_obj_create(screen);
    lv_obj_set_size(stripe, circle_size, 2);
    lv_obj_align(stripe, LV_ALIGN_CENTER, 0, y_pos + 8);
    lv_obj_set_style_bg_color(stripe, lv_color_hex(BACKGROUND_COLOR), 0);
    lv_obj_set_style_border_width(stripe, 0, 0);
    lv_obj_clear_flag(stripe, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_background(stripe);

    // Thicker stripe
    stripe = lv_obj_create(screen);
    lv_obj_set_size(stripe, circle_size, 10);
    lv_obj_align(stripe, LV_ALIGN_CENTER, 0, y_pos + 16);
    lv_obj_set_style_bg_color(stripe, lv_color_hex(BACKGROUND_COLOR), 0);
    lv_obj_set_style_border_width(stripe, 0, 0);
    lv_obj_clear_flag(stripe, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_background(stripe);

    // Final stripe to fill remaining space
    stripe = lv_obj_create(screen);
    // Calculate remaining space: half circle size minus current y position minus some padding
    lv_coord_t remaining_height = (circle_size / 2) - (y_pos + 26);
    lv_obj_set_size(stripe, circle_size, remaining_height);
    lv_obj_align(stripe, LV_ALIGN_CENTER, 0, y_pos + 26 + (remaining_height / 2));
    lv_obj_set_style_bg_color(stripe, lv_color_hex(BACKGROUND_COLOR), 0);
    lv_obj_set_style_border_width(stripe, 0, 0);
    lv_obj_clear_flag(stripe, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_move_background(stripe);

    // Finally move the circle to the background
    lv_obj_move_background(circle);
}

/* Screen initialization functions */
static void ui_splash_screen_init(void) {
    ui_splash_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_splash_screen, LV_OBJ_FLAG_SCROLLABLE);

    ui_splash_screen_image = lv_img_create(ui_splash_screen);
    lv_img_set_src(ui_splash_screen_image, &loading_screen);
    lv_obj_set_width(ui_splash_screen_image, 240);
    lv_obj_set_height(ui_splash_screen_image, 240);
    lv_obj_set_align(ui_splash_screen_image, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_splash_screen, ui_event_splash_screen, LV_EVENT_ALL, NULL);
}

static void ui_sakura_tree_screen_1_init(void) {
    ui_sakura_tree_1_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_sakura_tree_1_screen, LV_OBJ_FLAG_SCROLLABLE);

    ui_sakura_tree_1_image = lv_img_create(ui_sakura_tree_1_screen);
    lv_img_set_src(ui_sakura_tree_1_image, &Sakura_Tree_1);
    lv_obj_set_width(ui_sakura_tree_1_image, 144);
    lv_obj_set_height(ui_sakura_tree_1_image, 144);
    lv_obj_set_align(ui_sakura_tree_1_image, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_sakura_tree_1_screen, ui_event_sakura_screen_1, LV_EVENT_ALL, NULL);
    ui_background_init(ui_sakura_tree_1_screen, 144, 144);
}

static void ui_sakura_tree_screen_2_init(void) {
    ui_sakura_tree_2_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_sakura_tree_2_screen, LV_OBJ_FLAG_SCROLLABLE);

    ui_sakura_tree_2_image = lv_img_create(ui_sakura_tree_2_screen);
    lv_img_set_src(ui_sakura_tree_2_image, &Sakura_Tree_2);
    lv_obj_set_width(ui_sakura_tree_2_image, 128);
    lv_obj_set_height(ui_sakura_tree_2_image, 170);
    lv_obj_set_align(ui_sakura_tree_2_image, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_sakura_tree_2_screen, ui_event_sakura_screen_2, LV_EVENT_ALL, NULL);
    ui_background_init(ui_sakura_tree_2_screen, 128, 170);
}

static void ui_sakura_tree_screen_3_init(void) {
    ui_sakura_tree_3_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_sakura_tree_3_screen, LV_OBJ_FLAG_SCROLLABLE);

    ui_sakura_tree_3_image = lv_img_create(ui_sakura_tree_3_screen);
    lv_img_set_src(ui_sakura_tree_3_image, &Sakura_Tree_3);
    lv_obj_set_width(ui_sakura_tree_3_image, 176);
    lv_obj_set_height(ui_sakura_tree_3_image, 220);
    lv_obj_set_align(ui_sakura_tree_3_image, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_sakura_tree_3_screen, ui_event_sakura_screen_3, LV_EVENT_ALL, NULL);
    ui_background_init(ui_sakura_tree_3_screen, 176, 190);
}

/* Public initialization function */
void init_ui(void) {
    ui_splash_screen_init();
    ui_sakura_tree_screen_1_init();
    ui_sakura_tree_screen_2_init();
    ui_sakura_tree_screen_3_init();
    lv_disp_load_scr(ui_sakura_tree_1_screen);
}