#include "Base/Application.h"
#include "Nodes/Group.h"

int main(int argc, char** argv)
{
    bool debug = false;
#ifndef NDEBUG
    debug = true;
#endif // NDEBUG

    Application app("triangle", debug);

    return app.exec();
}