#pragma once

#include <AK/Types.h>

typedef dword RGBA32;

inline constexpr dword make_rgb(byte r, byte g, byte b)
{
    return ((r << 16) | (g << 8) | b);
}

class Color {
public:
    enum NamedColor {
        Black,
        White,
        Red,
        Green,
        Blue,
        Yellow,
        Magenta,
        DarkGray,
        MidGray,
        LightGray,
    };

    Color() { }
    Color(NamedColor);
    Color(byte r, byte g, byte b) : m_value((r << 16) | (g << 8) | b) { }
    Color(RGBA32 rgba) : m_value(rgba) { }

    int red() const { return (m_value >> 16) & 0xff; }
    int green() const { return (m_value >> 8) & 0xff; }
    int blue() const { return m_value & 0xff; }

    RGBA32 value() const { return m_value; }

private:
    RGBA32 m_value { 0 };
};