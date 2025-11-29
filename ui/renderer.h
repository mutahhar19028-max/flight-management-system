#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <string_view>
#include <vector>
#include <chrono>
#include <thread>
#include <format>

// ═══════════════════════════════════════════════════════════
//  THEME CONFIGURATION - Easy to customize
// ═══════════════════════════════════════════════════════════
namespace Theme {
    // Primary colors
    constexpr const char* PRIMARY = "\033[96m";      // Bright Cyan
    constexpr const char* SECONDARY = "\033[94m";    // Bright Blue
    constexpr const char* ACCENT = "\033[95m";       // Bright Magenta
    
    // Status colors
    constexpr const char* SUCCESS = "\033[92m";      // Bright Green
    constexpr const char* ERROR = "\033[91m";        // Bright Red
    constexpr const char* WARNING = "\033[93m";      // Bright Yellow
    constexpr const char* INFO = "\033[94m";         // Bright Blue
    
    // Text styles
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* DIM = "\033[2m";
    constexpr const char* UNDERLINE = "\033[4m";
    constexpr const char* RESET = "\033[0m";
    
    // Animation speeds (milliseconds)
    constexpr int TYPING_SPEED = 8;
    constexpr int LINE_DELAY = 25;
    constexpr int FADE_DELAY = 35;
    constexpr int SPINNER_SPEED = 80;
}

// ═══════════════════════════════════════════════════════════
//  UI RENDERER CLASS
// ═══════════════════════════════════════════════════════════
class UIRenderer {
public:
    // Screen management
    static void clearScreen();
    static void moveCursor(int row, int col);
    static void hideCursor();
    static void showCursor();
    
    // Box drawing
    static void drawBoxTop(int width, std::string_view title = "");
    static void drawBoxMiddle(int width, std::string_view content = "", bool centered = false);
    static void drawBoxSeparator(int width);
    static void drawBoxBottom(int width);
    static void drawFullBox(int width, std::string_view title, const std::vector<std::string>& lines);
    
    // Text effects
    static void typeText(std::string_view text, int delay_ms = Theme::TYPING_SPEED);
    static void fadeInLine(std::string_view text, int delay_ms = Theme::LINE_DELAY);
    static void printCentered(std::string_view text, int width = 80);
    
    // Status messages
    static void successMessage(std::string_view message);
    static void errorMessage(std::string_view message);
    static void warningMessage(std::string_view message);
    static void infoMessage(std::string_view message);
    
    // Loading animations
    static void showSpinner(std::string_view message, int duration_ms = 1000);
    static void progressBar(int percentage, int width = 40);
    
    // Menu helpers
    static void printMenuItem(int number, std::string_view icon, std::string_view text);
    static void printSeparator(int width = 70, char fillChar = '─');
    
    // Utilities
    static void pause(std::string_view message = "Press Enter to continue...");
    static void sleep(int milliseconds);
    
private:
    static constexpr const char* SPINNER_FRAMES[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    static constexpr int SPINNER_FRAME_COUNT = 10;
};

#endif // RENDERER_H