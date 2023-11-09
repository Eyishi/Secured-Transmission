#include <cstdio>
#include "ServerOp.h"
int main()
{
    printf("%s 向你问好!\n", "ServerSeckey");
    ServerOp s("config.json");
    s.startServer();
    return 0;
}