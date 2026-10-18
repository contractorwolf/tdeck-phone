const String projectName = "XIAO PHONE";
const int ledPin =  13;      // the number of the LED pin

const long debounceDelay = 1000;    // the debounce time; increase if the output flickers
const String defaultTelephoneNumber = "15555550123";

const int callButtonPin = 3; //push button attached to this pin
int callButtonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
long callLastDebounceTime = millis();  // the last time the output pin was toggled

//CHANGE THIS TOMORROW: dont share pins that could be SDA, requires rewire
const int textButtonPin = 4; //push button attached to this pin
//CHANGE THIS TOMORROW: dont share pins that could be SDA
int textButtonState = LOW; //this variable tracks the state of the button, low if not pressed, high if pressed
long textLastDebounceTime = millis();  // the last time the output pin was toggled

long lastGPS = millis();
int gpsWaitPeriodSeconds = 30;
int requestCount = 0;

boolean isTextActive = true;
boolean isCallActive = true;
boolean isGPSActive = true;

boolean isGPSObtained = false;
boolean isGPSCurrent = false;
boolean isGPSRequested = false;
boolean isGPSSent = false;

int firstGPSTime = 0;
int firstGPSCount = 0;
int GPSMinSize = 108;
int GPSBodyMinSize = 80;
int GPSAquired = 0;

String GPSBody;
String West;
String North;
String DecimalMinutes;
String Degrees;
String GPSLink;

void setup() {
  // setup pins
  pinMode(ledPin, OUTPUT);
  pinMode(textButtonPin, INPUT);
  pinMode(callButtonPin, INPUT);

  GPSBody.reserve(26);
  West.reserve(11);
  North.reserve(11);
  GPSLink.reserve(60);
  Degrees.reserve(3);
  DecimalMinutes.reserve(10);

  // begin serial comms with IDE and SIM7600
  Serial.begin(9600);// serial from arduino ide
  Serial.setTimeout(250);
  
  Serial1.begin(9600);//serial to SIM7600 cellular board
  Serial1.setTimeout(250);

  Serial.print(projectName);
  Serial.println(" started");  
}

void loop() {
  // checks if it is able to send GPS (button pressed and GPS obtained
  if(canSendGPS()){
    isGPSRequested = false;
    isGPSSent = true;
    sendTextToDefaultNumber(GPSLink);
  }

  // check if send GPS button pressed
  if(isTextButtonPressed()){
     isGPSRequested = true;
     isGPSSent = false;
  }

  // check if call button is pressed
  if(isCallButtonPressed()){
     callDefaultNumber();
  }
  // ------------------------------------------

  
  // ----------------------------------------
  // read data from arduino ide
  // ------------------------------------------
  if (Serial.available() > 0)
  {
    String incoming = Serial.readString();
    incoming.trim();

    if(incoming == "c") {
        ///callDefaultNumber();
        Serial.println("CALL NUMBER - disabled");
    }else if(incoming == "t") {
        Serial.println("SEND TEXT - disabled");
    }else if(incoming == "g") {
        Serial.println("GET GPS");
        requestGPSData();
    }else if(incoming == "x") {
        Serial.println("GET Connection");
        requestCurrentConnection();
    }else if(incoming == "d") {
        Serial.println("GET Device Capabilites");
        requestDeviceCapabilities();
    }else if(incoming == "p") {
        Serial.println("GET Product Info");
        requestProductInfo();
    }else if(incoming == "#") {
        Serial.println("GET Phone Number");
        requestPhoneNumber();
    }else{
        Serial.print("incoming:");
        Serial.print(incoming);      
        Serial.print("/");   
        Serial.println(incoming.length());
    }
    // end if    
  }
  // ------------------------------------------ 


  // ------------------------------------------  
  // read incoming all incoming data from SIM7600
  // ------------------------------------------ 
  if (Serial1.available() > 0) {
    String messageFromSIM7600 = Serial1.readString();
    //remove incoming end \r\n from the message
    messageFromSIM7600.trim();

    if(isGPSActive){
      String gpsInfo = getBodyByPrefix("+CGNSSINFO: ", messageFromSIM7600);

      if(gpsInfo.length() != 0) {
        // a GPS Info message received from SIM7600
        Serial.print("gpsInfo: ");
        Serial.println(gpsInfo);   
  
        if(gpsInfo.length() >= GPSBodyMinSize && gpsInfo.length() < GPSBodyMinSize + 10){// small buffer for size
          North = getPointByTerminatingCharacter(gpsInfo, ",N,");
          West =  getPointByTerminatingCharacter(gpsInfo, ",W,"); 
          getGPSLink();
          isGPSCurrent = true;
          GPSAquired++;
  
          if(isGPSObtained == false){
            isGPSObtained = true;
            firstGPSTime = millis();
            firstGPSCount = requestCount;
          }
        }else{
           Serial.print("LAST ");
           isGPSCurrent = false;
        }
  
        Serial.print("north: ");
        Serial.print(North);
        Serial.print(" west: ");
        Serial.println(West);
        Serial.print("link: ");
        Serial.println(GPSLink);
        Serial.print("gpsInfo.length(): ");   
        Serial.println(gpsInfo.length());   
      }else{
        // not GPS Info message from SIM7600
        Serial.println(messageFromSIM7600);
      }
    }
    
    //Serial.println(messageFromSIM7600.length());
  }
  // ------------------------------------------ 

   // test by time period for GPS
   if (isGPSActive && ( millis() - lastGPS >= gpsWaitPeriodSeconds * 1000 )) {
        // Remember when we published
        lastGPS = millis();
        //turn gps off
        Serial.println("GET GPS period");
        Serial.print("millis: ");       
        Serial.print(lastGPS);
        Serial.print(": period: ");
        Serial.print(gpsWaitPeriodSeconds);   
         
        if(isGPSObtained == true){
           Serial.print(": firstGPSTime: ");
           Serial.print(firstGPSTime);  
           Serial.print(": firstGPSCount: ");
           Serial.print(firstGPSCount);   
        }       
        Serial.print(": GPSAquired: ");
        Serial.print(GPSAquired);  
        Serial.print("/");
        Serial.print(++requestCount);  
        Serial.println(" requests");
        requestGPSData();
    }
}

int checkPrefix(String message, String fullString){
  // +CGNSSINFO:
  int index = fullString.indexOf(message);
  if (index == -1) return index; // full string doesnt contain message
  return index + message.length(); 
}

String getBodyByPrefix(String messagePrefix, String fullString){
  int start = checkPrefix(messagePrefix, fullString);

  if (start == -1)return "";
  
  return getMessageBody(start, fullString);
}

String getMessageBody(int start, String fullString){
  return fullString.substring(start, fullString.length() - 5); //remove OK and newline characters
}

String getPointByTerminatingCharacter(String body, String terminatingCharacters){
  int index = body.indexOf(terminatingCharacters);

  if (index == -1)return "";
  
  return body.substring(index - 11, index);
}

String convertToGPSDMMfromPoint(String point){
  // Degrees and decimal minutes (DMM): 41 24.2028, 2 10.4418
  // https://support.google.com/maps/answer/18539?hl=en&co=GENIE.Platform%3DDesktop
  // 08052.284623 (Point)
  // 80%2052.284623 (DDM for Google Maps)
  int dotIndex = point.indexOf(".");

  Degrees = point.substring(0, 2);
  
  DecimalMinutes = point.substring(dotIndex - 2, point.length());

  return Degrees + "%20" + DecimalMinutes;
}

void getGPSLink(){
  // example:  https://www.google.com/maps/place/41%2024.2028,2%2010.4418
  GPSLink = "https://www.google.com/maps/place/" + convertToGPSDMMfromPoint(North) + ",-" + convertToGPSDMMfromPoint(West);
}

bool canSendGPS() {
  return isTextActive && isGPSActive && isGPSObtained && isGPSRequested && !isGPSSent;
}

bool isTextButtonPressed(){
  long current = millis();    

  if ((current - textLastDebounceTime) > debounceDelay && digitalRead(textButtonPin) == HIGH) {
      Serial.print("text buttonState:");
      Serial.println(digitalRead(textButtonPin) == HIGH);      
     textLastDebounceTime = current + 1000; //set the current time     
     return true;
  }

  return false;
}

bool isCallButtonPressed(){
  long current = millis();
  //(current - callLastDebounceTime) > debounceDelay && 
  if ((current - callLastDebounceTime) > debounceDelay && digitalRead(callButtonPin) == HIGH) {
     Serial.print("call buttonState:");
     Serial.println(digitalRead(callButtonPin) == HIGH);     
     callLastDebounceTime = current + 1000; //set the current time
     return true;
  }

  return false;
}

void callDefaultNumber() {
  Serial.print("MAKE CALL VIA BUTTON: ");
  Serial.print(millis());  
  Serial.print(": callLastDebounceTime: ");  
  Serial.println(callLastDebounceTime);  
  
  if(isCallActive == true) {
    Serial1.print("ATD+");
    Serial1.print(defaultTelephoneNumber);
    Serial1.println(";");
  }

  Serial.println("MAKE CALL COMPLETED");
}

void sendTextToDefaultNumber(String message) {
  Serial.print("SEND TEXT VIA BUTTON: ");
  Serial.print(millis());  
  Serial.print(": textLastDebounceTime: ");  
  Serial.println(textLastDebounceTime);  

  if(isTextActive == true){
    Serial1.print("AT+CMGS=\"");// must have quotes on number
    Serial1.print(defaultTelephoneNumber);
    Serial1.println("\"");// must have quotes on number
    
    //wait for > response from SIM7600
    delay(200);
    
    // send message to SIM7600
    Serial1.print(message);
    Serial1.print(" @");
    Serial1.print(millis());
    
    // log text part of sent message
    Serial.print("sent: ");
    Serial.println(message);
    
    // close message sending process with control-z character
    Serial1.write(0x1A);  // sends ctrl+z end of message
  }

  Serial.println("COMPLETED");
}

void requestGPSData() {
  Serial.print("requestGPSData: ");
  Serial.println(millis());  
  Serial1.println("AT+CGNSSINFO");
  Serial.println("COMPLETED");
}

void requestCurrentConnection() {
  Serial.print("requestCurrentConnection: ");
  Serial.println(millis());  
  Serial1.println("AT+COPS?");
  Serial.println("COMPLETED");
}

void requestDeviceCapabilities() {
  Serial.print("requestDeviceCapabilities: ");
  Serial.println(millis());  
  Serial1.println("AT+GCAP");
  Serial.println("COMPLETED");
}

void requestProductInfo() {
  Serial.print("requestProductInfo: ");
  Serial.println(millis());  
  Serial1.println("ATI");
  Serial.println("COMPLETED");
}

void requestPhoneNumber() {
  Serial.print("requestPhoneNumber: ");
  Serial.println(millis());  
  Serial1.println("AT+CNUM");
  Serial.println("COMPLETED");
}


/*
 * NEED TO REWRITE THIS FOR SIM7600
 *     while (mySerial->available()) {
      char c = mySerial->read();
      if (c == '\r')
        continue;
      if (c == 0xA) {
        if (replyidx == 0) // the first 0x0A is ignored
          continue;

        if (!multiline) {
          timeout = 0; // the second 0x0A is the end of the line
          break;
        }
      }
      replybuffer[replyidx] = c;
      // DEBUG_PRINT(c, HEX); DEBUG_PRINT("#"); DEBUG_PRINTLN(c);
      replyidx++;
    }

// EXAMPLE FOR SENDING SMS WITH CMGS

bool Adafruit_FONA::sendSMS(char *smsaddr, char *smsmsg) {
  if (!sendCheckReply(F("AT+CMGF=1"), ok_reply))
    return false;

  char sendcmd[30] = "AT+CMGS=\"";
  strncpy(sendcmd + 9, smsaddr,
          30 - 9 - 2); // 9 bytes beginning, 2 bytes for close quote + null
  sendcmd[strlen(sendcmd)] = '\"';

  if (!sendCheckReply(sendcmd, F("> ")))
    return false;

  DEBUG_PRINT(F("> "));
  DEBUG_PRINTLN(smsmsg);

  mySerial->println(smsmsg);
  mySerial->println();
  mySerial->write(0x1A);

  DEBUG_PRINTLN("^Z");

  if ((_type == FONA3G_A) || (_type == FONA3G_E)) {
    // Eat two sets of CRLF
    readline(200);
    // DEBUG_PRINT("Line 1: "); DEBUG_PRINTLN(strlen(replybuffer));
    readline(200);
    // DEBUG_PRINT("Line 2: "); DEBUG_PRINTLN(strlen(replybuffer));
  }
  readline(10000); // read the +CMGS reply, wait up to 10 seconds!!!
  // DEBUG_PRINT("Line 3: "); DEBUG_PRINTLN(strlen(replybuffer));
  if (strstr(replybuffer, "+CMGS") == 0) {
    return false;
  }
  readline(1000); // read OK
  // DEBUG_PRINT("* "); DEBUG_PRINTLN(replybuffer);

  if (strcmp(replybuffer, "OK") != 0) {
    return false;
  }

  return true;
}

// maybe working example?
Serial.write(0x1A);  // sends ctrl+z end of message
Serial.write(0x0D);  // Carriage Return in Hex
Serial.write(0x0A);  // Line feed in Hex
// from: https://forum.arduino.cc/t/sending-sms-using-gsm-sim900/225745

#include <NewSoftSerial.h> //Include the NewSoftSerial library to send serial commands to the cellular module.

char r=13; //return character
NewSoftSerial cell(4,3); //Create a ‘fake’ serial port. Pin 4 is the Rx pin, pin 3 is the Tx pin.
String cellContent=""; //string we’re sending
String ctlZ="\x1A"; //the code for Ctrl-Z

String content=“Here is my SMS messaage”; //The text I’m sending

void setup() {

// start the serial library:
Serial.begin(9600);
cell.begin(9600);
}

void loop()
{
  //add our message to the CellContent string
  cellContent+=content;
  //add the Crtl-Z hex character
  cellContent+="\x1A";
  //put the modem into text mode
  textMode();
  //wait for a moment
  delay(50);
  //send message
  sendMsg();
  
  cellContent="";
  delay(15000);
}

void textMode()
{
  //init text mode
  cell.print(“AT+CMGF=1”);
  cell.print(r);
}

void sendMsg(){
  char r=13; //return character
  //This number needs to be replaced with the number you are sending to
  cell.print("AT+CMGS=“5555550124"”);
  cell.print(r);
  cell.print(content);
  cell.print("\x1A");
  cell.print(r);
}
*/