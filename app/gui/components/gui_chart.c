#include "gui_chart.h"

static gui_chart_st chart_st;

static void update_chart_task(lv_task_t *task);
static void _gui_create_chart(lv_obj_t *parent);
static void _gui_define_chart_curves(void);
static void _gui_create_chart_label(lv_obj_t *parent);
static void _gui_create_chart_task(void);
static void _gui_update_chart_points(gui_chart_st *chart, int16_t pitch, int16_t roll);

void gui_create_chart(lv_obj_t *parent)
{
    _gui_create_chart(parent);
    _gui_define_chart_curves();
    _gui_create_chart_label(parent);
    _gui_create_chart_task();
}

static void _gui_create_chart(lv_obj_t *parent)
{
    chart_st.chart = lv_chart_create(parent, NULL);
    set_size(chart_st.chart, 220, 220);
    set_align(chart_st.chart, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart_st.chart, LV_CHART_TYPE_LINE);
}

static void _gui_define_chart_curves(void)
{
    chart_st.pitch_line = lv_chart_add_series(chart_st.chart, LV_COLOR_RED);
    chart_st.roll_line = lv_chart_add_series(chart_st.chart, LV_COLOR_NAVY);

    for (int i = 0; i < CHART_POINTS_NUMBER; i++) {
        lv_chart_set_next(chart_st.chart, chart_st.pitch_line, CHART_OFFSET_VALUE);
        lv_chart_set_next(chart_st.chart, chart_st.roll_line, CHART_OFFSET_VALUE);
    }
}

static void _gui_create_chart_label(lv_obj_t *parent)
{
    chart_st.chart_label = (_gui_label_st){.parent = parent, 
                                .text = "Pitch and Roll", 
                                .align = LV_ALIGN_IN_TOP_MID, 
                                .x_ofs = 0, 
                                .y_ofs = 35};

    chart_st.chart_label.label = gui_create_label(&chart_st.chart_label);
    lv_chart_refresh(chart_st.chart);
}

static void _gui_create_chart_task(void)
{
    chart_st.chart_task = (_gui_task_st){ .task_xcb = update_chart_task, 
                                .period = 30, 
                                .priority = LV_TASK_PRIO_LOWEST,
                                .parameter = &chart_st};
    gui_create_task(&chart_st.chart_task);
}

static void update_chart_task(lv_task_t *task)
{
    int16_t pitch, roll;
    gui_chart_st *chart = (gui_chart_st *)(task->user_data);

#ifdef EMBEDDED
    imu_get_pitch_and_roll(&pitch, &roll);
#endif
    
    _gui_update_chart_points(chart, pitch,roll);
}

static void _gui_update_chart_points(gui_chart_st *chart, int16_t pitch, int16_t roll)
{
    for (int i = 0; i < CHART_POINTS_NUMBER; i++) {
        if (i == (CHART_POINTS_NUMBER - 1)) {
            chart->pitch_line->points[i] = (int16_t)pitch + CHART_OFFSET_VALUE;
            chart->roll_line->points[i] = (int16_t)roll + CHART_OFFSET_VALUE;
        } else {
            chart->pitch_line->points[i] = chart->pitch_line->points[i + 1];
            chart->roll_line->points[i] = chart->roll_line->points[i + 1];
        }
    }
    lv_chart_refresh(chart->chart);
}