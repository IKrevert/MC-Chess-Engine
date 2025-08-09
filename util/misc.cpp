#include "misc.h"

#include <chrono>
#include <cstdio>
#include <cstring>   // std::strchr, std::strncmp, std::strlen

#ifdef _WIN32
  #define NOMINMAX
  #include <windows.h>
#else
  #include <sys/select.h>
  #include <sys/time.h>
  #include <unistd.h>
#endif


int GetTimeMs() {
    using namespace std::chrono;
    // steady_clock is monotonic; good for measuring elapsed time
    const auto now = steady_clock::now().time_since_epoch();
    const auto ms  = duration_cast<milliseconds>(now).count();
    // Match legacy signature: int (wrap after ~24 days is fine for engines)
    return static_cast<int>(ms);
}

int InputWaiting() {
#ifdef _WIN32
    // Handle both console and pipe input (similar idea to original)
    static bool initialized = false;
    static HANDLE hIn = nullptr;
    static bool isPipe = false;

    if (!initialized) {
        initialized = true;
        hIn = GetStdHandle(STD_INPUT_HANDLE);

        DWORD mode = 0;
        isPipe = (hIn == nullptr) || !GetConsoleMode(hIn, &mode);
        if (!isPipe && hIn) {
            // Disable mouse/window input so keypresses register as events
            SetConsoleMode(hIn, mode & ~(ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT));
            FlushConsoleInputBuffer(hIn);
        }
    }

    if (isPipe) {
        DWORD bytesAvailable = 0;
        if (!PeekNamedPipe(hIn, nullptr, 0, nullptr, &bytesAvailable, nullptr)) {
            // If PeekNamedPipe fails, assume something is there (like closed pipe)
            return 1;
        }
        return static_cast<int>(bytesAvailable);
    } else {
        // Console: check number of pending events
        DWORD events = 0;
        if (!GetNumberOfConsoleInputEvents(hIn, &events)) return 0;
        // The console always maintains at least one event; consider >1 as real input
        return (events <= 1) ? 0 : static_cast<int>(events);
    }
#else
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 0;

    const int nfds = STDIN_FILENO + 1;
    const int ret  = select(nfds, &readfds, nullptr, nullptr, &tv);
    if (ret <= 0) return 0;
    return FD_ISSET(STDIN_FILENO, &readfds) ? 1 : 0;
#endif
}


void ReadInput(S_SEARCHINFO* info) {
    if (!info) return;

    if (!InputWaiting()) return;

    // Mirror original behavior: mark stopped when we detect input
#ifdef TRUE
    info->stopped = TRUE;
#else
    info->stopped = true;
#endif

#ifdef _WIN32
    static HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

    // Read one line if available
    char buffer[256] = {0};
    DWORD readBytes  = 0;

    // Use non-blocking Peek then ReadConsole/ReadFile depending on mode
    DWORD bytesAvail = 0;
    if (PeekNamedPipe(hIn, nullptr, 0, nullptr, &bytesAvail, nullptr) && bytesAvail > 0) {
        if (ReadFile(hIn, buffer, sizeof(buffer) - 1, &readBytes, nullptr)) {
            // Normalize to line
            if (char* eol = std::strchr(buffer, '\n')) *eol = '\0';
        }
    } else {
        // Fall back to fgets on stdin (console scenario)
        if (std::fgets(buffer, sizeof(buffer), stdin)) {
            if (char* eol = std::strchr(buffer, '\n')) *eol = '\0';
        }
    }

    if (std::strlen(buffer) > 0) {
        if (!std::strncmp(buffer, "quit", 4)) {
#ifdef TRUE
            info->quit = TRUE;
#else
            info->quit = true;
#endif
        }
    }
#else
    // POSIX: safe to read because InputWaiting() confirmed data is there
    char buffer[256] = {0};
    ssize_t n = ::read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        if (char* eol = std::strchr(buffer, '\n')) *eol = '\0';

        if (std::strlen(buffer) > 0) {
            if (!std::strncmp(buffer, "quit", 4)) {
#ifdef TRUE
                info->quit = TRUE;
#else
                info->quit = true;
#endif
            }
        }
    }
#endif
}
