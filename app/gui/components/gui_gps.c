#include "gui_gps.h"

static gui_gps_screen_st gps_screen;
static _gui_aux_labels aux_labels;

static void update_gps_labels_callback(lv_task_t *task);
static void _gui_convert_gps_data_to_string(_gui_aux_labels *aux_labels);
static void _gui_set_gps_text_labels(_gui_aux_labels *aux_labels);
static void _gui_init_aux_strings (_gui_aux_labels *aux_labels);
static void _gui_create_gps_style(void);
static void _gui_prepare_gps_labels(lv_obj_t *parent);
static void _gui_create_gps_labels(void);
static void _gui_add_style_to_gps_labels(void);
static void _gui_create_gps_task(void);

#ifndef EMBEDDED
static void  gps_get_parsed_data_simulator(float *latitude, float *longitude, int32_t *altitude, int32_t *tracked_satellites);
#endif

void gui_create_gps_labels(lv_obj_t *parent)
{
    _gui_create_gps_style();
    _gui_prepare_gps_labels(parent);
    _gui_create_gps_labels();
    _gui_add_style_to_gps_labels();
    _gui_create_gps_task();
}

static void _gui_create_gps_style(void)
{
    gui_create_style(&gps_screen.gps_label_style, &lv_font_montserrat_18);
}

static void _gui_prepare_gps_labels(lv_obj_t *parent)
{
    gps_screen.altitude_label = (_gui_label_st){.parent = parent, 
                                .text = "Altitude: 0", 
                                .align = LV_ALIGN_CENTER, 
                                .x_ofs = -10, 
                                .y_ofs = -60};

    gps_screen.tracked_satellites_label = (_gui_label_st){.parent = parent, 
                                .text = "Tracked satellite: 0", 
                                .align = LV_ALIGN_CENTER, 
                                .x_ofs = -15, 
                                .y_ofs = -30};

    gps_screen.latitude_label = (_gui_label_st){.parent = parent, 
                                .text = "Latitude: 0", 
                                .align = LV_ALIGN_CENTER, 
                                .x_ofs = -10, 
                                .y_ofs = 0};

    gps_screen.longitude_label = (_gui_label_st){.parent = parent, 
                                .text = "Longitude: 0", 
                                .align = LV_ALIGN_CENTER, 
                                .x_ofs = -10, 
                                .y_ofs = 30};
}

static void _gui_add_style_to_gps_labels(void)
{
    gui_add_style_to_obj(gps_screen.altitude_label.label, &gps_screen.gps_label_style);
    gui_add_style_to_obj(gps_screen.tracked_satellites_label.label, &gps_screen.gps_label_style);
    gui_add_style_to_obj(gps_screen.latitude_label.label, &gps_screen.gps_label_style);
    gui_add_style_to_obj(gps_screen.longitude_label.label, &gps_screen.gps_label_style);
}

static void _gui_create_gps_labels(void)
{
    gps_screen.altitude_label.label = gui_create_label(&gps_screen.altitude_label);
    gps_screen.tracked_satellites_label.label  = gui_create_label(&gps_screen.tracked_satellites_label);
    gps_screen.latitude_label.label  = gui_create_label(&gps_screen.latitude_label);
    gps_screen.longitude_label.label  = gui_create_label(&gps_screen.longitude_label);
}


static void _gui_create_gps_task(void)
{
    gps_screen.gps_task = (_gui_task_st){ .task_xcb = update_gps_labels_callback, 
                                .period = 1000, 
                                .priority = LV_TASK_PRIO_LOWEST,
                                .parameter = NULL};
    gui_create_task(&gps_screen.gps_task);
}

static void update_gps_labels_callback(lv_task_t *task)
{
    _gui_init_aux_strings(&aux_labels);

#ifdef EMBEDDED
    gps_get_parsed_data(&aux_labels.latitude, &aux_labels.longitude, 
                        &aux_labels.altitude, &aux_labels.tracked_satellites);
#else 
    gps_get_parsed_data_simulator(&aux_labels.latitude, &aux_labels.longitude, 
                        &aux_labels.altitude, &aux_labels.tracked_satellites);
#endif

    _gui_convert_gps_data_to_string(&aux_labels);
    _gui_set_gps_text_labels(&aux_labels);
}

static void _gui_init_aux_strings (_gui_aux_labels *aux_labels)
{
    strcpy(aux_labels->latitude_string, "Latitude: ");
    strcpy(aux_labels->longitude_string, "Longitude: ");
    strcpy(aux_labels->altitude_string, "Altitude: ");
    strcpy(aux_labels->tracked_satellite_string, "Tracked satellite: ");
}

static void _gui_convert_gps_data_to_string (_gui_aux_labels *aux_labels)
{
    char lat_chr[7];
    char long_chr[7];
    char altitude_chr[7];
    char tracked_satellite_chr[7];

    int16_t latitude, longitude;

    float_to_int16(aux_labels->latitude, &latitude);
    float_to_int16(aux_labels->longitude, &longitude);

    int_to_string(latitude, sizeof(lat_chr), lat_chr);
    int_to_string(longitude, sizeof(long_chr), long_chr);
    int_to_string(aux_labels->altitude, sizeof(altitude_chr), altitude_chr);
    int_to_string(aux_labels->tracked_satellites, sizeof(tracked_satellite_chr), tracked_satellite_chr);

    concatenate_strings(aux_labels->latitude_string, lat_chr);
    concatenate_strings(aux_labels->longitude_string, long_chr);
    concatenate_strings(aux_labels->altitude_string, altitude_chr);
    concatenate_strings(aux_labels->tracked_satellite_string, tracked_satellite_chr);
}

static void _gui_set_gps_text_labels(_gui_aux_labels *aux_labels)
{
    gui_set_text_label(gps_screen.latitude_label.label , aux_labels->latitude_string);
    gui_set_text_label(gps_screen.longitude_label.label , aux_labels->longitude_string);
    gui_set_text_label(gps_screen.altitude_label.label , aux_labels->altitude_string);
    gui_set_text_label(gps_screen.tracked_satellites_label.label , aux_labels->tracked_satellite_string);
}

#ifndef EMBEDDED
static void  gps_get_parsed_data_simulator(float *latitude, float *longitude, int32_t *altitude, int32_t *tracked_satellites)
{
    static float lat = 0, longtd = 0;
    static int32_t alt = 0, track = 0;
    *latitude = ++lat;
    *longitude = ++longtd;
    *altitude = ++alt;
    *tracked_satellites = ++track;
}
#endif