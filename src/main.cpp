#include <Arduino.h>
#include <LiquidCrystal.h>


const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
char messageFromPC[numChars] = {0};
char message2FromPC[numChars] = {0};
char message3FromPC[numChars] = {0};

int integerFromPC = 0;
char text2fromPC[numChars] = {0};
char text3fromPC[numChars] = {0};

boolean newData = false;

//============
//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
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

    strtokIndx = strtok(NULL,",");      // get the 2nd part - the string
    strcpy(message2FromPC, strtokIndx); // copy it to messageFromPC

    strtokIndx = strtok(NULL, ",");
    strcpy(text2fromPC, strtokIndx);

    strtokIndx = strtok(NULL,",");      // get the first part - the string
    strcpy(message3FromPC, strtokIndx); // copy it to messageFromPC

    strtokIndx = strtok(NULL, ",");
    strcpy(text3fromPC, strtokIndx); 

}

//============

void showParsedData() {
    // Serial.print("Message ");
    // Serial.println(messageFromPC);
    // Serial.print("Integer ");
    // Serial.println(integerFromPC);
    // Serial.print("Message2  ");
    // Serial.println(message2FromPC);
    // Serial.print("Integer2 ");
    // Serial.println(text2fromPC);
    // Serial.print("Message3 ");
    // Serial.println(message3FromPC);
    // Serial.print("Integer3 ");
    // Serial.println(text3fromPC);
}

void setup() {
    lcd.begin(16,2);
    lcd.clear();
    Serial.begin(9600);
    Serial.println("This demo expects 6 pieces of data - text, an integer x3");
    Serial.println();
}

//============

void loop() {
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

    lcd.setCursor(0,0);

    float percent = integerFromPC / 300.0;
    percent *= 100;
    lcd.print(messageFromPC);
    lcd.print(int(percent));

    lcd.print(message2FromPC);
    lcd.print(text2fromPC);

    lcd.setCursor(0,1);
    lcd.print(message3FromPC);
    lcd.print(text3fromPC);

}



