#include "bios.h"

void initThreads() {
    moboThread = CreateThread(
        NULL,                   // Default security attributes
        0,                      // Default stack size
        mobo,                   // Function to be executed by the thread
        NULL,                   // Argument to be passed to the thread function
        0,                      // Creation flags (0 for normal creation)
        NULL                    // Thread ID (optional)
    );
    core0Thread = CreateThread(
        NULL,
        0,
        core0,
        NULL,
        0,
        NULL
    );
    core1Thread = CreateThread(
        NULL,
        0,
        core1,
        NULL,
        0,
        NULL
    );
    core2Thread = CreateThread(
        NULL,
        0,
        core2,
        NULL,
        0,
        NULL
    );
    core3Thread = CreateThread(
        NULL,
        0,
        core3,
        NULL,
        0,
        NULL
    );
    core4Thread = CreateThread(
        NULL,
        0,
        core4,
        NULL,
        0,
        NULL
    );
    core5Thread = CreateThread(
        NULL,
        0,
        core5,
        NULL,
        0,
        NULL
    );
    core6Thread = CreateThread(
        NULL,
        0,
        core6,
        NULL,
        0,
        NULL
    );
    core7Thread = CreateThread(
        NULL,
        0,
        core7,
        NULL,
        0,
        NULL
    );
}

int main(int argc, char** argv) {
    initThreads();

	return 0;
}