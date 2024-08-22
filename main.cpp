#include <application.hpp>

int main() {
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(Config::window.width, Config::window.height, Config::title.data());
    rlImGuiSetup(true);
    Application app;
    app.run();
    rlImGuiShutdown();
    CloseWindow();
    return 0;
}
