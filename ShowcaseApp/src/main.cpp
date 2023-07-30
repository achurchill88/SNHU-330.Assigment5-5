#include <application.h>

int main(int argc, char** argv) {
    // Create an instance of the Application with the specified window title, width, and height
    Application app{ "Andy Churchill", 800, 600 };

    // Run the application
    app.Run();

    // Return 0 to indicate successful execution
    return 0;
}
