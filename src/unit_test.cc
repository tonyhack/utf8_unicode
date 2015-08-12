#include <stdio.h>
#include <string.h>

#include "unicode_filter.h"
#include "utf8_unicode.h"

void test_utf8_unicode() {
  struct utf8_unicode_pair {
    char utf8_str[4];
    int unicode_value;
  };

  struct utf8_unicode_pair cases[] = {
    { "】", 0x3011 },
    { "《", 0x300a },
    { "Ａ", 0xff21 },
    { "％", 0xff05 },
    { "＠", 0xff20 },
    { "︻", 0xfe3b },
    { "我", 0x6211 },
    { "中", 0x4e2d },
    { "g",  0x0067 },
  };

  size_t case_num = sizeof(cases) / sizeof(struct utf8_unicode_pair);
  size_t pass_case_num = 0;

  for (size_t i = 0; i < case_num; ++i) {
    size_t utf8_size = utf8_get_size(cases[i].utf8_str);

    int unicode_value = utf8_convert_unicode(cases[i].utf8_str, utf8_size);

    if (unicode_value == cases[i].unicode_value) {
      ++pass_case_num;
    } else {
      printf("test_utf8_unicode: utf8_str(%s) unicode_value(0x%x) failed(0x%x)\n.",
          cases[i].utf8_str, cases[i].unicode_value, unicode_value);
    }
  }

  if (case_num > pass_case_num) {
    printf("test_utf8_unicode: failed num[%ld].\n", case_num - pass_case_num);
  } else {
    printf("test_utf8_unicode: pass all test.\n");
  }
}

void test_unicode_filter() {
  struct unicode_range test_range[] = {
    {0x0021, 0x007E},   // 英文(不包括空格)
    {0x2E80, 0x2EFF},   // CJK部首补充
    {0x3000, 0x303F},   // CJK标点符号
    {0x31C0, 0x31EF},   // CJK笔划
    {0x4e00, 0x9fa5},   // 中文
    {0xff21, 0xff5a},   // 全角英文
    {0xff10, 0xff19},   // 全角数字
    {0xff01, 0xff09},   // 美式键盘上shift + 从1到9键上的特殊符号
    {0xFF01, 0xFF5e},   // 全角ASCII、全角中英文标点
    {0xFFE5, 0xFFE5},   // ￥
  };

  struct unicode_filter_pair {
    char str[64];
    int hit;
  };

  struct unicode_filter_pair cases[] = {
    { "1234567890!@#$%^&*()_+-=", 0},
    { "！@#￥%&*（）", 0},
    { "我人有的和主产不为这《》？、", 0},
    { "·，。、【】{}《》？~！、|", -1},
    { "】《Ａ％＠︻我中g", -1},
    { "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", 0},
    { "亻艹", 0},
    { "a b", -1},
  };

  size_t range_size = sizeof(test_range) / sizeof(struct unicode_range);
  size_t case_num = sizeof(cases) / sizeof(struct unicode_filter_pair);
  size_t pass_case_num = 0;

  for (size_t i = 0; i < case_num; ++i) {
    int result_value = unicode_filter(cases[i].str, strlen(cases[i].str),
        test_range, range_size);
    if (result_value == cases[i].hit) {
      ++pass_case_num;
    } else {
      printf("test_unicode_filter: case(%s) failed.", cases[i].str);
    }
  }

  if (case_num > pass_case_num) {
    printf("test_unicode_filter: failed num[%ld].\n", case_num - pass_case_num);
  } else {
    printf("test_unicode_filter: pass all test.\n");
  }
}

int main() {
  printf("--------------------\n");
  test_utf8_unicode();
  printf("--------------------\n");
  test_unicode_filter();
  printf("--------------------\n");

  return 0;
}

