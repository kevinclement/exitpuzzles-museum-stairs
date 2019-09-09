#include "Arduino.h"
#include "AudioPlayer.h"
HardwareSerial mySerial(1);

#define WHOSH_TRACK_LENGTH 2700

void sendCommand(byte command);
void sendCommand(byte command, byte dat2);
void sendCommand(byte command, byte dat1, byte dat2);

unsigned long playing_song_at = 0;


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

  // set volume to 60%
  sendCommand(06, 0x32);
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
  Send_buf[4] = 0x01;    // Feedback 0x00 NO, 0x01 YES
  Send_buf[5] = dat1;    // DATA1 datah
  Send_buf[6] = dat2;    // DATA2 datal
  Send_buf[7] = 0xEF;    // End byte

  for(int i=0; i<8; i++) {
    mySerial.write(Send_buf[i]) ;
  }
}

void play(byte track) {
  sendCommand(0X08, track);
}

void playOnce(byte track) {
  sendCommand(0X03, track);
}

void AudioPlayer::idle() {
  play(1);
}

void AudioPlayer::levelUp() {
  playing_song_at = millis();
  playOnce(3);
}

void AudioPlayer::stop() {
  sendCommand(0x16, 0x00);
  playing_song_at = 0;
}

void AudioPlayer::handle() {
  if (playing_song_at > 0 && millis() - playing_song_at > WHOSH_TRACK_LENGTH) {
    Serial.println("Audio: levelup played.  restarting idle.");
    playing_song_at = 0;
    idle();
  }
}
