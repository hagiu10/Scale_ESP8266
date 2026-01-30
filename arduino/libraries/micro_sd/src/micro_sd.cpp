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
    if (!SD.exists("/data.json")) { 
        // create empty JSON file 
        StaticJsonDocument<256> doc; 
        doc["day"] = JsonArray(); 
        doc["month"] = JsonArray(); 
        doc["year"] = JsonArray(); 
        File f = SD.open("/data.json", FILE_WRITE); 
        serializeJson(doc, f); 
        f.close();
        #ifdef DEBUG
        Serial.println("micro_sd::init - Created new /data.json file");
        #endif 
    } 
    
}
/** Read data from the SD card
 */

 StaticJsonDocument<2048> micro_sd::readFile(String path = "/data.json") {
    StaticJsonDocument<2048> doc;

    if(sdOK == false) {
        #ifdef DEBUG
        Serial.println("micro_sd::readFile - SD not initialized");
        #endif
        return doc;
    }

    File file = SD.open(path, FILE_READ);
    if (file) {
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            #ifdef DEBUG
            Serial.print("micro_sd::readFile - deserializeJson() failed: ");
            Serial.println(error.f_str());
            #endif
        }
        file.close();
    } else {
        #ifdef DEBUG
        Serial.println("micro_sd::readFile - Failed to open data.json for reading");
        #endif
    }
    return doc;
}
/** Write data to the SD card
 */
void micro_sd::writeFile(StaticJsonDocument<2048>& doc, String path = "/data.json") {
    if(sdOK == false) {
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - SD not initialized");
        #endif
        return;
    }

    File file = SD.open(path, FILE_WRITE);
    if (file) {
        file.seek(0);
        serializeJson(doc, file);
        file.close();
    } else {
        #ifdef DEBUG
        Serial.println("micro_sd::writeFile - Failed to open data.json for writing");
        #endif
    }
}
/** Write in array day value */
void micro_sd::writeData(float weight, String name_location = "day", String date_time = "00:00") {
    if(sdOK == false) {
        #ifdef DEBUG
        Serial.println("micro_sd::writeData - SD not initialized");
        #endif
        return;
    }
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
    if(sdOK == false) {
        #ifdef DEBUG
        Serial.println("micro_sd::loadDataFromSD - SD not initialized");
        #endif
        return String();
    }
    StaticJsonDocument<2048> doc = readFile("/data.json");
    String jsonString;
    serializeJson(doc, jsonString);
    return jsonString;
}

