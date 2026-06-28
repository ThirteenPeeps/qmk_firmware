#include QMK_KEYBOARD_H
#include "process_layer_lock.h"

/*
 * TODO: Nice features to have
 * - Re-enable audio but make it toggleable?
 * - Way to toggle numrow keys for numpad
 * - Emulated mouse keys
 * - Enable alternate default layer for wasd games that lets numbers and f-keys be accessed with one hand somehow
 * - Add print screen, display swap buttons. Maybe refactor brt/vol to share (mute can be display off/on)
 * - Something to toggle whether a mod is left/right
 */

// Define layer names
enum userspace_layers {
  _ABC, // Default layer
  _NAV, // Navigation
  _NUM, // Numpad
  _FNC, // Functions
  _ADJ, // Adjust
  _DUM
};

// Define custom keycodes
enum jrvt_keycodes_user {
  PW_MGR = SAFE_RANGE,
  LAPPY,
  DESKY,
  GMAIL,
  PALETTE
};

// Custom shift key pairings
const key_override_t COMM_OVERRIDE = ko_make_with_layers(MOD_MASK_SHIFT, KC_COMM, KC_SCLN, ~(1 << _NUM));
const key_override_t DOT_OVERRIDE = ko_make_with_layers(MOD_MASK_SHIFT, KC_DOT, KC_COLN, ~(1 << _NUM));
const key_override_t SLSH_OVERRIDE = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, KC_BSLS);
const key_override_t QUES_OVERRIDE = ko_make_basic(MOD_MASK_SHIFT, KC_QUES, KC_EXLM);

// Define config object as per https://docs.qmk.fm/features/key_overrides
const key_override_t *key_overrides[] = {
  &COMM_OVERRIDE,
  &DOT_OVERRIDE,
  &SLSH_OVERRIDE,
  &QUES_OVERRIDE
};

const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
  { {0, 0}, {10, 0}, {9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {11, 0}},
  {{11, 1}, {10, 1}, {9, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1},  {0, 1}},
  {{11, 2}, {10, 2}, {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2},  {0, 2}},
  {{11, 3}, {10, 3}, {9, 3}, {8, 3}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3},  {0, 3}},
};

/*
 * ,-----------------------------------------------------------------------.
 * | Esc |  Q  |  W  |  F  |  P  |  B  |  J  |  L  |  U  |  Y  | ? ! | Del |
 * |-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----|
 * | Tab |  A  |  R  |  S  |  T  |  G  |  M  |  N  |  E  |  I  |  O  | ' " |
 * |-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----|
 * | GUI |  Z  |  X  |  C  |  D  |  V  |  K  |  H  | , ; | . : | / \ | Ent |
 * |-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----|
 * |     |     |     | Alt | Ctl | Sft | Spc | Bsp | Fnc |     |     |     |
 * `-------------------------------------Nav---Num-------------------------'
 */

#define LT_SPC  LT(_NUM, KC_SPC)
#define LT_BSPC LT(_NAV, KC_BSPC)
#define LT_LOCK LT(_FNC, KC_F13)
#define LT_SWAP LT(_DUM, KC_ESC)
#define LT_LLCK LT(_FNC, KC_F14)

// DEFINE KEYBOARD LAYERS
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_ABC] = LAYOUT_planck_grid( // Base layer is modified Colemak
    LT_SWAP, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUES, KC_DEL,
    KC_TAB,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
    KC_LGUI, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
    XXXXXXX, XXXXXXX, XXXXXXX, KC_LALT, KC_LCTL, KC_LSFT, LT_SPC,  LT_BSPC, LT_LOCK, XXXXXXX, XXXXXXX, XXXXXXX
  ),
  [_NAV] = LAYOUT_planck_grid( // VIM contains HJKL-layout arrow keys but otherwise same as _NAV
    _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_INS,  _______,
    _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_SPC,  _______, LT_LLCK, _______, _______, _______
  ),
  [_NUM] = LAYOUT_planck_grid( // Num contains number row.  NOTE: redundant COMM/DOT needed for override mask
    _______, KC_GRV,  _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, _______,
    _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
    _______, _______, _______, _______, _______, _______, _______, _______, KC_COMM, KC_DOT,  KC_PIPE, _______,
    _______, _______, _______, _______, _______, _______, _______, KC_BSPC, LT_LLCK, _______, _______, _______
  ),
  [_FNC] = LAYOUT_planck_grid( // Fnc (or func) contains the F keys and brightness/media keys
    _______, _______, _______, _______, KC_PSCR, _______, KC_BRID, KC_VOLD, KC_MUTE, KC_VOLU, KC_BRIU, _______,
    KC_F12,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
    _______, _______, _______, _______, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_SPC,  _______, _______, _______, _______, _______
  ),
  [_ADJ] = LAYOUT_planck_grid( // Adj (adjust) has hardware adjustment keys and normal macros
    QK_BOOT, _______, _______, _______, PW_MGR,  _______, _______, LAPPY,   _______, _______, _______, _______,
    _______, _______, _______, _______, _______, GMAIL,   PALETTE, _______, _______, _______, _______, _______,
    CG_TOGG, _______, _______, _______, DESKY,   _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_SPC,  _______, _______, _______, _______, _______
  ),
  [_DUM] = LAYOUT_planck_grid( // Dummy layer only used for hand-swap logic
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  )
};

// Go to ADJ layer when NAV and FNC are activated together
layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _FNC, _NAV, _ADJ);
}

const uint32_t BLINK_HALF_PERIOD = 300; // milliseconds
bool blink_state = true;

uint32_t blink_state_toggle(uint32_t trigger_time, void *cb_arg) {
    blink_state = !blink_state;
    return BLINK_HALF_PERIOD;
}

// Return value ignored since we never need to cancel the timer
void keyboard_post_init_user(void) {
  defer_exec(BLINK_HALF_PERIOD, blink_state_toggle, NULL);
}

// Define RGB matrix palette names
enum palette_names {
  LIGHT,
  DARK,
  NUM_PALETTES
};

enum color_names {
  ___,
  DIM,
  MDL,
  BRT,
  NUM_COLORS
};

enum palette_names cur_palette = LIGHT;

void suspend_power_down_user(void) {
  cur_palette = DARK;
}

void suspend_wakeup_init_user(void) {
  cur_palette = LIGHT;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LT_LLCK:
      if (record->tap.count && record->event.pressed) { return process_layer_lock(QK_LLCK, record); }
      else { return true; }
    case LT_SWAP:
      if (!record->tap.count && record->event.pressed) {
        swap_hands_toggle();
        return false;
      } else { return true; }
    case LT_LOCK:
      if (record->tap.count && record->event.pressed) {
        uint16_t kc = QK_LOCK;
        return process_key_lock(&kc, record);
      } else { return true; }
    case PALETTE:
      if (record->event.pressed) { cur_palette = (cur_palette + 1) % NUM_PALETTES; }
      return false;
    case PW_MGR:
      if (record->event.pressed) { SEND_STRING("paste`queaussprachemag>nanimous"); }
      return false;
    case LAPPY:
      if (record->event.pressed) { SEND_STRING("birdholespermbank"); }
      return false;
    case DESKY:
      if (record->event.pressed) { SEND_STRING("eviloverheadludicroustrash"); }
      return false;
    case GMAIL:
      if (record->event.pressed) { SEND_STRING("mx.june.reese@gmail.com"); }
      return false;
    default:
      return true;
  }
}

const RGB PALETTES[][NUM_COLORS] = {
  [LIGHT] = {
    [___] = {.r = 0x00, .g = 0x00, .b = 0x00},
    [DIM] = {.r = 0x3f, .g = 0x00, .b = 0x7f},
    [MDL] = {.r = 0x7f, .g = 0x00, .b = 0x3f},
    [BRT] = {.r = 0x7f, .g = 0x5f, .b = 0x00}
  },
  [DARK] = {
    [___] = {.r = 0x00, .g = 0x00, .b = 0x00},
    [DIM] = {.r = 0x05, .g = 0x00, .b = 0x00},
    [MDL] = {.r = 0x7f, .g = 0x05, .b = 0x00},
    [BRT] = {.r = 0x7f, .g = 0x20, .b = 0x00}
  }
};

// To quickly modify the LED index while hands are swapped
const uint8_t HAND_SWAP_LEDS[RGB_MATRIX_LED_COUNT] = {
   0, 10,  9,  8,  7,  6,      5,  4,  3,  2,  1, 11,
  23, 22, 21, 20, 19, 18,     17, 16, 15, 14, 13, 12,
  35, 34, 33, 32, 31, 30,     29, 28, 27, 26, 25, 24,
  48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36
};

const uint8_t COLORMAPS[][RGB_MATRIX_LED_COUNT] = {
  [_ABC] = {
    MDL, ___, ___, ___, ___, ___,      ___, ___, ___, ___, ___, DIM,
    ___, ___, ___, ___, BRT, ___,      ___, BRT, ___, ___, ___, ___,
    DIM, ___, ___, ___, ___, ___,      ___, ___, ___, ___, ___, DIM,
    ___, ___, ___, DIM, DIM, DIM, ___, MDL, MDL, MDL, ___, ___, ___
  },
  [_NAV] = {
    MDL, ___, ___, ___, ___, ___,      DIM, DIM, DIM, DIM, MDL, DIM,
    ___, ___, ___, ___, BRT, ___,      MDL, BRT, MDL, MDL, ___, ___,
    DIM, ___, ___, ___, ___, ___,      ___, ___, ___, ___, ___, DIM,
    ___, ___, ___, DIM, DIM, DIM, ___, DIM, BRT, MDL, ___, ___, ___
  },
  [_NUM] = {
    MDL, DIM, ___, ___, ___, ___,      ___, DIM, DIM, MDL, MDL, DIM,
    ___, MDL, MDL, MDL, BRT, MDL,      MDL, BRT, MDL, MDL, MDL, ___,
    DIM, ___, ___, ___, ___, ___,      ___, ___, MDL, MDL, DIM, DIM,
    ___, ___, ___, DIM, DIM, DIM, ___, BRT, DIM, MDL, ___, ___, ___
  },
  [_FNC] = {
    MDL, ___, ___, ___, ___, ___,      MDL, MDL, MDL, MDL, MDL, DIM,
    MDL, DIM, DIM, DIM, BRT, DIM,      DIM, BRT, DIM, DIM, DIM, MDL,
    DIM, ___, ___, ___, ___, ___,      ___, MDL, MDL, MDL, ___, DIM,
    ___, ___, ___, DIM, DIM, DIM, ___, DIM, MDL, BRT, ___, ___, ___
  },
  [_ADJ] = {
    BRT, ___, ___, ___, DIM, ___,      ___, DIM, ___, ___, ___, DIM,
    ___, ___, ___, ___, ___, DIM,      MDL, ___, ___, ___, ___, ___,
    MDL, ___, ___, ___, DIM, ___,      ___, ___, ___, ___, ___, DIM,
    ___, ___, ___, DIM, DIM, DIM, ___, DIM, BRT, BRT, ___, ___, ___
  }
};

// Single-use macro real quick
#define SET_COLOR_IF(color_num, condition) {\
  if (condition) { \
    cur_color = PALETTES[cur_palette][color_num]; \
    break; \
  } else { goto default_label; } \
}

bool rgb_matrix_indicators_user(void) {
  uint8_t mods = get_mods();

  for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
    RGB cur_color;

    switch (i) {
      case 0:
        SET_COLOR_IF(BRT, blink_state && is_swap_hands_on());
      case 24:
        if (keymap_config.swap_lctl_lgui) {
          if (mods & MOD_BIT(KC_LCTL)) { SET_COLOR_IF(blink_state ? BRT : MDL, true); }
          else { SET_COLOR_IF(blink_state ? DIM : MDL, true); }
        } else { SET_COLOR_IF(BRT, mods & MOD_BIT(KC_LGUI)); }
      case 39:
        SET_COLOR_IF(BRT, mods & MOD_BIT(KC_LALT));
      case 40:
        if (keymap_config.swap_lctl_lgui) {
          if (mods & MOD_BIT(KC_LGUI)) { SET_COLOR_IF(blink_state ? BRT : MDL, true); }
          else { SET_COLOR_IF(blink_state ? DIM : MDL, true); }
        } else { SET_COLOR_IF(BRT, mods & MOD_BIT(KC_LCTL)); }
      case 41:
        SET_COLOR_IF(BRT, mods & MOD_BIT(KC_LSFT));
      case 45:
        SET_COLOR_IF(BRT, is_key_lock_waiting() || (blink_state && is_layer_locked(get_highest_layer(layer_state))));
      default:
        default_label:
        cur_color = PALETTES[cur_palette][COLORMAPS[biton32(layer_state)][i]];
    }

    uint8_t idx = (is_swap_hands_on() ? HAND_SWAP_LEDS[i] : i);
    rgb_matrix_set_color(idx, cur_color.r, cur_color.g, cur_color.b);
  }

  return true;
}

#undef JUNIP_MACRO
