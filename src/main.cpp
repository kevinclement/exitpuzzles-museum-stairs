#include <Arduino.h>
HardwareSerial mySerial(1);
static int8_t Send_buf[8] = {0};

#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_PLAY_W_VOL 0X22
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_PREVIOUS 0X02
#define CMD_NEXT 0X01

void sendCommand2(byte command, byte dat1, byte dat2)
{
  byte Send_buf[8] = {0}; // Buffer for Send commands.

  // Command Structure 0x7E 0xFF 0x06 CMD FBACK DAT1 DAT2 0xEF

  delay(20);
  Send_buf[0] = 0x7E;    // Start byte
  Send_buf[1] = 0xFF;    // Version
  Send_buf[2] = 0x06;    // Command length not including Start and End byte.
  Send_buf[3] = command; // Command
  Send_buf[4] = 0x01;    // Feedback 0x00 NO, 0x01 YES
  Send_buf[5] = dat1;    // DATA1 datah
  Send_buf[6] = dat2;    // DATA2 datal
  Send_buf[7] = 0xEF;    // End byte

  for(int i=0; i<8; i++)
  {
    mySerial.write(Send_buf[i]) ;
  }

  delay(500);  // Wait between sending commands.
}

void play(byte track)
{
  sendCommand2(0X08, 0, track);
}

void playOnce(byte track)
{
  sendCommand2(0X03, 0, track);
}


void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for(uint8_t i=0; i<8; i++)//
  {
    mySerial.write(Send_buf[i]) ;
  }
}

void setup() 
{
    Serial.begin(115200);
    Serial.println("mp3 player by kevinc...");

    // RX: 14
    // TX: 32
    mySerial.begin(9600, SERIAL_8N1, 14, 32);
    
    delay(500);//Wait chip initialization is complete
    sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card  
    delay(200);//wait for 200ms

    // set volume to 60%
    sendCommand(06, 0x3C);
}

String str;
void loop() {
  if(Serial.available())
  {
    str = Serial.readStringUntil('\n');
    if (str == "a") {
      Serial.println("advancing");
      playOnce(3);
      delay(2700);
      Serial.println("looping back");
      play(1);
    }
    if(str == "1")
    {
      Serial.println("1");
      play(1);
    }
    if(str == "2")
    {
      //sendCommand(CMD_PLAY_W_VOL, 0X1E04);//play the forth track with volume 30 class
      Serial.println("2");
      play(2);
    }
    if(str == "3")
    {
      //sendCommand(CMD_PLAY_W_VOL, 0X1E03);//play the third track with volume 30 class
      play(3);
      Serial.println("3");
    }
    if(str == "4")
    {
      //sendCommand(CMD_PLAY_W_VOL, 0X1E04);//play the forth track with volume 30 class
      Serial.println("4");
      play(4);
    }
    if(str == "ps")
    {
      sendCommand(CMD_PAUSE, 0X0E);//pause the playing track
      Serial.println("Pause");
    }
    if(str == "pl")
    {
      sendCommand(CMD_PLAY, 0X0D);//play it again 
      Serial.println("Play");
    }
    if(str == "pr")
    {
      sendCommand(CMD_PREVIOUS, 0X02);//play previous track
      Serial.println("Playing previous track.");
    }
    if(str == "nx")
    {
      sendCommand(CMD_NEXT, 0X01);//play next track
      Serial.println("Playing next track.");
    }
    if(str == "f")
    {
      sendCommand(CMD_PLAY_W_VOL, 0X1E0E);//play the second track with volume 30 class
      Serial.println("flame 1");
    }
    if(str == "g")
    {
      sendCommand(CMD_PLAY_W_VOL, 0X1E0F);//play the second track with volume 30 class
      Serial.println("flame 2");
    }
  }
}
