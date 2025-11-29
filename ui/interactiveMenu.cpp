#include "interactiveMenu.h"
#include "renderer.h"
#include <iostream>
#include <format>

InteractiveMenu::InteractiveMenu(std::string_view title, const std::vector<MenuItem>& items)
    : _title(title), _items(items) {}

int InteractiveMenu::show() {
    _enableRawMode();
    UIRenderer::hideCursor();
    
    bool running = true;
    int key;
    
    while (running) {
        _render();
        key = _getKey();
        
        switch(key) {
            case KEY_UP:   if(_currentSelection > 0) _currentSelection--; break;
            case KEY_DOWN:  if(_currentSelection < static_cast<int>(_items.size()) - 1) _currentSelection++; break;
            case KEY_ENTER: running = false; break;
            case KEY_ESC:   return -1;
        }
    }
    
    _disableRawMode();
    UIRenderer::showCursor();
    return _items[_currentSelection].value;
}

void InteractiveMenu::_render() {
    // Move cursor to top
    std::cout << "\033[2J\033[H";
    
    std::cout << "\n";
    
    // Draw top border
    UIRenderer::drawBoxTop(_width, _title);
    UIRenderer::drawBoxMiddle(_width);
    
    // Draw menu items
    for (size_t i = 0; i < _items.size(); ++i) {
        _drawMenuItem(_items[i], i == static_cast<size_t>(_currentSelection));
    }
    
    // Draw bottom border
    UIRenderer::drawBoxMiddle(_width);
    UIRenderer::drawBoxBottom(_width);
    
    // Instructions
    std::cout << "\n" << Theme::DIM 
              << "  ↑/↓ Navigate  │  Enter Select  │  ESC Back" 
              << Theme::RESET << "\n" << std::flush;
}

void InteractiveMenu::_drawMenuItem(const MenuItem& item, bool highlighted) {
    std::cout << _borderColor << "║  " << Theme::RESET;
    
    if (highlighted) {
        // Highlighted item
        std::cout << _highlightColor << "▶ " << item.icon << "  " 
                  << item.value << ". " << item.text;
        
        // Calculate padding
        int contentLength = 2 + item.icon.length() + 2 + 
                           std::to_string(item.value).length() + 2 + item.text.length();
        int padding = _width - 6 - contentLength;
        
        for (int i = 0; i < padding; ++i) std::cout << " ";
        std::cout << Theme::RESET;
    } else {
        // Normal item
        std::cout << "  " << Theme::ACCENT << item.icon << "  " << Theme::RESET
                  << Theme::SECONDARY << item.value << ". " << Theme::RESET 
                  << item.text;
        
        // Calculate padding
        int contentLength = 2 + item.icon.length() + 2 + 
                           std::to_string(item.value).length() + 2 + item.text.length();
        int padding = _width - 6 - contentLength;
        
        for (int i = 0; i < padding; ++i) std::cout << " ";
    }
    
    std::cout << "  " << _borderColor << "║" << Theme::RESET << "\n";
}

int InteractiveMenu::_getKey() {
#ifdef _WIN32
int ch = _getch();
if (ch == 0 || ch == 224) ch = _getch();
if (ch == 72) return KEY_UP;
if (ch == 80) return KEY_DOWN;
if (ch == 13) return KEY_ENTER;
return ch == 27 ? KEY_ESC : 0;
#else
int first = getchar();
if (first == 27) {
    if (getchar() == 91) {
        int c = getchar();
        if (c == 65) return KEY_UP;
        if (c == 66) return KEY_DOWN;
    }
    return KEY_ESC;
}
return first == 10 ? KEY_ENTER : 0;
#endif
}

void InteractiveMenu::_enableRawMode() {
#ifdef _WIN32
    // Windows: _getch() already handles raw input
#else
    // Linux/Mac: Disable canonical mode and echo
    tcgetattr(STDIN_FILENO, &_originalTermios);
    struct termios raw = _originalTermios;
    
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
#endif
}

void InteractiveMenu::_disableRawMode() {
#ifdef _WIN32
    // Windows: Nothing to restore
#else
    // Linux/Mac: Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &_originalTermios);
#endif
}