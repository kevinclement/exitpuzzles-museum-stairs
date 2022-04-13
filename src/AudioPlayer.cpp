#include "Arduino.h"
#include "AudioPlayer.h"
#include "logic.h"
HardwareSerial mySerial(1);

#define WHOSH_TRACK 1
#define IDLE_TRACK 3
#define SOLVED_TRACK 2
#define FAILURE_TRACK 6
#define WHOSH_TRACK_LENGTH 2700
#define SOLVED_TRACK_LENGTH 10000
#define IDLE_TRACK_LENGTH 5400

void sendCommand(byte command);
void sendCommand(byte command, byte dat2);
void sendCommand(byte command, byte dat1, byte dat2);

unsigned long whoosh_playing_at = 0;
unsigned long solved_playing_at = 0;

AudioPlayer::AudioPlayer(Logic &logic)
: _logic(logic)
{  
}

void AudioPlayer::setup() {
  // RX: 14 TX: 42
  mySerial.begin(9600, SERIAL_8N1, 14, 32);

  delay(500); // Wait chip initialization is complete
  sendCommand(0X09, 0X02); // select the TF card
  delay(200);

  idle();
}

void sendCommand(byte command) {
  sendCommand(command, 0, 0);
}

void sendCommand(byte command, byte dat2) {
  sendCommand(command, 0, dat2);
}

void sendCommand(byte command, byte dat1, byte dat2)
{
  byte Send_buf[8] = {0}; // Buffer for Send commands.

  // Command Structure 0x7E 0xFF 0x06 CMD FBACK DAT1 DAT2 0xEF

  delay(20);
  Send_buf[0] = 0x7E;    // Start byte
  Send_buf[1] = 0xFF;    // Version
  Send_buf[2] = 0x06;    // Command length not including Start and End byte.
  Send_buf[3] = command; // Command
  Send_buf[4] = 0x00;    // Feedback 0x00 NO, 0x01 YES
  Send_buf[5] = dat1;    // DATA1 datah
  Send_buf[6] = dat2;    // DATA2 datal
  Send_buf[7] = 0xEF;    // End byte

  for(int i=0; i<8; i++) {
    mySerial.write(Send_buf[i]) ;
  }
}

void AudioPlayer::idle() {

  // set volume
  sendCommand(0x06, _solved ? volume_low : volume_high);
  delay(20);

  // play on repeat
  sendCommand(0X08, IDLE_TRACK);

  whoosh_playing_at = 0;
  solved_playing_at = 0;
}

void AudioPlayer::levelUp() {
  whoosh_playing_at = millis();
  solved_playing_at = 0;

  // play at whosh volume once
  sendCommand(0x22, volume_whosh, WHOSH_TRACK);
}

void AudioPlayer::failure() {
  stop();
  
  // play at low volume once
  sendCommand(0x22, 0x14, FAILURE_TRACK);
}

void AudioPlayer::stop() {
  sendCommand(0x16, 0x00);
  whoosh_playing_at = 0;
  solved_playing_at = 0;
}

void AudioPlayer::solved() {
  _solved = true;
}

void AudioPlayer::handle() {

  // whosh finished playing
  if (whoosh_playing_at > 0 && millis() - whoosh_playing_at > WHOSH_TRACK_LENGTH - 1000) {
      if (_solved) {
        _logic.serial.print("audio: levelup played.  playing solved track.\r\n");
        solved_playing_at = millis();
        whoosh_playing_at = 0;
        sendCommand(0x22, volume_whosh, SOLVED_TRACK);
      }
      else if (millis() - whoosh_playing_at > WHOSH_TRACK_LENGTH) {
        _logic.serial.print("audio: levelup played not solved.  restarting idle.\r\n");
        whoosh_playing_at = 0;
        idle();
      }
  }
  else if (solved_playing_at > 0 && millis() - solved_playing_at > SOLVED_TRACK_LENGTH) {
    solved_playing_at = 0;
    _logic.serial.print("audio: levelup played.  restarting idle.\r\n");
    idle();
  }
}
