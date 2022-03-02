//
// Created by mushan
//

#include <unistd.h>
#include "./server/server.h"

int main() {
    daemon(1, 1);

    Server server{};
    server.start();
}