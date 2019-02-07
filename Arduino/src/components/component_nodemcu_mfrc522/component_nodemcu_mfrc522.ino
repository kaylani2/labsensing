/*
 * Author: Kaylani Bochie
 * 
 * Reads a RFID tag and prints the UID on the serial monitor. That's it.
 */

 
/*    OUTPUTS AND CIRCUIT:

   Board used: Arduino Nano
   RFID reader board: rfid-rc522

   UID = Unique ID
   PICC = short for Proximity Integrated Circuit Card (RFID Tag itself)
   Based on the DumpInfo example of the MFRC522.h library



   The output of the DumpByteArray (mfrc522.uid.uidByte, mfrc522.uid.size) gives something like the following on the output:
      95 32 0D AB


   The output of the PICC_DumpToSerial (Uid *uid) method gives something like the following on the output:

    Card UID: 95 32 0D AB
    Card SAK: 08
    Tag type: MIFARE 1KB
    Sector Block   0  1  2  3   4  5  6  7   8  9 10 11  12 13 14 15  AccessBits
      15     63   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             62   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             61   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             60   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
      14     59   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             58   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             57   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             56   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
      13     55   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             54   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             53   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             52   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
      12     51   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             50   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             49   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             48   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
      11     47   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             46   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             45   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             44   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
      10     43   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             42   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             41   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             40   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       9     39   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             38   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             37   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             36   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       8     35   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             34   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             33   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             32   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       7     31   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             30   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             29   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             28   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       6     27   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             26   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             25   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             24   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       5     23   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             22   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             21   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             20   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       4     19   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             18   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             17   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             16   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       3     15   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             14   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             13   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
             12   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       2     11   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
             10   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
              9   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
              8   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       1      7   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
              6   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
              5   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
              4   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
       0      3   00 00 00 00  00 00 FF 07  80 69 FF FF  FF FF FF FF  [ 0 0 1 ]
              2   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
              1   00 00 00 00  00 00 00 00  00 00 00 00  00 00 00 00  [ 0 0 0 ]
              0   95 32 0D AB  01 08 04 00  62 63 64 65  66 67 68 69  [ 0 0 0 ]



   The output of the PICC_DumpDetailsToSerial(Uid *uid) method gives something like the following on the output:

    Card UID: 95 32 0D AB
    Card SAK: 08
    Tag type: MIFARE 1KB



  Typical pin layout used:
   ----------------------------------------------------------------------------------------------------
               MFRC522       Arduino        NodeMCU    Arduino   Arduino    Arduino          Arduino
               Reader/PCD    Uno/101        ESP8266    Mega      Nano v3    Leonardo/Micro   Pro Micro
   Signal      Pin           Pin            Pin        Pin       Pin        Pin              Pin
   ----------------------------------------------------------------------------------------------------
   RST/Reset   RST           9              D3 [1]      5         D9         RESET/ICSP-5     RST
   SPI SS      SDA(SS) [3]   10             D8 [2]      53        D10        10               10
   SPI MOSI    MOSI          11 / ICSP-4    D7          51        D11        ICSP-4           16
   SPI MISO    MISO          12 / ICSP-1    D6          50        D12        ICSP-1           14
   SPI SCK     SCK           13 / ICSP-3    D5          52        D13        ICSP-3           15

    [1] (1, 2) Configurable, typically defined as RST_PIN in sketch/program.
    [2] (1, 2) Configurable, typically defined as SS_PIN in sketch/program.
    [3] The SDA pin might be labeled SS on some/older MFRC522 boards
*/


// Other directives
#define SERIAL_BAUD_RATE                        115200

// Directives for the NodeMCU board
#define D0                                        16
#define D1                                        5
#define D2                                        4
#define D3                                        0
#define D4                                        2
#define D5                                        14
#define D6                                        12
#define D7                                        13
#define D8                                        15
#define D9                                        3
#define D10                                       1
#define BUILT_IN_LED                              D4
#define INDICATOR_LED                             D4

// Directives for the MFRC522
#include <SPI.h>
#include <MFRC522.h>
#define RESET_PIN                             D3
#define SLAVE_SELECT_PIN                      D8
#define OK                                    0
#define DIFFERENT_BYTE_ARRAYS                 1


// Global variables for the MFRC522
MFRC522 mfrc522 (SLAVE_SELECT_PIN, RESET_PIN);
// This object will hold the ID that will be read


/*
 * void DumpByteArray (byte *, byte);
 * 
 * Arguments:
 * byte * - Pointer to the byte array
 * byte - Length of the array
 * 
 * Description:
 * Prints a generic hex array on the serial monitor
 */
void DumpByteArray (byte *byteArray, byte bufferSize)
{
  for (unsigned char index = 0; index < bufferSize; index++)
  {
    Serial.print (byteArray [index] < 0x10 ? " 0" : " ");
    Serial.print (byteArray [index], HEX);
  }
  Serial.println ();
}

/*
 * int compareByteArrays (byte *, byte *, unsigned int);
 * 
 * Arguments:
 * byte * - First byte array
 * byte * - Second byte array
 * unsigned int - Length of the arrays
 * 
 * Description:
 * Compare each byte of the arrays. If they have the same content, returns OK. Othewise an error code is returned.
 * 
 * Return codes:
 * OK - The arrays are the same.
 * DIFFERENT_BYTE_ARRAY - The arrays are not the same.
 */
int compareByteArrays (byte *firstByteArray, byte *secondByteArray, unsigned int byteArrayLength)
{
  for (unsigned char index = 0; index < byteArrayLength; index++)
  {
    if (firstByteArray [index] != secondByteArray [index])
      return DIFFERENT_BYTE_ARRAYS;
  }

  return OK;
}


void setup ()
{
  Serial.begin (SERIAL_BAUD_RATE);
  pinMode (BUILT_IN_LED, OUTPUT);
  digitalWrite (BUILT_IN_LED, HIGH);

  SPI.begin ();
  mfrc522.PCD_Init ();
  Serial.println ("Ready to scan RFID tags and cards.");
}

void loop()
{
  if (mfrc522.PICC_IsNewCardPresent ())
  {
    //mfrc522.PICC_DumpToSerial (& (mfrc522.uid));
    //mfrc522.PICC_DumpDetailsToSerial (& (mfrc522.uid));
    if (mfrc522.PICC_ReadCardSerial ())
      DumpByteArray (mfrc522.uid.uidByte, mfrc522.uid.size);
  }
  
  // mfrc522.uid.uidByte [0] = 0xFF;
  // /\ Resets the object ID if needed
}
