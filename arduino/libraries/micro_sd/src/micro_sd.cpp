#include <micro_sd.h>

bool sdOK = false;
/** Constructor
 */
micro_sd::micro_sd() {
 // Constructor implementation
}

/** Initialize the SD card
 */
void micro_sd::init(void) {
    // Reinitialize SPI and SD to ensure proper setup
    SD.end(); // Close SD if it was previously opened 
    SPI.end(); // Close SPI 
    delay(50); 
    SPI.begin(); // Reinitialize SPI 
    delay(50);

    sdOK = SD.begin(SD_CS_PIN);
    if (!sdOK) { 
        #ifdef DEBUG
        Serial.println("micro_sd::init - SD init failed!"); 
        #endif
        return;
    } 
    #ifdef DEBUG
    Serial.println("micro_sd::init - SD init OK");
    #endif 
}
/** Write data to the SD card
 * Structure of file is:
 * year_yyyy/month_mm/day_dd.ndjson 
 * Each line in the file is a JSON object with the following structure:
 * {time: "hh:mm", weight: 25.5}
 */
bool micro_sd::writeFile(StaticJsonDocument<2048>& doc, String folder_year, String folder_month, String file_day) {
    if(sdOK == false) {
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - SD not initialized");
        #endif
        init();
    }
    if(!SD.exists(folder_year)) {
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - Creating folder: " + folder_year);
        #endif
        SD.mkdir(folder_year);
    }
    if(!SD.exists(folder_year + "/" + folder_month)) {
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - Creating folder: " + folder_year + "/" + folder_month);
        #endif
        SD.mkdir(folder_year + "/" + folder_month);
    }
    String path = folder_year + "/" + folder_month + "/" + file_day + ".ndjson";
    File file = SD.open(path, FILE_WRITE);
    if (file) {
        serializeJson(doc, file);
        file.println(); // newline after each entry for readability
        file.close();
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - Wrote to " + path + " successfully");
        #endif
    } else {
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - Failed to open " + path + " for writing");
        #endif
        return false;
    }
    return true;
}
/** Append data to the SD card 
 * This function is a wrapper around writeFile that takes care of reading the existing data, appending the new entry, and writing it back to the file.
 * Esach value will be stored in file day_dd.ndjson with folder structure year_yyyy/month_mm
*/
void micro_sd::writeData(float weight) {
    // Create a document to hold the data
    StaticJsonDocument<2048> doc;

    String folder_year = "/year_" + rtc::getDateTime("year"); 
    String folder_month = "month_" + rtc::getDateTime("month");
    String file_day = "day_" + rtc::getDateTime("day");
    doc["time"] = rtc::getDateTime("hour") + ":" + rtc::getDateTime("minute");
    doc["weight"] = weight;

    // Write back to the file
    bool success = writeFile(doc, folder_year, folder_month, file_day);

    #ifdef DEBUG
    if (success) {
        Serial.println("micro_sd::writeData - Added new data line successfully");
    } else {
        Serial.println("micro_sd::writeData - Failed to write data line");
    }
    #endif
}
/** Get data from SD card in JSON format
 * This function will read the data from the SD card and return it in JSON format
 */
String micro_sd::getDataFromSD(String year, String month, String day) {
    String path = "/year_" + year + "/month_" + month + "/day_" + day + ".ndjson";

    if (!SD.exists(path)) {
        return "[]";
    }

    File file = SD.open(path);
    if (!file) {
        return "[]";
    }

    String result = "[";
    bool first = true;

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim();

        if (line.length() == 0) continue;  // ignorăm liniile goale

        if (!first) result += ",";
        first = false;

        result += line;  // linia NDJSON este deja un obiect JSON valid
    }

    file.close();
    result += "]";

    return result;
}

String micro_sd::getDataFromSD(String year, String month) {
    String path = "/year_" + year + "/month_" + month;

    if (!SD.exists(path)) {
        return "[]";
    }

    File monthDir = SD.open(path);
    if (!monthDir || !monthDir.isDirectory()) {
        return "[]";
    }

    std::vector<String> files;

    File entry = monthDir.openNextFile();
    while (entry) {
        if (!entry.isDirectory()) {
            String name = String(entry.name());
            if (name.endsWith(".ndjson")) {
                files.push_back(name);
            }
        }
        entry.close();
        entry = monthDir.openNextFile();
    }

    std::sort(files.begin(), files.end());

    String result = "[";
    bool first = true;

    for (String filename : files) {

        String fullPath = path + "/" + filename;
        File f = SD.open(fullPath);
        if (!f) continue;

        String lastLine = "";
        while (f.available()) {
            lastLine = f.readStringUntil('\n');
        }
        f.close();

        lastLine.trim();
        if (lastLine.length() == 0) continue;

        // extragem ziua
        String day = filename;
        day.replace("day_", "");
        day.replace(".ndjson", "");

        // construim obiectul JSON corect
        if (!first) result += ",";
        first = false;

        result += "{\"day\":\"" + day + "\",";
        result += lastLine.substring(1, lastLine.length() - 1);
        result += "}";
    }

    result += "]";
    return result;
}

String micro_sd::getDataFromSD(String year) {
    String path = "/year_" + year;

    if (!SD.exists(path)) {
        return "[]";
    }

    File yearDir = SD.open(path);
    if (!yearDir || !yearDir.isDirectory()) {
        return "[]";
    }

    std::vector<String> months;

    File entry = yearDir.openNextFile();
    while (entry) {
        if (entry.isDirectory()) {
            String name = String(entry.name());
            if (name.indexOf("month_") >= 0) {
                months.push_back(name);
            }
        }
        entry.close();
        entry = yearDir.openNextFile();
    }

    std::sort(months.begin(), months.end());

    String result = "[";
    bool first = true;

    for (String monthPath : months) {

        String monthName = monthPath.substring(monthPath.lastIndexOf('_') + 1);

        // apelăm funcția ta optimizată pentru lună
        String monthData = getDataFromSD(year, monthName);

        if (monthData != "[]") {
            if (!first) result += ",";
            first = false;

            result += "{\"month\":\"" + monthName + "\",\"data\":" + monthData + "}";
        }
    }

    result += "]";
    return result;
}




