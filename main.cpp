//main.cpp
#include "cdocs_threadpool/class.hpp"

int main() {

    CDOCS_threadpool threadpool = CDOCS_threadpool();

    threadpool.thread();
    threadpool.save_md_preproc();

    return 0;
}