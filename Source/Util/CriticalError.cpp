#include <Util/CriticalError.hpp>
#include <portable_file_dialogs.h>
#include <thread>
#include <Medae.hpp>

void throwCriticalError(const std::string &message) {
    pfd::message criticalErrorDialog{"Critical error", message, pfd::choice::ok,
                                     pfd::icon::error};
    std::thread shotdown([]() { Medae::getInstance().emergencyShotDown(); });
    while (!criticalErrorDialog.ready(500)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    criticalErrorDialog.kill();
    shotdown.join();
    exit(1);
}