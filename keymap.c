/* Copyright 2018 Eucalyn
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H
#include "keymap_jp.h"
#include "quantum.h"

enum custom_keycodes {
  RGBRST = SAFE_RANGE,
  RGBMRST,
};

enum layer_number{
    _BASE = 0,
    _FN,
    _ADJUST,
};

uint16_t idle_timer;
bool is_sleeping = false;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT( \
    KC_ESC,  JP_1,    JP_2,    JP_3,    JP_4,    JP_5,        JP_6,    JP_7,    JP_8,    JP_9,    JP_0,    JP_MINS, JP_CIRC,    KC_BSPC,  \
    KC_TAB,    JP_Q,    JP_W,    JP_E,    JP_R,    JP_T,        JP_Y,    JP_U,    JP_I,    JP_O,    JP_P,    JP_AT, JP_LBRC, JP_RBRC,  \
    MO(_FN),    JP_A,    JP_S,    JP_D,    JP_F,   JP_G,        JP_H,    JP_J,    JP_K,    JP_L,    JP_SCLN, JP_COLN,        KC_ENT,   \
    KC_LSFT,      JP_Z,    JP_X,    JP_C,    JP_V,   JP_B,        JP_N,    JP_M,    JP_COMM, JP_DOT,  JP_SLSH, JP_BSLS,   KC_LSFT,  MO(_ADJUST),   \
    JP_ZKHK,  KC_LGUI,  KC_LALT,  KC_LCTL,    KC_SPC,             KC_SPC,              KC_LSFT,   KC_LCTL,    MO(_FN),  KC_APP,KC_DEL \
  ),
  [_FN] = LAYOUT( \
    S(KC_CAPS), KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,        KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,    KC_PSCR,  \
    _______, _______, _______, _______, _______, _______,      _______,   KC_PGUP, _______, _______, _______,  _______, _______, _______,  \
    _______,  _______, _______,   _______, _______, _______,        KC_LEFT,   KC_DOWN,   KC_UP  ,   KC_RGHT,    _______, _______, _______,   \
    _______,    _______, _______, _______, _______, _______,      _______,   KC_PGDN,  _______,  _______,  _______,  C(KC_F),  _______,  _______,   \
    S(KC_CAPS),    _______,   _______,  _______,  _______,             _______,              _______,   _______,    _______, _______,_______\
  ),
  [_ADJUST] = LAYOUT( \
    RGB_TOG, RGBRST, RGB_MOD, RGB_HUI, RGB_SAI, RGB_VAI,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX,  \
    RGB_M_X, RGBMRST, RGB_RMOD, RGB_HUD, RGB_SAD, RGB_VAD,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  \
    KC_MUTE,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX,       XXXXXXX,   \
    KC_VOLU,    KC_MNXT, KC_MSTP, KC_BRIU, XXXXXXX, KC_MSTP,     XXXXXXX, XXXXXXX,    XXXXXXX,XXXXXXX,  XXXXXXX, XXXXXXX,   XXXXXXX,  _______,   \
    KC_VOLD,    KC_MPRV,    KC_MPLY,    KC_BRID,      KC_MPLY,          XXXXXXX,              XXXXXXX,   XXXXXXX,    XXXXXXX, XXXXXXX,XXXXXXX\
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
        }
      #endif
      break;
    case RGBMRST:
      if (record->event.pressed) {
          rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
      }
      break;
  }
  if(is_sleeping){
      rgblight_enable_noeeprom();
      is_sleeping=false;
      idle_timer=timer_read();
  }
  return true;
}

uint32_t layer_state_set_user(uint32_t state){
    switch (biton32(state)) {
        case _BASE:
            if(rgblight_get_hue()!=0){
                rgblight_sethsv_noeeprom(0,rgblight_get_sat(),rgblight_get_val());
            }
            break;
        case _FN:
            rgblight_sethsv_noeeprom(85,rgblight_get_sat(),rgblight_get_val());
            break;
        case _ADJUST:
            rgblight_sethsv_noeeprom(170,rgblight_get_sat(),rgblight_get_val());
            break;
    }
    return state;
}

void matrix_scan_user(void){
    if(!is_sleeping&&timer_elapsed(idle_timer)>60000){
        rgblight_disable_noeeprom();
        is_sleeping=true;
    }
}
typedef const uint16_t comb_keys_t[];
static PROGMEM comb_keys_t
    comb_keys_PIPE = {KC_LSFT, KC_0, COMBO_END},
    comb_keys_ZKHK = {KC_LSFT, KC_ESC, COMBO_END};
combo_t key_combos[COMBO_COUNT]={
    COMBO( comb_keys_PIPE, JP_PIPE),
    COMBO( comb_keys_ZKHK, JP_ZKHK)
};
