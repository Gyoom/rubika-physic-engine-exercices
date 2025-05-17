#include "Application.h"

int main(int argc, char *args[]) {
    Application app = Application();

    while (app.reload) {
		app.reload = false;
        app.Setup();

        while (app.IsRunning()) {
            app.Input();
            app.Update();
            app.Render();
        }

        app.Destroy();
    }
    Graphics::CloseWindow();

    return 0;
}