#ifndef UTF8_UNICODE_H
#define UTF8_UNICODE_H

bool utf8_check_bytes(const char *first, size_t count) {
  for (size_t i = 1; i <= count; ++i) {
    unsigned char c = *(first + i);
    if (c >> 6 != 2) {
      return false;
    }
  }
  return true;
}

size_t utf8_get_size(const char *str) {
  unsigned char c = *str;

  if (c >> 7 == 0x0) {
    // single byte
    return 1;
  } else if (c >> 5 == 0x06) {
    // double bytes
    if (utf8_check_bytes(str, 1) == false) {
      return -1;
    }
    return 2;
  } else if (c >> 4 == 0x0e) {
    // three bytes
    if (utf8_check_bytes(str, 2) == false) {
      return -1;
    }
    return 3;
  } else if (c >> 3 == 0x1e) {
    // four bytes
    if (utf8_check_bytes(str, 3) == false) {
      return -1;
    }
    return 4;
  } else {
    return -1;
  }
}

int utf8_convert_unicode(const char *str, size_t size) {
  if (size == 1) {
    return str[0] & 0x7F;
  } else if (size == 2) {
    return ((str[0] & 0x1F) << 6) + (str[1] & 0x3F);
  } else if (size == 3) {
    return ((str[0] & 0x0F) << 12) + ((str[1] & 0x3F) << 6) +
      (str[2] & 0x3F);
  } else if (size == 4) {
    return ((str[0] & 0x07) << 18) + ((str[1] & 0x3F) << 12) +
      ((str[2] & 0x3F) << 6) + (str[3] & 0x3F);
  } else {
    return -1;
  }
}

#endif  // UTF8_UNICODE_H

