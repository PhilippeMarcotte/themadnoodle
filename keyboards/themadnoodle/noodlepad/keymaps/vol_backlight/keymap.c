#include QMK_KEYBOARD_H
#include "raw_hid.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* LAYER 0
     * ,-----------------------.
     * |   <<  |  MUTE |  >>   |  ENCODER - PRESS (MUTE) / KNOB (VOLUME CONTROL)
     * |-------+-------+-------|
     * |  STOP |  PLAY | MEDIA |
     * |-------+-------+-------|
     * | CALC  | MAIL  | PC/FN |
     * `-----------------------'
     */

    [0] = LAYOUT_ortho_3x3(
      KC_MPRV, LT(2, KC_MUTE), KC_MNXT,
      KC_F18, KC_MPLY, KC_MSEL,
      KC_F17, KC_MAIL, TO(1)
      ),


    /* LAYER 1
     * ,-----------------------.
     * | MODE+ |RGB TOG| MODE- |  ENCODER - PRESS (NA) / KNOB (Hue Control)
     * |-------+-------+-------|
     * |  SPD- |  SPD+ |Bright |
     * |-------+-------+-------|
     * |  SAT+ |  SAT- |       |
     * `-----------------------'
     */

    [1] = LAYOUT_ortho_3x3(
      RGB_MOD, RGB_TOG, RGB_RMOD,
      KC_TRNS, RGB_SPD, RGB_VAI,
      KC_TRNS, RGB_SAD, TO(2)
      ),


    /* LAYER 2 (ENCODER)
     * ,-----------------------.
     * |       |       |       |  ENCODER - PRESS (NA) / KNOB (Arrow Left/Right)
     * |-------+-------+-------|
     * |       |       |       |
     * |-------+-------+-------|
     * |       |       |       |
     * `-----------------------'
     */

    [2] = LAYOUT_ortho_3x3(
      KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, KC_TRNS,
      KC_TRNS, KC_TRNS, TO(0)
      )

};

RGB layer_color;
HSV changingVolumeColor;
bool isChangingVolume = false;
uint16_t changingVolumeTimer;

void keyboard_post_init_user(void) {
    changingVolumeColor = (HSV){205, 255, 255};
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    return layer_state_set_user(state);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
    case 1:
        layer_color = (RGB){0x00, 0x00, 0xFF};
        break;
    case 2:
        layer_color = (RGB){0xFF,  0x00, 0x00};
        break;
    default: //  for any other layers, or the default layer
        layer_color = (RGB){0x00,  0xFF, 0xFF};
        break;
    }
    if (!isChangingVolume)
        rgblight_setrgb(layer_color.r, layer_color.g, layer_color.b);
    return state;
}

void matrix_scan_user(void) {
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case 1:
            if (clockwise) {
                tap_code16(KC_F13);
            } else {
                tap_code16(KC_F14);
            }
            break;
        case 2:
            if (clockwise) {
                tap_code16(KC_F15);
            } else {
                tap_code16(KC_F16);
            }
            break;
        default:
            if (clockwise) {
                tap_code16(KC_VOLU);
            } else {
                tap_code16(KC_VOLD);
            }
            break;
    }
    return false;
}
