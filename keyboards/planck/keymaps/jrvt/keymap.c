#include QMK_KEYBOARD_H
#include "process_layer_lock.h"

/*
 * TODO: Nice features to have
 * - Re-enable audio but make it toggleable?
 * - Somehow indicate when control and GUI are swapped
 * - Double tap shift for caps lock
 * - Way to toggle numrow keys for numpad
 * - Emulated mouse keys
 * - Double tap and hold mods or GUI to mirror other keys?
 * - Enable alternate default layer for wasd games that lets numbers and f-keys be accessed with one hand somehow
 * - Add print screen, display swap buttons. Maybe refactor brt/vol to share (mute can be display off/on)
 * - Something to toggle whether a mod is left/right
 */

/* RGB matrix: colors per palette */
#define NUM_COLORS 4

/* Define RGB matrix palette names */
enum palette_names {
  LIGHT,
  DARK,
  NUM_PALETTES
};

/* Define layer names */
enum userspace_layers {
  _ABC, // Default layer
  _NAV, // Navigation
  _NUM, // Numpad
  _FNC, // Functions
  _ADJ, // Adjust
  _DUM
};

/* Define custom keycodes */
enum jrvt_keycodes_user {
  PW_MGR = SAFE_RANGE,
  LAPPY,
  DESKY,
  GMAIL,
  PALETTE
};

/* Custom shift key pairings */
const key_override_t COMM_OVERRIDE = ko_make_basic(MOD_MASK_SHIFT, KC_COMM, KC_SCLN);
const key_override_t DOT_OVERRIDE = ko_make_basic(MOD_MASK_SHIFT, KC_DOT, KC_COLN);
const key_override_t SLSH_OVERRIDE = ko_make_basic(MOD_MASK_SHIFT, KC_SLSH, KC_BSLS);
const key_override_t QUES_OVERRIDE = ko_make_basic(MOD_MASK_SHIFT, KC_QUES, KC_EXLM);

/* Define config object as per https://docs.qmk.fm/features/key_overrides */
const key_override_t *key_overrides[] = {
  &COMM_OVERRIDE,
  &DOT_OVERRIDE,
  &SLSH_OVERRIDE,
  &QUES_OVERRIDE
};

const keypos_t PROGMEM hand_swap_config[MATRIX_ROWS][MATRIX_COLS] = {
  {{0, 0}, {10, 0}, {9, 0}, {8, 0}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {11, 0}},
  {{11, 1}, {10, 1}, {9, 1}, {8, 1}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}},
  {{11, 2}, {10, 2}, {9, 2}, {8, 2}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}},
  {{11, 3}, {10, 3}, {9, 3}, {8, 3}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}},
};

// KC_OPER will be intercepted and replaced with QK_LOCK verbatim
// The only way to activate SH_MON is to change to _DUM, so when SH_MON is released, we deactivate _DUM

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

/* TODO: The goal: QK_LOCK functionality on some key or another. Seems like it needs to be already held
* when pressing the key to be locked. But must test this before any further wild goose chases.
* If that is the case then we certainly can't use LT, might have to do apportion Enter for this purpose.
* But I'd really like to not have to since I do sometimes need to hold that key :< same for Del.
*/

#define LT_LOCK LT(_FNC, KC_F13)
#define LT_SWAP LT(_DUM, KC_ESC)
#define LT_LLCK LT(_FNC, KC_F14)

/* DEFINE KEYBOARD LAYERS */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_ABC] = LAYOUT_planck_grid( // Base layer is modified Colemak
    LT_SWAP, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUES, KC_DEL,
    KC_TAB,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,    KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT,
    KC_LGUI, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT,
    XXXXXXX, XXXXXXX, XXXXXXX, KC_LALT, KC_LCTL, KC_LSFT, LT(_NUM, KC_SPC),  LT(_NAV, KC_BSPC), LT_LOCK, XXXXXXX, XXXXXXX, XXXXXXX
  ),

  // [_NAV] = LAYOUT_planck_grid( // Nav contains navigation keys and all brackets
  //   _______, _______, KC_LBRC, _______, KC_RBRC, _______, _______, KC_PGUP, KC_UP,   KC_PGDN, _______, _______,
  //   _______, KC_LT,   KC_LPRN, _______, KC_RPRN, KC_GT,   _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
  //   _______, _______, KC_LCBR, _______, KC_RCBR, _______, _______, KC_HOME, KC_INS,  KC_END,  _______, _______,
  //   _______, _______, _______, _______, _______, _______, _______, _______, QK_LLCK, _______, _______, _______
  // ),
  //

  [_NAV] = LAYOUT_planck_grid( // VIM contains HJKL-layout arrow keys but otherwise same as _NAV
    _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_INS,  _______,
    _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_SPC,  _______, LT_LLCK, _______, _______, _______
  ),

  [_NUM] = LAYOUT_planck_grid( // Num contains number row and the common symbols
    _______, KC_GRV,  _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  _______,
    _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
    _______, KC_LT,   KC_GT,   _______, _______, _______, _______, _______, _______, KC_LBRC, KC_RBRC, _______,
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

  [_DUM] = LAYOUT_planck_grid(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  )
};

/* PALETTES[0] = light palette; [1] = dark palette */
const RGB PALETTES[][NUM_COLORS] = {
  [0] = {
    {.r = 0x00, .g = 0x00, .b = 0x00},  // 0 off
    {.r = 0x3f, .g = 0x00, .b = 0x7f},  // 1 dim
    {.r = 0x7f, .g = 0x00, .b = 0x3f},  // 2 mid
    {.r = 0x7f, .g = 0x5f, .b = 0x00}   // 3 brt
  },
  [1] = {
    {.r = 0x00, .g = 0x00, .b = 0x00},
    {.r = 0x05, .g = 0x00, .b = 0x00},
    {.r = 0x7f, .g = 0x05, .b = 0x00},
    {.r = 0x7f, .g = 0x20, .b = 0x00}
  }
};

// To quickly modify the LED index while hands are swapped
const uint8_t HAND_SWAP_LEDS[RGB_MATRIX_LED_COUNT] = {
   0, 10,  9,  8,  7,  6,     5,  4,  3,  2,  1,  11,
  23, 22, 21, 20, 19, 18,     17, 16, 15, 14, 13, 12,
  35, 34, 33, 32, 31, 30,     29, 28, 27, 26, 25, 24,
  48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36
};

const uint8_t COLORMAPS[][RGB_MATRIX_LED_COUNT] = {
  [_ABC] = {
    2, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 3, 0,    0, 3, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 1,
    0, 0, 0, 1, 1, 1, 0, 2, 2, 2, 0, 0, 0
  },
  [_NAV] = {
    2, 0, 0, 0, 0, 0,    1, 1, 1, 1, 2, 1,
    0, 0, 0, 0, 3, 0,    2, 3, 2, 2, 0, 0,
    1, 0, 0, 0, 0, 0,    0, 0, 0, 0, 0, 1,
    0, 0, 0, 1, 1, 1, 0, 1, 3, 2, 0, 0, 0
  },
  [_NUM] = {
    2, 1, 0, 0, 0, 0,    0, 0, 0, 2, 2, 1,
    0, 2, 2, 2, 3, 2,    2, 3, 2, 2, 2, 0,
    1, 2, 2, 0, 0, 0,    0, 0, 0, 2, 2, 1,
    0, 0, 0, 1, 1, 1, 0, 3, 1, 2, 0, 0, 0
  },
  [_FNC] = {
    2, 0, 0, 0, 0, 0,    2, 2, 2, 2, 2, 1,
    2, 1, 1, 1, 3, 1,    1, 3, 1, 1, 1, 2,
    1, 0, 0, 0, 0, 0,    0, 2, 2, 2, 0, 1,
    0, 0, 0, 1, 1, 1, 0, 1, 2, 3, 0, 0, 0
  },
  [_ADJ] = {
    1, 0, 0, 0, 1, 0,    0, 1, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 1,    2, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 1, 0,    0, 0, 0, 0, 0, 1,
    0, 0, 0, 1, 1, 1, 0, 1, 3, 3, 0, 0, 0
  }
};

/* Go to ADJ layer when NAV and FNC are activated together */
layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _FNC, _NAV, _ADJ);
}

enum palette_names cur_palette = LIGHT;

void suspend_power_down_user(void) {
  cur_palette = DARK;
}
void suspend_wakeup_init_user(void) {
  cur_palette = LIGHT;
}

bool key_lock_waiting = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    /* Do QK_LLCK on tap, do regular layer change on hold */
    case LT_LLCK:
      if (record->tap.count && record->event.pressed) {
        return process_layer_lock(QK_LLCK, record);
      } else {
        return true;
      }

    /* Ef LT_SWAP gets held, toggle hand swap state */
    case LT_SWAP:
      if (!record->tap.count && record->event.pressed) {
        swap_hands_toggle();
        return false;
      } else {
        return true;
      }

    /* Substitutes KC_NUM with QK_LOCK */
    case LT_LOCK:
      if (record->tap.count && record->event.pressed) {
        uint16_t kc = QK_LOCK;
        return process_key_lock(&kc, record);
      } else {
        return true;
      }

    /* Typing macros */
    case PW_MGR:
      if (record->event.pressed)
        SEND_STRING("paste`queaussprachemag>nanimous");
      return false;
    case LAPPY:
      if (record->event.pressed)
        SEND_STRING("birdholespermbank");
      return false;
    case DESKY:
      if (record->event.pressed)
        SEND_STRING("eviloverheadludicroustrash");
      return false;
    case GMAIL:
      if (record->event.pressed)
        SEND_STRING("mx.june.reese@gmail.com");
      return false;

    /* Cycle color palettes */
    case PALETTE:
      if (record->event.pressed)
        cur_palette = (cur_palette + 1) % NUM_PALETTES;
      return false;

    /* Process all other keycodes normally */
    default:
      return true;
  }
}

#define SET_BRIGHT_IF(A) {\
  if (A) { \
    cur_color = PALETTES[cur_palette][3]; \
    break; \
  } else { \
    goto default_label; \
  } \
}

bool rgb_matrix_indicators_user(void) {
  uint8_t mods = get_mods();

  for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
    // Handle special cases
    RGB cur_color;

    switch (i) {
      case 0:
        SET_BRIGHT_IF(is_swap_hands_on());
      case 24:
        SET_BRIGHT_IF(mods & MOD_BIT(KC_LGUI));
      case 39:
        SET_BRIGHT_IF(mods & MOD_BIT(KC_LALT));
      case 40:
        SET_BRIGHT_IF(mods & MOD_BIT(KC_LCTL));
      case 41:
        SET_BRIGHT_IF(mods & MOD_BIT(KC_LSFT));
      case 46:
        SET_BRIGHT_IF(key_lock_waiting || is_key_locked());
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
