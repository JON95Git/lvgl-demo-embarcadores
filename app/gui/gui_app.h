#ifndef __GUI_APP_H
#define __GUI_APP_H

#ifdef EMBEDDED
#include "lvgl.h"
#include "gui_chart.h"
#include "gui_gauge.h"
#include "gui_gps.h"
#include "gui_hardware_interface.h"
#else
#include "lvgl/lvgl.h"
#include "components/gui_chart.h"
#include "components/gui_gauge.h"
#include "components/gui_gps.h"
#endif
#include "gui_widgets.h"

typedef struct {
    lv_group_t *group;
    lv_obj_t *tabview;
    lv_obj_t *tab_gps;
    lv_obj_t *tab_gauge;
    lv_obj_t *tab_imu;
} gui_app_st;

#define CHART_POINTS_NUMBER 10

void gui_app(void);

#endif /* __GUI_APP_H */