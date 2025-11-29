#ifndef INTERACTIVE_MENU_H
#define INTERACTIVE_MENU_H

#include <string>
#include <vector>
#include <string_view>

static constexpr int KEY_UP = 1;
static constexpr int KEY_DOWN = 2;
static constexpr int KEY_ENTER = 3;
static constexpr int KEY_ESC = 4;

#ifdef _WIN32
    #include <conio.h>
    #define IS_WINDOWS 1
#else
    #include <termios.h>
    #include <unistd.h>
    #define IS_WINDOWS 0
#endif

class InteractiveMenu {
public:
    struct MenuItem {
        std::string icon;
        std::string text;
        int value;
        
        MenuItem(std::string_view i, std::string_view t, int v)
            : icon(i), text(t), value(v) {}
    };
    
    InteractiveMenu(std::string_view title, const std::vector<MenuItem>& items);
    
    int show();
    
    void setWidth(int w) { _width = w; }
    void setBorderColor(std::string_view color) { _borderColor = color; }
    void setHighlightColor(std::string_view color) { _highlightColor = color; }
    
private:
    std::string _title;
    std::vector<MenuItem> _items;
    int _currentSelection = 0;
    int _width = 70;
    std::string _borderColor = "\033[96m";      // cyan
    std::string _highlightColor = "\033[97;46m"; // white on cyan BG
    std::string _normalColor = "\033[0m";
    
    void _render();
    void _drawMenuItem(const MenuItem& item, bool highlighted);
    
    int _getKey();
    void _enableRawMode();
    void _disableRawMode();
    
#ifndef _WIN32
    struct termios _originalTermios;
#endif
};

#endif