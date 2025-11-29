#include "renderer.h"
#include <iostream>
#include <iomanip>

void UIRenderer::clearScreen() {
    std::cout << "\033[2J\033[H";
}

void UIRenderer::moveCursor(int row, int col) {
    std::cout << std::format("\033[{};{}H", row, col);
}

void UIRenderer::hideCursor() {
    std::cout << "\033[?25l" << std::flush;
}

void UIRenderer::showCursor() {
    std::cout << "\033[?25h" << std::flush;
}

void UIRenderer::drawBoxTop(int width, std::string_view title) {
    std::cout << Theme::PRIMARY << "╔";
    
    if (title.empty()) {
        for (int i = 0; i < width - 2; ++i) std::cout << "═";
    } else {
        int padding = width - title.length() - 6;
        int leftPad = padding / 2;
        int rightPad = padding - leftPad;
        
        for (int i = 0; i < leftPad; ++i) std::cout << "═";
        std::cout << "═ " << Theme::BOLD << title << Theme::RESET << Theme::PRIMARY << " ═";
        for (int i = 0; i < rightPad; ++i) std::cout << "═";
    }
    
    std::cout << "╗" << Theme::RESET << "\n";
}

void UIRenderer::drawBoxMiddle(int width, std::string_view content, bool centered) {
    std::cout << Theme::PRIMARY << "║" << Theme::RESET;
    
    if (content.empty()) {
        for (int i = 0; i < width - 2; ++i) std::cout << " ";
    } else {
        if (centered) {
            int padding = width - 2 - content.length();
            int leftPad = padding / 2;
            int rightPad = padding - leftPad;
            for (int i = 0; i < leftPad; ++i) std::cout << " ";
            std::cout << content;
            for (int i = 0; i < rightPad; ++i) std::cout << " ";
        } else {
            std::cout << content;
            for (int i = content.length(); i < width - 2; ++i) std::cout << " ";
        }
    }
    
    std::cout << Theme::PRIMARY << "║" << Theme::RESET << "\n";
}

void UIRenderer::drawBoxSeparator(int width) {
    std::cout << Theme::PRIMARY << "╠";
    for (int i = 0; i < width - 2; ++i) std::cout << "═";
    std::cout << "╣" << Theme::RESET << "\n";
}

void UIRenderer::drawBoxBottom(int width) {
    std::cout << Theme::PRIMARY << "╚";
    for (int i = 0; i < width - 2; ++i) std::cout << "═";
    std::cout << "╝" << Theme::RESET << "\n";
}

void UIRenderer::drawFullBox(int width, std::string_view title, const std::vector<std::string>& lines) {
    drawBoxTop(width, title);
    for (const auto& line : lines) {
        drawBoxMiddle(width, line);
    }
    drawBoxBottom(width);
}

void UIRenderer::typeText(std::string_view text, int delay_ms) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
    }
    std::cout << "\n";
}

void UIRenderer::fadeInLine(std::string_view text, int delay_ms) {
    std::cout << text << "\n" << std::flush;
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
}

void UIRenderer::printCentered(std::string_view text, int width) {
    int padding = (width - text.length()) / 2;
    for (int i = 0; i < padding; ++i) std::cout << " ";
    std::cout << text << "\n";
}

void UIRenderer::successMessage(std::string_view message) {
    std::cout << Theme::SUCCESS << "✓ " << message << Theme::RESET << "\n";
    sleep(Theme::LINE_DELAY);
}

void UIRenderer::errorMessage(std::string_view message) {
    std::cout << Theme::ERROR << "✗ " << message << Theme::RESET << "\n";
    sleep(Theme::LINE_DELAY);
}

void UIRenderer::warningMessage(std::string_view message) {
    std::cout << Theme::WARNING << "⚠ " << message << Theme::RESET << "\n";
    sleep(Theme::LINE_DELAY);
}

void UIRenderer::infoMessage(std::string_view message) {
    std::cout << Theme::INFO << "ℹ " << message << Theme::RESET << "\n";
    sleep(Theme::LINE_DELAY);
}

void UIRenderer::showSpinner(std::string_view message, int duration_ms) {
    hideCursor();
    int frames = duration_ms / Theme::SPINNER_SPEED;
    
    for (int i = 0; i < frames; ++i) {
        std::cout << "\r" << Theme::PRIMARY << SPINNER_FRAMES[i % SPINNER_FRAME_COUNT] 
                  << " " << Theme::RESET << message << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(Theme::SPINNER_SPEED));
    }
    
    std::cout << "\r";
    for (size_t i = 0; i < message.length() + 10; ++i) std::cout << " ";
    std::cout << "\r" << std::flush;
    showCursor();
}

void UIRenderer::progressBar(int percentage, int width) {
    int filled = (percentage * width) / 100;
    
    std::cout << "[" << Theme::SUCCESS;
    for (int i = 0; i < filled; ++i) std::cout << "█";
    std::cout << Theme::DIM;
    for (int i = filled; i < width; ++i) std::cout << "░";
    std::cout << Theme::RESET << "] " << std::format("{:3}%", percentage) << "\n";
}

void UIRenderer::printMenuItem(int number, std::string_view icon, std::string_view text) {
    std::cout << Theme::PRIMARY << "║  " << Theme::BOLD << Theme::ACCENT 
              << icon << "  " << Theme::RESET << Theme::SECONDARY 
              << number << ". " << Theme::RESET << text;
    
    int padding = 60 - text.length();
    for (int i = 0; i < padding; ++i) std::cout << " ";
    
    std::cout << Theme::PRIMARY << "║" << Theme::RESET << "\n";
    sleep(Theme::LINE_DELAY);
}

void UIRenderer::printSeparator(int width, char fillChar) {
    std::cout << Theme::DIM;
    for (int i = 0; i < width; ++i) std::cout << fillChar;
    std::cout << Theme::RESET << "\n";
}

void UIRenderer::pause(std::string_view message) {
    std::cout << "\n" << Theme::DIM << message << Theme::RESET << std::flush;
    std::cin.get();
}

void UIRenderer::sleep(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}