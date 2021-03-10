
/**
 * Created by K. Suwatchai (Mobizt)
 * 
 * Email: k_suwatchai@hotmail.com
 * 
 * Github: https://github.com/mobizt
 * 
 * Copyright (c) 2020 mobizt
 *
*/

//This example shows how to set array data through FirebaseJsonArray object then read the data back and parse them.

#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "Newxxx"
#define WIFI_PASSWORD "12345555"

#define FIREBASE_HOST "https://iotmyproject-776c0-default-rtdb.firebaseio.com/"

/** The database secret is obsoleted, please use other authentication methods, 
 * see examples in the Authentications folder. 
*/
#define FIREBASE_AUTH "PeSw47kcQm3Tfonija9j7qpgroZEvHKz6WBbxt5N"

//Define Firebase Data Object
FirebaseData fbdo;

FirebaseJsonArray arr;

void printResult(FirebaseData &data);

unsigned long sendDataPrevMillis = 0;

String path = "/room/temperature";

uint16_t count = 0;
const int sensorPin = A0; // connecting LM35
void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    fbdo.setBSSLBufferSize(1024, 1024);

    //Set the size of HTTP response buffers in the case where we want to work with large data.
    fbdo.setResponseSize(1024);

    if (!Firebase.beginStream(fbdo, path))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't begin stream connection...");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
    pinMode(sensorPin, INPUT);
    
}

void loop()
{
    int sensorValue = analogRead(sensorPin);
    float millivolts = (sensorValue / 1024.0) * 3300; //3300 is the voltage provided by NodeMCU
    float celsius = millivolts / 10;
//    Serial.print(celsius);
//    Serial.println(" degree celsius");  
    if (millis() - sendDataPrevMillis > 15000)
    {
        sendDataPrevMillis = millis();
        count++;

        Serial.println("------------------------------------");
        Serial.println("Set Array...");

        arr.clear();
        arr.set("/[0]", count);
        arr.set("/[1]", "temperature");
        arr.set("/[2]", "bedroom");
        arr.set("/[3]", round(celsius));
        if (Firebase.set(fbdo, path + "/Data", arr))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.print("VALUE: ");
            printResult(fbdo);
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }

        Serial.println("------------------------------------");
        Serial.println("Get Array...");
        if (Firebase.get(fbdo, path + "/Data"))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.print("VALUE: ");
            printResult(fbdo);
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }
    }

    if (!Firebase.readStream(fbdo))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't read stream data...");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }

    if (fbdo.streamTimeout())
    {
        Serial.println("Stream timeout, resume streaming...");
        Serial.println();
    }

    if (fbdo.streamAvailable())
    {
        Serial.println("------------------------------------");
        Serial.println("Stream Data available...");
        Serial.println("STREAM PATH: " + fbdo.streamPath());
        Serial.println("EVENT PATH: " + fbdo.dataPath());
        Serial.println("DATA TYPE: " + fbdo.dataType());
        Serial.println("EVENT TYPE: " + fbdo.eventType());
        Serial.print("VALUE: ");
        printResult(fbdo);
        Serial.println("------------------------------------");
        Serial.println();
    }
}
float round(float var) 
{ 
    // 37.66666 * 100 =3766.66 
    // 3766.66 + .5 =3767.16    for rounding off value 
    // then type cast to int so value is 3767 
    // then divided by 100 so the value converted into 37.67 
    float value = (int)(var * 100 + .5); 
    return (float)value / 100; 
}
void printResult(FirebaseData &data)
{

    if (data.dataType() == "int")
        Serial.println(data.intData());
    else if (data.dataType() == "float")
        Serial.println(data.floatData(), 5);
    else if (data.dataType() == "double")
        printf("%.9lf\n", data.doubleData());
    else if (data.dataType() == "boolean")
        Serial.println(data.boolData() == 1 ? "true" : "false");
    else if (data.dataType() == "string")
        Serial.println(data.stringData());
    else if (data.dataType() == "json")
    {
        Serial.println();
        FirebaseJson &json = data.jsonObject();
        //Print all object data
        Serial.println("Pretty printed JSON data:");
        String jsonStr;
        json.toString(jsonStr, true);
        Serial.println(jsonStr);
        Serial.println();
        Serial.println("Iterate JSON data:");
        Serial.println();
        size_t len = json.iteratorBegin();
        String key, value = "";
        int type = 0;
        for (size_t i = 0; i < len; i++)
        {
            json.iteratorGet(i, type, key, value);
            Serial.print(i);
            Serial.print(", ");
            Serial.print("Type: ");
            Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
            if (type == FirebaseJson::JSON_OBJECT)
            {
                Serial.print(", Key: ");
                Serial.print(key);
            }
            Serial.print(", Value: ");
            Serial.println(value);
        }
        json.iteratorEnd();
    }
    else if (data.dataType() == "array")
    {
        Serial.println();
        //get array data from FirebaseData using FirebaseJsonArray object
        FirebaseJsonArray &arr = data.jsonArray();
        //Print all array values
        Serial.println("Pretty printed Array:");
        String arrStr;
        arr.toString(arrStr, true);
        Serial.println(arrStr);
        Serial.println();
        Serial.println("Iterate array values:");
        Serial.println();
        for (size_t i = 0; i < arr.size(); i++)
        {
            Serial.print(i);
            Serial.print(", Value: ");

            FirebaseJsonData &jsonData = data.jsonData();
            //Get the result data from FirebaseJsonArray object
            arr.get(jsonData, i);
            if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
                Serial.println(jsonData.boolValue ? "true" : "false");
            else if (jsonData.typeNum == FirebaseJson::JSON_INT)
                Serial.println(jsonData.intValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
                Serial.println(jsonData.floatValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
                printf("%.9lf\n", jsonData.doubleValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
                     jsonData.typeNum == FirebaseJson::JSON_NULL ||
                     jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
                     jsonData.typeNum == FirebaseJson::JSON_ARRAY)
                Serial.println(jsonData.stringValue);
        }
    }
    else if (data.dataType() == "blob")
    {

        Serial.println();

        for (int i = 0; i < data.blobData().size(); i++)
        {
            if (i > 0 && i % 16 == 0)
                Serial.println();

            if (i < 16)
                Serial.print("0");

            Serial.print(data.blobData()[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    else if (data.dataType() == "file")
    {

        Serial.println();

        File file = data.fileStream();
        int i = 0;

        while (file.available())
        {
            if (i > 0 && i % 16 == 0)
                Serial.println();

            int v = file.read();

            if (v < 16)
                Serial.print("0");

            Serial.print(v, HEX);
            Serial.print(" ");
            i++;
        }
        Serial.println();
        file.close();
    }
    else
    {
        Serial.println(data.payload());
    }
}
