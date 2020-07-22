#include "gui_app.h"

static gui_app_st gui;

static void gui_create_group_focus(void);
static void gui_create_imu_chart(lv_obj_t *parent);
static void gui_create_tabview_screen(void);
static void gui_add_obj_to_group(void);

static void focus_callback(lv_group_t *g);

void gui_app(void)
{
#ifdef EMBEDDED
    gui_create_group_focus();
    gui_define_input_device(gui.group);
#endif

    gui_create_tabview_screen();
    gui_create_gps_labels(gui.tab_gps);
    gui_create_gauge(gui.tab_gauge);
    gui_create_imu_chart(gui.tab_imu);
    
#ifdef EMBEDDED
    gui_add_obj_to_group();
#endif
}

static void gui_create_tabview_screen(void)
{
    gui.tabview = lv_tabview_create(lv_scr_act(), NULL);
    gui.tab_gps = lv_tabview_add_tab(gui.tabview, " GPS");
    gui.tab_gauge = lv_tabview_add_tab(gui.tabview, " GAUGE");
    gui.tab_imu = lv_tabview_add_tab(gui.tabview, " IMU");
}

static void gui_add_obj_to_group(void)
{
    lv_group_add_obj(gui.group, gui.tabview);
}

static void gui_create_imu_chart(lv_obj_t *parent)
{
    gui_create_chart(parent);
}

static void gui_create_group_focus()
{
    gui.group = lv_group_create();
    lv_group_set_focus_cb(gui.group, focus_callback);
}

static void focus_callback(lv_group_t *group)
{
    lv_obj_t *obj = lv_group_get_focused(group);
    if (obj != gui.tabview) {
        uint16_t tab = lv_tabview_get_tab_act(gui.tabview);
        switch (tab) {
        case 0:
            lv_page_focus(gui.tab_gps, obj, LV_ANIM_ON);
            break;
        case 1:
            lv_page_focus(gui.tab_gauge, obj, LV_ANIM_ON);
            break;
        case 2:
            lv_page_focus(gui.tab_imu, obj, LV_ANIM_ON);
            break;
        }
    }
}