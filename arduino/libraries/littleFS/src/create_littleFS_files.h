#ifndef CREATE_LITTLEFS_FILES_H
#define CREATE_LITTLEFS_FILES_H

#include <commonLibs.h>
#include <address_binary_files.h>
#include <LittleFS.h>


class LittleFSFile {
    public:
        static void init();
    private:
        static void writeChartJS();
        static void writeWebPageHTML();
        static void writeWebPageCSS();
        static void writeWebPageJS();

};
#endif // CREATE_LITTLEFS_FILES_H