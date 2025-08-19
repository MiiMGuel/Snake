#include <iostream>
#include <stdexcept>
#include <cstdlib>

class App {
public:
    virtual void run() { };
}; 

static App* app;

App* main_app(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    try {
        App* app = main_app(argc, argv);
        app->run();
        free(app);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } return EXIT_SUCCESS;
}