#ifndef __GUI_CHART_H
#define __GUI_CHART_H

#ifdef EMBEDDED
#include "lvgl.h"
#include "imu.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "gui_widgets.h"

void gui_create_chart(lv_obj_t *parent);

#endif /* __GUI_CHART_H */