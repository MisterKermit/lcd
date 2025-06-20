#include <Arduino.h>
#include <LiquidCrystal.h>


const int rs = 40, en = 38, d4 = 36, d5 = 34, d6 = 32, d7 = 30;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
// char message2FromPC[numChars] = {0};
// char message3FromPC[numChars] = {0};

int integerFromPC = 0;
// int integer2FromPC = 0;
// int integer3FromPC = 0;

boolean newData = false;

//============
//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial1.available() > 0 && newData == false) {
        rc = Serial1.read();
        Serial.println(rc);

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC
  
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    integerFromPC = atoi(strtokIndx);     // convert this part to an integer

    // strtokIndx = strtok(NULL,",");      // get the 2nd part - the string
    // strcpy(message2FromPC, strtokIndx); // copy it to messageFromPC

    // strtokIndx = strtok(NULL, ",");
    // integer2FromPC = atoi(strtokIndx);     // convert this part to a int

    // strtokIndx = strtok(NULL,",");      // get the first part - the string
    // strcpy(message3FromPC, strtokIndx); // copy it to messageFromPC

    // strtokIndx = strtok(NULL, ",");
    // integer3FromPC = atoi(strtokIndx);     // convert this part to a int

}

//============

void showParsedData() {
    Serial.print("Message ");
    Serial.println(messageFromPC);
    Serial.print("Integer ");
    Serial.println(integerFromPC);
    // Serial.print("Message2  ");
    // Serial.println(message2FromPC);
    // Serial.print("Integer2 ");
    // Serial.println(integer2FromPC);
    // Serial.print("Message3 ");
    // Serial.println(message3FromPC);
    // Serial.print("Integer3 ");
    // Serial.println(integer3FromPC);
}

void setup() {
    lcd.begin(16,2);
    lcd.clear();
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial.println("This demo expects 6 pieces of data - text, an integer x3");
    // Serial1.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();
}

//============

void loop() {
    lcd.setCursor(0,0);
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        lcd.clear();

        newData = false;
    }
    
    float percent = integerFromPC / 255.0;
    percent *= 100;
    lcd.print(messageFromPC);
    lcd.print(int(percent));
    


    // lcd.print(message2FromPC);
    // lcd.println(integer2FromPC);
    // lcd.print(message3FromPC);
    // lcd.print(integer3FromPC);
}



