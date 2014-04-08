#ifndef BONAVISTA_LOGGING_COLOR_PRINTF_H_
#define BONAVISTA_LOGGING_COLOR_PRINTF_H_

enum TextColor {
  TEXT_COLOR_RED,
  TEXT_COLOR_GREEN
};

void ColorPrintf(TextColor color, const char* format, ...);

#endif
