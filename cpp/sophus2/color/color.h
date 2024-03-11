// Copyright (c) 2011, Hauke Strasdat
// Copyright (c) 2012, Steven Lovegrove
// Copyright (c) 2021, farm-ng, inc.
//
// Use of this source code is governed by an MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT.

#pragma once

#include <Eigen/Core>
#include <sophus2/common/enum.h>

#include <cmath>
#include <limits>
#include <stdexcept>

namespace sophus2 {

SOPHUS_ENUM(
    ColorHue,
    (red,
     orange,
     yellow,
     chartreuse,
     green,
     turquoise,
     cyan,
     azure,
     blue,
     violet,
     magenta,
     pink));

SOPHUS_ENUM(ColorBrightness, (bright, medium, dark));
SOPHUS_ENUM(ColorSaturation, (normal, neon));

struct Color {
  // clang-format off
  constexpr Color (float r, float g, float b, float a = 1.0)
      // clang-format on
      : r(r), g(g), b(b), a(a) {}

  // clang-format off
  constexpr Color (float a = 1.0)
      // clang-format on
      : Color(0.5, 0.5, 0.5, a) {}

  static Color constexpr from(
      ColorHue hue,
      ColorBrightness brightness = ColorBrightness::medium,
      ColorSaturation sat = ColorSaturation::normal,
      float a = 1.0) {
    float factor = 1.f;
    float zero = 0.f;
    float half = 0.5f;
    switch (brightness) {
      case ColorBrightness::bright: {
        break;
      }
      case ColorBrightness::medium: {
        factor = 0.75f;
        break;
      }
      case ColorBrightness::dark: {
        factor = 0.5f;
        break;
      }
    }
    switch (sat) {
      case ColorSaturation::normal: {
        break;
      }
      case ColorSaturation::neon: {
        zero = 0.5f;
        half = 0.75f;
        break;
      }
    }
    switch (hue) {
      case ColorHue::red: {
        return Color(1.0, zero, zero, a) * factor;
        break;
      }
      case ColorHue::orange: {
        return Color(1.0, half, zero, a) * factor;
        break;
      }
      case ColorHue::yellow: {
        return Color(1.0, 1.0, zero, a) * factor;
        break;
      }
      case ColorHue::chartreuse: {
        return Color(half, 1.0, zero, a) * factor;
        break;
      }
      case ColorHue::green: {
        return Color(zero, 1.0, zero, a) * factor;
        break;
      }
      case ColorHue::turquoise: {
        return Color(zero, 1.0, half, a) * factor;
        break;
      }
      case ColorHue::cyan: {
        return Color(zero, 1.0, 1.0, a) * factor;
        break;
      }
      case ColorHue::azure: {
        return Color(zero, half, 1.0, a) * factor;
        break;
      }
      case ColorHue::blue: {
        return Color(zero, zero, 1.0, a) * factor;
        break;
      }
      case ColorHue::violet: {
        return Color(half, zero, 1.0, a) * factor;
        break;
      }
      case ColorHue::magenta: {
        return Color(1.0, zero, 1.0, a) * factor;
        break;
      }
      case ColorHue::pink: {
        return Color(1.0, zero, half, a) * factor;
        break;
      }
    }
    FARM_IMPL_ABORT();
  }

  // bright colors
  static Color constexpr brightRed(float a = 1.0) {
    return Color::from(
        ColorHue::red, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightGreen(float a = 1.0) {
    return Color::from(
        ColorHue::green, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightBlue(float a = 1.0) {
    return Color::from(
        ColorHue::blue, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightYellow(float a = 1.0) {
    return Color::from(
        ColorHue::yellow, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightMagenta(float a = 1.0) {
    return Color::from(
        ColorHue::magenta, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightCyan(float a = 1.0) {
    return Color::from(
        ColorHue::cyan, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightOrange(float a = 1.0) {
    return Color::from(
        ColorHue::orange, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightTurquoise(float a = 1.0) {
    return Color::from(
        ColorHue::turquoise,
        ColorBrightness::bright,
        ColorSaturation::normal,
        a);
  }
  static Color constexpr brightViolet(float a = 1.0) {
    return Color::from(
        ColorHue::violet, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightPink(float a = 1.0) {
    return Color::from(
        ColorHue::pink, ColorBrightness::bright, ColorSaturation::normal, a);
  }
  static Color constexpr brightChartreuse(float a = 1.0) {
    return Color::from(
        ColorHue::chartreuse,
        ColorBrightness::bright,
        ColorSaturation::normal,
        a);
  }
  static Color constexpr brightAzure(float a = 1.0) {
    return Color::from(
        ColorHue::azure, ColorBrightness::bright, ColorSaturation::normal, a);
  }

  // colors
  static Color constexpr red(float a = 1.0) {
    return Color::from(
        ColorHue::red, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr green(float a = 1.0) {
    return Color::from(
        ColorHue::green, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr blue(float a = 1.0) {
    return Color::from(
        ColorHue::blue, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr yellow(float a = 1.0) {
    return Color::from(
        ColorHue::yellow, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr magenta(float a = 1.0) {
    return Color::from(
        ColorHue::magenta, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr cyan(float a = 1.0) {
    return Color::from(
        ColorHue::cyan, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr orange(float a = 1.0) {
    return Color::from(
        ColorHue::orange, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr turquoise(float a = 1.0) {
    return Color::from(
        ColorHue::turquoise,
        ColorBrightness::medium,
        ColorSaturation::normal,
        a);
  }
  static Color constexpr violet(float a = 1.0) {
    return Color::from(
        ColorHue::violet, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr pink(float a = 1.0) {
    return Color::from(
        ColorHue::pink, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  static Color constexpr chartreuse(float a = 1.0) {
    return Color::from(
        ColorHue::chartreuse,
        ColorBrightness::medium,
        ColorSaturation::normal,
        a);
  }
  static Color constexpr azure(float a = 1.0) {
    return Color::from(
        ColorHue::azure, ColorBrightness::medium, ColorSaturation::normal, a);
  }
  // dark colors
  static Color constexpr darkRed(float a = 1.0) {
    return Color::from(
        ColorHue::red, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkGreen(float a = 1.0) {
    return Color::from(
        ColorHue::green, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkBlue(float a = 1.0) {
    return Color::from(
        ColorHue::blue, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkYellow(float a = 1.0) {
    return Color::from(
        ColorHue::yellow, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkMagenta(float a = 1.0) {
    return Color::from(
        ColorHue::magenta, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkCyan(float a = 1.0) {
    return Color::from(
        ColorHue::cyan, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkOrange(float a = 1.0) {
    return Color::from(
        ColorHue::orange, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkTurquoise(float a = 1.0) {
    return Color::from(
        ColorHue::turquoise, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkViolet(float a = 1.0) {
    return Color::from(
        ColorHue::violet, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkPink(float a = 1.0) {
    return Color::from(
        ColorHue::pink, ColorBrightness::dark, ColorSaturation::normal, a);
  }
  static Color constexpr darkChartreuse(float a = 1.0) {
    return Color::from(
        ColorHue::chartreuse,
        ColorBrightness::dark,
        ColorSaturation::normal,
        a);
  }
  static Color constexpr darkAzure(float a = 1.0) {
    return Color::from(
        ColorHue::azure, ColorBrightness::dark, ColorSaturation::normal, a);
  }

  // neon colors
  static Color constexpr neonRed(float a = 1.0) {
    return Color::from(
        ColorHue::red, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonGreen(float a = 1.0) {
    return Color::from(
        ColorHue::green, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonBlue(float a = 1.0) {
    return Color::from(
        ColorHue::blue, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonYellow(float a = 1.0) {
    return Color::from(
        ColorHue::yellow, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonMagenta(float a = 1.0) {
    return Color::from(
        ColorHue::magenta, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonCyan(float a = 1.0) {
    return Color::from(
        ColorHue::cyan, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonOrange(float a = 1.0) {
    return Color::from(
        ColorHue::orange, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonTurquoise(float a = 1.0) {
    return Color::from(
        ColorHue::turquoise, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonViolet(float a = 1.0) {
    return Color::from(
        ColorHue::violet, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonPink(float a = 1.0) {
    return Color::from(
        ColorHue::pink, ColorBrightness::bright, ColorSaturation::neon, a);
  }
  static Color constexpr neonChartreuse(float a = 1.0) {
    return Color::from(
        ColorHue::chartreuse,
        ColorBrightness::bright,
        ColorSaturation::neon,
        a);
  }
  static Color constexpr neonAzure(float a = 1.0) {
    return Color::from(
        ColorHue::azure, ColorBrightness::bright, ColorSaturation::neon, a);
  }

  // shades of gray
  static Color constexpr black(float a = 1.0) {
    return Color(0.0, 0.0, 0.0, a);
  }
  static Color constexpr white(float a = 1.0) {
    return Color(1.0, 1.0, 1.0, a);
  }
  static Color constexpr brightGray(float a = 1.0) {
    return Color(0.75, 0.75, 0.75, a);
  }
  static Color constexpr darkGray(float a = 1.0) {
    return Color(0.25, 0.25, 0.25, a);
  }
  static Color constexpr gray(float a = 1.0) { return Color(0.5, 0.5, 0.5, a); }

  Color constexpr operator*(double scale) {
    return Color(scale * r, scale * g, scale * b, a);
  }

  float r;
  float g;
  float b;
  float a;
};

static_assert(sizeof(Color) == 4 * sizeof(float));

}  // namespace sophus2
