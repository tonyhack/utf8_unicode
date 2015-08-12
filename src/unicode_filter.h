#ifndef UNICODE_FILTER_H
#define UNICODE_FILTER_H

#include "utf8_unicode.h"

struct unicode_range {
  int min;
  int max;
};

bool unicode_check_range(int value, const struct unicode_range *range, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    if (value >= range[i].min && value <= range[i].max) {
      return true;
    }
  }

  return false;
}


int unicode_filter(const char *str, size_t size, const struct unicode_range *range, size_t range_size) {
  int unicode = 0;

  for (const char *p = str; *p != 0;) {
    unsigned char c = *p;

    if (c >> 7 == 0x0) {
      // single byte
      unicode = utf8_convert_unicode(p, 1);
      if (unicode < 0 || unicode_check_range(unicode, range, range_size) == false) {
        return -1;
      }
      ++p;
    } else if (c >> 5 == 0x06) {
      // double bytes
      if (utf8_check_bytes(p, 1) == false) {
        return -1;
      }
      unicode = utf8_convert_unicode(p, 2);
      if (unicode < 0 || unicode_check_range(unicode, range, range_size) == false) {
        return -1;
      }
      p += 2;
    } else if (c >> 4 == 0x0e) {
      // three bytes
      if (utf8_check_bytes(p, 2) == false) {
        return -1;
      }
      unicode = utf8_convert_unicode(p, 3);
      if (unicode < 0 || unicode_check_range(unicode, range, range_size) == false) {
        return -1;
      }
      p += 3;
    } else if (c >> 3 == 0x1e) {
      // four bytes
      if (utf8_check_bytes(p, 3) == false) {
        return -1;
      }
      unicode = utf8_convert_unicode(p, 4);
      if (unicode < 0 || unicode_check_range(unicode, range, range_size) == false) {
        return -1;
      }
      p += 4;
    } else {
      return -1;
    }
  }

  return 0;
}

#endif  // UNICODE_FILTER_H

