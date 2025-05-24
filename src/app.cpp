#include "launch.h"
#include "WindowBase.h"
#include "WindowOnePoint.h"
#include "WindowOneTriangle.h"
#include "WindowOneCube.h"

int main()
{
    return run<WindowOneCube>();
}