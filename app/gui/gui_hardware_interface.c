#include "gui_hardware_interface.h"

static int32_t encoder_last_value;
static lv_indev_state_t encoder_state;

static bool encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);
static bool is_encoder_button_pressed(void);

void gui_define_input_device(lv_group_t *g)
{
    lv_indev_drv_t enc_drv;
    lv_indev_drv_init(&enc_drv);
    enc_drv.type = LV_INDEV_TYPE_ENCODER;
    enc_drv.read_cb = encoder_read;
    lv_indev_t *enc_indev = lv_indev_drv_register(&enc_drv);
    lv_indev_set_group(enc_indev, g);
}

/**
 * Hardware callbacks
 */
static bool encoder_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data)
{
    static int lastBtn;
    int btn_state;

    if (is_encoder_button_pressed()) {
        btn_state = LV_INDEV_STATE_PR;
    } else {
        btn_state = LV_INDEV_STATE_REL;
    }

    encoder_state = btn_state;
    data->enc_diff = encoder_value - encoder_last_value;
    data->state = encoder_state;
    encoder_last_value = encoder_value;

    if (lastBtn != btn_state) {
        lastBtn = btn_state;
    }
    /*Return `false` because we are not buffering and no more data to read*/
    return false;
}

static bool is_encoder_button_pressed(void)
{
    bool flag = encoder_btn_flag;
    encoder_btn_flag = false;
    return flag;
}