#ifndef __GUI_GPS_H
#define __GUI_GPS_H

#ifdef EMBEDDED
#include "lvgl.h"
#include "gps.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "gui_widgets.h"

typedef struct {
    float latitude;
    float longitude;
    int32_t altitude;
    int32_t tracked_satellites;
    
    char latitude_string[15];
    char longitude_string[15];
    char altitude_string[15];
    char tracked_satellite_string[25];
} _gui_aux_labels;


void gui_create_gps_labels(lv_obj_t *parent);

#endif /* __GUI_GPS_H */