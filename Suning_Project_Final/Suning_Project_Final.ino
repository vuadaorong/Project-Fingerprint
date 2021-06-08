

/* ----------------Begin khai báo thư viện----------------- */
//khai báo thư viện để sử dụng I2C
#include <LiquidCrystal_I2C.h>
//khai báo để sử dụng thư viện Keypad
#include <Keypad.h>
//khai báo để sử dụng thư viện của màn hình CLD
#include <LiquidCrystal_PCF8574.h>
#include <Wire.h>
/* ----------------End khai báo thư viện------------------- */
#include <Adafruit_Fingerprint.h>
SoftwareSerial mySerial(3, 2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
/* ---------------Begin khai báo và khởi tạo các biến---------- */
//thiết đặt cho LCD với địa chỉ 0x27 có 16 ký tự 4 dòng
LiquidCrystal_PCF8574 lcd(0x27);
int homie = 0;
int num;
int count = 0;
int mainHome = 1;
bool lastState = 0;
bool kt;
int delPass = 1890;
int numPass = 1969;
int adpass = 1905;
int R = A0;
int G = A1;
int B = A2;
uint8_t id;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {5, 6, 7, 8};
byte colPins[COLS] = {9, 10, 11};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
/* ---------------End khai báo và khởi tạo các biến---------- */

/*-------------------Begin hàm setup ----------------------*/
void setup()
{

  int error;
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();

  lcd.begin(16, 4);//khai báo LCD với 16 ký tự và 4 dòng
  lcd.setBacklight(255);
  screen();
  fgr();
  pinMode(4, OUTPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);

}

//-----------hàm screen------------------

void screen() {
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("HOME");
  lcd.setCursor(0, 1);
  lcd.print("PUSH FINGERPRINT");
  lcd.setCursor(0, 2);
  lcd.print("# -> ENTER NUM");
  lcd.setCursor(0, 3);
  lcd.print("* -> SETTINGS");
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);

}
/*-------------------End hàm setup ----------------------*/

/* --------------------Begin hàm create------------------- */
void create()
{

  String str = "";
  String pw = "";
  char key = keypad.getKey();

  //-----------------Menu cai dat----------------

  if (key == '*') {
a:
    digitalWrite(R, LOW);
    digitalWrite(G, LOW);
    digitalWrite(B, HIGH);
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("SETTINGS");
    lcd.setCursor(0, 1);
    lcd.print("ENTER PASS: ");
    lcd.setCursor(0, 2);
    lcd.print("* -> BACK HOME");
    lcd.setBacklight(255);
    homie = 1;

    while (homie == 1)
    {
      char key = keypad.getKey();
      if (key);
      delay(100);
      if (key) {
        pw += "*";
        str += (key - 48);
        num = str.toInt();
        lcd.setCursor(11, 1);
        lcd.print(pw);
        lcd.setCursor(0, 3);
        lcd.print("# -> CONFIRM");

      }
      //------------về trang chủ-------------
      if (key == '*') {
        lcd.clear();
        screen();
        homie = 0;
      }
      //-----------kiểm tra admin pass------------
      if (key == '#') {
        if (num == adpass) {
          lcd.clear();
          lcd.setCursor(2, 1);
          lcd.print("WELCOME ADMIN");
          lcd.setCursor(1, 2);
          lcd.print("TO SMART VAULT");
          delay(2000);
          lcd.clear();
          caidat();
          homie = 0;
        }
        else {
          lcd.clear();
          lcd.print("INVALID PASSWORD ");
          delay(2000);
          lcd.clear();
          homie = 0;
          str = "";
          pw = "";
          goto a;
        }
      }

    }
  }

  //--------Menu Nhap Numberpass--------------

  else if (key == '#') {
    digitalWrite(R, HIGH);
    digitalWrite(G, LOW);
    digitalWrite(B, HIGH);
    String str = "";
    kt = true;
    lcd.setBacklight(255);
    lcd.home();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NUMBER PASSWORD");
    lcd.setCursor(0, 1);
    lcd.print("ENTER NUM:");
    lcd.setCursor(0, 2);
    lcd.print("* -> BACK HOME");
    while (kt)
    {
      char key = keypad.getKey();
      if (key);
      delay(100);
      if (key) {
        pw += "*";
        str += (key - 48);
        num = str.toInt();
        lcd.setCursor(11, 1);
        lcd.print(pw);
        lcd.setCursor(0, 3);
        lcd.print("# -> CONFIRM");
      }
      //---------về trang chủ--------
      if (key == '*') {
        lcd.clear();
        screen();
        kt = false;
      }
      //------------kiểm tra number pass-------------
      if (key == '#') {
        if (num == numPass) {
          digitalWrite(R, LOW);
          digitalWrite(G, HIGH);
          digitalWrite(B, LOW);
          lcd.clear();
          lcd.setCursor(0, 1);
          digitalWrite(4, HIGH);
          lcd.print("NUMBERPASS DOOR");
          lcd.setCursor(4, 2);
          lcd.print("  OPEN!");
          delay(2000);
          displayFingerOK();
          screen();
          kt = false;
        }
        else {
          lcd.clear();
          lcd.print("INVALID PASSWORD ");
          delay(2000);
          lcd.clear();
          kt = false;
          str = "";
          pw = "";
          screen();
        }
      }
    }
  }

}
//hàm menu caidat

void caidat() {
  String str = "";
  kt = true;
  lcd.setBacklight(255);
  lcd.home();
  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("SETTING");
  lcd.setCursor(0, 1);
  lcd.print("ENTER ID:");
  lcd.setCursor(0, 2);
  lcd.print("* -> BACK HOME");
  while (kt)
  {
    char key = keypad.getKey();
    if (key);
    delay(100);
    if (key) {
      str += (key - 48);
      num = str.toInt();

      lcd.setCursor(10, 1);
      lcd.print(num);
      lcd.setCursor(0, 3);
      lcd.print("# -> CONFIRM");
    }
    if (key == '*') {
      screen();
      kt = false;
    }
    if (key == '#') {
      id = num;
      //-----------xóa dấu vân tay-----------------
      if (num == delPass) {
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("DELETED ALL");
        lcd.setCursor(2, 2);
        lcd.print("FINGERPRINT");
        finger.emptyDatabase();
        id = 0;
        kt = false;
        num = 0;
        delay(2000);
        screen();
        kt = false;
      }
      //----------------Nếu ID nhỏ hơn 1 hoặc lớn hơn 100 thì không cho phép mở khóa và thử lại---
      else if (num < 1 || num > 100) {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("ID CANNOT BE");
        lcd.setCursor(0, 1);
        lcd.print("lESS THAN 1 OR");
        lcd.setCursor(0, 2);
        lcd.print("GREATER THAN 100");
        delay(3000);
        screen();
        kt = false;
      }
      else {
        digitalWrite(R, HIGH);
        digitalWrite(G, LOW);
        digitalWrite(B, LOW);
        while (!getFingerprintEnroll() );
        kt = false;
      }
    }
  }
}

/* --------------------Begin hàm Loop------------------- */
void loop()
{
  create();
  int idf = getFingerprintIDez();
  if ( idf == -1 ) {
    //-----khi laststate = 0 hiển thị lỗi-----------
    if ( lastState == 0 && mainHome == 0) {
      lastState = -1;
      displayInvalidFinger();
      delay(2000);
      count++;
      screen();
    }
    else if (mainHome = 1) {
      lastState = -1;
      mainHome = 0;

    }
  }
  else if ( idf != -1) {
    displayFingerOK();
    screen();
    count = 0;
    mainHome = 1;

  }
  if (count == 3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("YOUR FINGERPRINT");
    lcd.setCursor(3,1);
    lcd.print("WAS LOCKED");
    lcd.setCursor(1,3);
    lcd.print("RETURN HOME...");
    delay(4000);
    count = 0;
    screen();
  }

  delay(50);

  //  char key = keypad.getKey();
  //  if (key) {
  //    Serial.println(key);
  //  }
  //  getFingerprintIDez();
  //  delay(50);
}
/* --------------------End hàm Loop------------------- */

//---------------hàm lấy dấu vân tay-----------------------

uint8_t getFingerprintEnroll() {
  digitalWrite(R, LOW);
  digitalWrite(G, LOW);
  digitalWrite(B, HIGH);
  int p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   SCANNING...");
  lcd.setCursor(0, 2);

  lcd.print("PUSH FINGERPRINT");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      case FINGERPRINT_IMAGEFAIL:
        lcd.clear();
        lcd.setCursor(5, 1);
        lcd.print("   ERROR");
        delay(2000);
        screen();
        break;
      default:
        lcd.clear();
        lcd.setCursor(5, 1);
        lcd.print("   ERROR");
        delay(2000);
        screen();
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      break;

    default:
      lcd.clear();
      lcd.setCursor(1, 5);
      lcd.print("ERROR");
      delay(2000);
      screen();
      return p;
  }
  lcd.clear();
  lcd.print("LIFT YOUR FINGER");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();
  lcd.print("   PUSH AGAIN");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        break;
      case FINGERPRINT_NOFINGER:
        break;
      default:
        lcd.setCursor(5, 1);
        lcd.print("ERROR");
        delay(2000);
        screen();
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("COULD NOT FIND");
      delay(2000);
      screen();
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("COULD NOT FIND");
      delay(2000);
      screen();
      return p;
    default:
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("COULD NOT FIND");
      delay(2000);
      screen();
      return p;
  }

  // OK converted!

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.setCursor(4, 1);
    lcd.print("NOT MATCH");
    lcd.setCursor(4, 3);
    lcd.print("TRY AGAIN");
    delay(3000);
    screen();
    return p;
  } else {
    return p;
  }

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    ///////////////////////////////
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("SUCCESS");
    lcd.setCursor(0, 2);
    lcd.print("ID=");
    lcd.setCursor(5, 2);
    lcd.print(id);
    delay(3000);
    screen();


    //////////////////////////////
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    return p;
  } else {
    return p;
  }
  return true;
}

//-------------kết thúc hàm kiểm tra------------------------

//-------------hàm khởi động----------------

void fgr() {
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  finger.begin(57600);
  if (finger.verifyPassword()) {
  } else { 
    while (1) {
      delay(1);
    }
  }
}
//------------hàm kiểm tra dấu vân tay------------

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);


  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #

//--------------------------------------------

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if ( p == 0 ) {
    lastState = 0;
  }
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("FINGERPRINT DOOR");
  lcd.setCursor(4, 2);
  lcd.print("  OPEN!");
  digitalWrite(4, HIGH);
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
  delay(1500);
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  lcd.clear();
  return finger.fingerID;
}

//--------------Kết thúc hàm kiểm tra dấu vân tay----------------

//-----------------------------

//--------------ham hiển thị lỗi nhập vân tay lên lcd

void displayInvalidFinger()
{
  lcd.clear();
  // lcd.setRGB(255, 0, 0);
  lcd.setCursor(5, 1);
  lcd.print("FINGER");
  lcd.setCursor(2, 2);
  lcd.print("NOT FOUND!!!");
}

// hàm hiển thị nhập vân tay thành công lên lcd

void displayFingerOK()
{
  digitalWrite(R, LOW);
  digitalWrite(G, HIGH);
  digitalWrite(B, LOW);
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("WELCOME ");
  lcd.setCursor(3, 2);
  lcd.print("DOOR UNLOCK");
  camera();
  delay(1500);
  lcd.clear();
  digitalWrite(4, LOW);
  digitalWrite(R, HIGH);
  digitalWrite(G, LOW);
  digitalWrite(B, LOW);
  mainHome = 1;
}
// hàm camera
void camera(){
  digitalWrite(12, HIGH);
  delay(2000);
  digitalWrite(12, LOW);
}
