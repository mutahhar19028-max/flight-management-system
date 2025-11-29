#ifndef INTERACTIVE_MENU_H
#define INTERACTIVE_MENU_H

#include <string>
#include <vector>
#include <string_view>

static constexpr int KEY_UP = 1;
static constexpr int KEY_DOWN = 2;
static constexpr int KEY_ENTER = 3;
static constexpr int KEY_ESC = 4;

// Platform detection
#ifdef _WIN32
    #include <conio.h>
    #define IS_WINDOWS 1
#else
    #include <termios.h>
    #include <unistd.h>
    #define IS_WINDOWS 0
#endif

// ═══════════════════════════════════════════════════════════
//  INTERACTIVE MENU - Arrow Key Navigation
// ═══════════════════════════════════════════════════════════
class InteractiveMenu {
public:
    struct MenuItem {
        std::string icon;
        std::string text;
        int value;
        
        MenuItem(std::string_view i, std::string_view t, int v)
            : icon(i), text(t), value(v) {}
    };
    
    // Constructor
    InteractiveMenu(std::string_view title, const std::vector<MenuItem>& items);
    
    // Main display & selection
    int show();
    
    // Configuration
    void setWidth(int w) { _width = w; }
    void setBorderColor(std::string_view color) { _borderColor = color; }
    void setHighlightColor(std::string_view color) { _highlightColor = color; }
    
private:
    std::string _title;
    std::vector<MenuItem> _items;
    int _currentSelection = 0;
    int _width = 70;
    std::string _borderColor = "\033[96m";      // Cyan
    std::string _highlightColor = "\033[97;46m"; // White on Cyan BG
    std::string _normalColor = "\033[0m";
    
    // Display methods
    void _render();
    void _drawMenuItem(const MenuItem& item, bool highlighted);
    
    // Input handling
    int _getKey();
    void _enableRawMode();
    void _disableRawMode();
    
#ifndef _WIN32
    struct termios _originalTermios;
#endif
};

#endif // INTERACTIVE_MENU_H