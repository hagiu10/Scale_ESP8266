#include <micro_sd.h>
/** Constructor
 */
micro_sd::micro_sd() {
 // Constructor implementation
}

/** Initialize the SD card
 */
void micro_sd::init(void) {
    if (!SD.begin(SD_CS_PIN)) { 
        Serial.println("SD init failed!"); 
    } else { 
        Serial.println("SD init OK"); 
        if (!SD.exists("/data.json")) { 
            // create empty JSON file 
            StaticJsonDocument<256> doc; 
            doc["day"] = JsonArray(); 
            doc["month"] = JsonArray(); 
            doc["year"] = JsonArray(); 
            File f = SD.open("/data.json", FILE_WRITE); 
            serializeJson(doc, f); 
            f.close(); 
        } 
    }
}
/** Read data from the SD card
 */

 StaticJsonDocument<2048> micro_sd::readFile(String path = "/data.json") {
    StaticJsonDocument<2048> doc;
    File file = SD.open(path, FILE_READ);
    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            #ifdef DEBUG
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.f_str());
            #endif
        }
        file.close();
    } else {
        #ifdef DEBUG
        Serial.println("Failed to open data.json for reading");
        #endif
    }
    return doc;
}
/** Write data to the SD card
 */
void micro_sd::writeFile(StaticJsonDocument<2048>& doc, String path = "/data.json") {
    File file = SD.open(path, FILE_WRITE);
    if (file) {
        file.seek(0);
        serializeJson(doc, file);
        file.close();
    } else {
        #ifdef DEBUG
        Serial.println("Failed to open data.json for writing");
        #endif
    }
}
/** Write in array day value */
void micro_sd::writeData(String name_location = "day", char date_time[] = "00:00", float weight) {
    // Read existing JSON data from the file
    StaticJsonDocument<2048> doc = readFile("/data.json");
    // Add new entry to the specified array
    JsonArray dataArray = doc[name_location].as<JsonArray>();
    // Create a new JSON object for the entry
    StaticJsonDocument<256> entry;  
    entry["time"] = date_time;
    entry["weight"] = weight;
    // Add the entry to the array
    dataArray.add(entry);
    // Write updated JSON data back to the file
    writeFile(doc, "/data.json");
}
/** Load data from SD and return as JSON string */
String micro_sd::loadDataFromSD(void) {
    StaticJsonDocument<2048> doc = readFile("/data.json");
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

