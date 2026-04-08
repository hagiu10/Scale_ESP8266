#include <create_littleFS_files.h>

void LittleFSFile::init() {
    if (!LittleFS.begin()) {
        #ifdef DEBUG
        Serial.println("An error has occurred while mounting LittleFS");
        #endif
        return;
    }
    if (!LittleFS.exists("/chart.min.js")) {
        #ifdef DEBUG
        Serial.printf("LittleFSfile::init() - chart.min.js does not exist in LittleFS, writing it...\n");
        #endif
        writeChartJS();
    } 
}
void LittleFSFile::writeChartJS() {
    #if defined(CHART_MIN_START_ADDRESS) && defined(CHART_MIN_END_ADDRESS)
    #ifdef DEBUG
    Serial.printf("Chart.js binary file can be flashing at address range: 0x%06X - 0x%06X\n", CHART_MIN_START_ADDRESS, CHART_MIN_END_ADDRESS);
    #endif
    File file = LittleFS.open("/chart.min.js", "w");
    if (!file) {
        #ifdef DEBUG
        Serial.printf("LittleFSfile::writeChartJS() - Failed to open file for writing\n");
        return;
        #endif
    }
    // Write the binary data to the file
    for (uint32_t addr = CHART_MIN_START_ADDRESS; addr < CHART_MIN_END_ADDRESS; addr++) {
        uint8_t byte = *((uint8_t*)addr); // Read byte from the specified address
        Serial.printf("LittleFSfile::writeChartJS() - Writing byte 0x%02X from address 0x%06X to file\n", byte, addr);
        file.write(byte); // Write byte to the file

    }
    file.close();
    #else
    #ifdef DEBUG
    Serial.printf("LittleFSfile::writeChartJS() - Error: chart.min.js binary file address range is not defined in address_binary_files.h\n");
    #endif
    #endif
}