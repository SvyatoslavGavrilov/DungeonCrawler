// WindowTransparency.h
#pragma once
#ifndef WINDOW_TRANSPARENCY_H
#define WINDOW_TRANSPARENCY_H

#include <SFML/Graphics.hpp>

#ifdef _WIN32
#include <windows.h>

inline void makeWindowBackgroundTransparent(sf::RenderWindow& window) {
    HWND hwnd = window.getSystemHandle();  // Get the Win32 handle of the SFML window

    // Enable window transparency
    long windowStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    windowStyle |= WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, windowStyle);

    // Set the transparency color key to RGB(0, 0, 0) (black in this example)
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
}
#endif

#endif // WINDOW_TRANSPARENCY_H
