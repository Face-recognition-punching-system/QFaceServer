#include "server/http_server.h"

int main(int argc, char** argv) {
    HttpApplication app;
    return app.run(argc, argv);
}
