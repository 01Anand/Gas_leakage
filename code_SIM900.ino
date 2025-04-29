#include <SoftwareSerial.h>
SoftwareSerial SIM900(7, 8);

int sensor = 12;
int speaker = 13;
int gas_value, Gas_alert_val, Gas_shut_val;
int Gas_Leak_Status;
int call_count = 0; // Counter for the number of calls
int sensorValue;
int flg = 0;
int GasSensorPin = A0;
String location;
String phoneNumber = "+917350617969"; // The phone number to call

/* Variables for fetching Co-ordinates */
int i = 0;
String pass;
char *token;
char response[800], fin[30];
String loc[2];

void setup() {
  pinMode(sensor, INPUT);
  SIM900.begin(9600);
  Serial.begin(9600);
  location = show();
  delay(500);
  Serial.println("SIM900 Initialized");
  SIM900.println("AT"); // Test AT command
  delay(100);
  ShowSerialData();
}

void loop() {
  CheckGas();
  CheckShutDown();
}

void CheckGas() {
  sensorValue = ScanGasLevel();      // read analog input pin 0
  Serial.println(sensorValue, DEC); // prints the value read
  delay(100);                       // wait 100ms for next
  if (sensorValue > 600) {
    SetAlert();
    flg = 1;
  }
}

int ScanGasLevel() {
  sensorValue = analogRead(GasSensorPin); // read analog input pin 0
  delay(100);                             // wait 100ms for next reading
  return sensorValue;                     // returns temperature value in degree celsius
}

void SetAlert() {
  tone(13, 1000, 10000);
  while (call_count < 1 && flg == 0) // Number of Call Attempts
  {
    MakeCall(); // Function to make a voice call
  }
  Gas_Leak_Status = 1;
}

void CheckShutDown() {
  if (Gas_Leak_Status == 1) {
    Gas_shut_val = ScanGasLevel(); // scan the gas level
    if (Gas_shut_val < 600) {
      noTone(13);         // stop the buzzer
      call_count = 0;
      Gas_Leak_Status = 0;
    }
  }
}

void MakeCall() {
  Serial.print("Calling ");
  Serial.println(phoneNumber);
  SIM900.print("ATD");
  SIM900.print(phoneNumber);
  SIM900.println(";"); // Dial command with semicolon at the end
  delay(20000);        // Keep the call active for a certain duration (adjust as needed)
  SIM900.println("ATH");   // Hang up the call
  Serial.println("Call ended.");
  delay(5000);         // Delay before allowing another call
  call_count++;
}

void ShowSerialData() {
  while (SIM900.available() != 0) {
    Serial.write(char(SIM900.read()));
    response[i] = char(SIM900.read());
    Serial.print(response[i++]);
  }
}

String show() {
  SIM900.println("AT+CSQ"); // Signal quality check
  delay(100);
  ShowSerialData();

  SIM900.println("AT+CGATT?"); //Attach or Detach from GPRS Support
  delay(100);
  ShowSerialData();

  SIM900.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""); //setting the SAPBR, the connection type is using gprs
  delay(1000);
  ShowSerialData();

  SIM900.println("AT+SAPBR=3,1,\"APN\",\"CMNET\""); //setting the APN, Access point name string
  delay(4000);
  ShowSerialData();

  SIM900.println("AT+SAPBR=1,1"); //Openig the bearer having identifier 1
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+SAPBR=2,1"); //query the bearer having identifier 1
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+CIPGSMLOC=1,1"); //setting the SAPBR
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+CIPGSMLOC=2,1"); //querying for the location
  delay(2000);
  ShowSerialData();

  SIM900.println("AT+SAPBR=0,1"); //Deactivating the bearer having identifier 1
  delay(2000);
  ShowSerialData();

  int j = 0;
  for (i = 200; i < 222; ++i) {
    fin[j] = response[i];
    j++;
  }
  String str(fin);
  token = strtok(fin, ",");
  int x = 0;
  while (token != NULL) {
    if (x < 2)
      loc[x] = token;
    x++;
    token = strtok(NULL, ",");
  }

  pass = "http://maps.google.com/?q=";
  pass.concat(loc[2]);
  pass.concat(",");
  pass.concat(loc[1]);
  return pass;
}
