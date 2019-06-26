#include <asm-generic/errno-base.h>
#include <fcntl.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define uchar unsigned char
#define DEBUG 0
#define TEST_BIT(var, bitval) ((var & bitval) == bitval)
#define KEYMAP_LEN 0x66

void init_keymap(uchar *map) {
  memset(map, 0, KEYMAP_LEN);
  map[0x04] = KEY_A;
  map[0x05] = KEY_B;
  map[0x06] = KEY_C;
  map[0x07] = KEY_D;
  map[0x08] = KEY_E;
  map[0x09] = KEY_F;
  map[0x0a] = KEY_G;
  map[0x0b] = KEY_H;
  map[0x0c] = KEY_I;
  map[0x0d] = KEY_J;
  map[0x0e] = KEY_K;
  map[0x0f] = KEY_L;
  map[0x10] = KEY_M;
  map[0x11] = KEY_N;
  map[0x12] = KEY_O;
  map[0x13] = KEY_P;
  map[0x14] = KEY_Q;
  map[0x15] = KEY_R;
  map[0x16] = KEY_S;
  map[0x17] = KEY_T;
  map[0x18] = KEY_U;
  map[0x19] = KEY_V;
  map[0x1a] = KEY_W;
  map[0x1b] = KEY_X;
  map[0x1c] = KEY_Y;
  map[0x1d] = KEY_Z;
  map[0x1e] = KEY_1;
  map[0x1f] = KEY_2;
  map[0x20] = KEY_3;
  map[0x21] = KEY_4;
  map[0x22] = KEY_5;
  map[0x23] = KEY_6;
  map[0x24] = KEY_7;
  map[0x25] = KEY_8;
  map[0x26] = KEY_9;
  map[0x27] = KEY_0;
  map[0x28] = KEY_ENTER;
  map[0x29] = KEY_ESC;
  map[0x2a] = KEY_BACKSPACE;
  map[0x2b] = KEY_TAB;
  map[0x2c] = KEY_SPACE;
  map[0x2d] = KEY_MINUS;
  map[0x2e] = KEY_EQUAL;
  map[0x2f] = KEY_LEFTBRACE;
  map[0x30] = KEY_RIGHTBRACE;
  map[0x32] = KEY_BACKSLASH;
  map[0x33] = KEY_SEMICOLON;
  map[0x34] = KEY_APOSTROPHE;
  map[0x35] = KEY_GRAVE;
  map[0x36] = KEY_COMMA;
  map[0x37] = KEY_DOT;
  map[0x38] = KEY_SLASH;
  map[0x39] = KEY_CAPSLOCK;
  map[0x3a] = KEY_F1;
  map[0x3b] = KEY_F2;
  map[0x3c] = KEY_F3;
  map[0x3d] = KEY_F4;
  map[0x3e] = KEY_F5;
  map[0x3f] = KEY_F6;
  map[0x40] = KEY_F7;
  map[0x41] = KEY_F8;
  map[0x42] = KEY_F9;
  map[0x43] = KEY_F10;
  map[0x44] = KEY_F11;
  map[0x45] = KEY_F12;
  map[0x46] = KEY_SYSRQ;
  map[0x47] = KEY_SCROLLLOCK;
  map[0x48] = KEY_PAUSE;
  map[0x49] = KEY_INSERT;
  map[0x4a] = KEY_HOME;
  map[0x4b] = KEY_PAGEUP;
  map[0x4c] = KEY_DELETE;
  map[0x4d] = KEY_END;
  map[0x4e] = KEY_PAGEDOWN;
  map[0x4f] = KEY_RIGHT;
  map[0x50] = KEY_LEFT;
  map[0x51] = KEY_DOWN;
  map[0x52] = KEY_UP;
  map[0x53] = KEY_NUMLOCK;
  map[0x54] = KEY_KPSLASH;
  map[0x55] = KEY_KPASTERISK;
  map[0x56] = KEY_KPMINUS;
  map[0x57] = KEY_KPPLUS;
  map[0x58] = KEY_KPENTER;
  map[0x59] = KEY_KP1;
  map[0x5a] = KEY_KP2;
  map[0x5b] = KEY_KP3;
  map[0x5c] = KEY_KP4;
  map[0x5d] = KEY_KP5;
  map[0x5e] = KEY_KP6;
  map[0x5f] = KEY_KP7;
  map[0x60] = KEY_KP8;
  map[0x61] = KEY_KP9;
  map[0x62] = KEY_KP0;
  map[0x63] = KEY_KPDOT;
  map[0x64] = KEY_102ND;
  map[0x65] = KEY_COMPOSE;
}

#define N_MODKEYS 7
const uchar modkeys[N_MODKEYS][2] = {
  // bit_mask, modifier_key
  {1, KEY_LEFTCTRL},
  {2, KEY_LEFTSHIFT},
  {4, KEY_LEFTALT},
  {8, KEY_LEFTMETA},
  {16, KEY_RIGHTCTRL},
  {32, KEY_RIGHTSHIFT},
  {64, KEY_RIGHTALT}
};

int htoi(char hex) {
  if ('0'<=hex && hex<='9') return hex - '0';
  if ('a'<=hex && hex<='f') return hex - 'a' + 10;
  if ('A'<=hex && hex<='F') return hex - 'A' + 10;
  return -1;
}

void read_hex_str(const char *input, int input_len, uchar *output, int n) {
  int j = 0;
  const char *end = input + input_len;
  for (const char *ptr = input; ptr < end && *ptr && j<n; ptr++) {
    int h = htoi(*ptr);
    if (h >= 0) output[j++] = h*16 + htoi(*(++ptr));
  }
}

int write_ev(const int fd, int type, int code, int value) {
  struct input_event ev;
  memset(&ev, 0, sizeof(ev));
  ev.type = type;
  ev.code = code;
  ev.value = value;
  return write(fd, &ev, sizeof(ev));
}

#define BUFFER_LEN 31
#define KEYLOG_LEN 8

int main() {
  uchar buffer[BUFFER_LEN+1];
  uchar keypressed[KEYLOG_LEN];
  uchar keypressed_prec[KEYLOG_LEN];
  uchar keymap[KEYMAP_LEN];
  init_keymap(keymap);

  const int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK | O_DSYNC);

  ioctl(fd, UI_SET_EVBIT, EV_KEY);
  ioctl(fd, UI_SET_EVBIT, EV_SYN);
  for (int i = 0; i < KEYMAP_LEN; i++) {
    if (keymap[i]) ioctl(fd, UI_SET_KEYBIT, keymap[i]);
  }

  struct uinput_user_dev uidev;
  memset(&uidev, 0, sizeof(uidev));
  snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
  uidev.id.bustype = BUS_USB;
  uidev.id.vendor = 0x1234;
  uidev.id.product = 0xfedc;
  uidev.id.version = 1;
  write(fd, &uidev, sizeof(uidev));
  ioctl(fd, UI_DEV_CREATE);
  if (DEBUG) printf("uinput all set\n");

  FILE *in = fopen("/dev/stdin", "r");
  if (DEBUG) printf("stdin is open\n");

  while (fgets(buffer, BUFFER_LEN, in)) {
    if (DEBUG) printf("read: %s", buffer);

    memset(keypressed, 0, KEYLOG_LEN);
    read_hex_str(buffer, BUFFER_LEN, keypressed, KEYLOG_LEN);

    /* special keys */
    const uchar kp = keypressed[0];
    const uchar kr = keypressed_prec[0];
    if (DEBUG) printf("keypressed_prec[0]=%x  keypressed[0]=%x\n", kr, kp);

    if (kr != kp) {
      for (int i = 0; i < N_MODKEYS; i++) {
        /* released */
        if (TEST_BIT(kr, modkeys[i][0]) && !TEST_BIT(kp, modkeys[i][0])) {
          if (DEBUG) printf("Mod key %d released\n", i);
          write_ev(fd, EV_KEY, modkeys[i][1], 0);
        }
        /* pressed */
        if (!TEST_BIT(kr, modkeys[i][0]) && TEST_BIT(kp, modkeys[i][0])) {
          if (DEBUG) printf("Mod key %d pressed\n", i);
          write_ev(fd, EV_KEY, modkeys[i][1], 1);
        }
      }
    }

    /* regular keys */
    int j;
    for (int i = 2; i < KEYLOG_LEN; i++) {
      const uchar kp = keypressed[i];
      const uchar kr = keypressed_prec[i];
      if (DEBUG) printf("i=%d  kr=%x  kp=%x\n", i, kr, kp);

      if (DEBUG && kp && (kp >= KEYMAP_LEN || !keymap[kp]))
        printf("Key %x is not mapped\n", kp);

      if (kr) {
        /* still pressed? */
        for (j = 2; j < KEYLOG_LEN; j++) {
          if (kr == keypressed[j]) break;
        }
        if (j == KEYLOG_LEN && kr < KEYMAP_LEN) {
          /* key is released */
          write_ev(fd, EV_KEY, keymap[kr], 0);
        }
      }

      if (kp) {
        /* wasn't pressed? */
        for (j = 2; j < KEYLOG_LEN; j++) {
          if (keypressed_prec[j] == kp) break;
        }
        if (j == KEYLOG_LEN && kp < KEYMAP_LEN) {
          /* key is newly pressed */
          write_ev(fd, EV_KEY, keymap[kp], 1);
        }
      }
    }

    write_ev(fd, EV_SYN, SYN_REPORT, 1);

    memcpy(keypressed_prec, keypressed, KEYLOG_LEN);
    if (DEBUG) printf("\n");
  }

  return 0;
}
