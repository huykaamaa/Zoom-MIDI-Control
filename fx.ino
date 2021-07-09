//G1XON Untest
#include <usbh_midi.h>
#include <usbhub.h>

uint8_t request[] = {0xf0,0x7e,0x00,0x06,0x01,0xf7};
uint8_t edit[] = {0xF0,0x52,0x00,0x63,0x50,0xF7};   
//0x58 MS-50G
//0x5F MS-60B, 
//0x61 MS-70CDR
//0x59 G3X
//0x5A G3
//0x63 G1ON
//????G1XON?????? thử 0x60,0x62,0x6A, 0x6F
uint8_t fx1ON[] = {0xF0, 0x52, 0x00, 0x63, 0x31, 0x00, 0x00, 0x01, 0x00, 0xF7};
uint8_t fx1OFF[] = {0xF0, 0x52, 0x00, 0x63, 0x31, 0x00, 0x00, 0x00, 0x00, 0xF7};
uint8_t fx2ON[] = {0xF0, 0x52, 0x00, 0x63, 0x31, 0x01, 0x00, 0x01, 0x00, 0xF7};
uint8_t fx2OFF[] = {0xF0, 0x52, 0x00, 0x63, 0x31, 0x01, 0x00, 0x00, 0x00, 0xF7};
uint8_t fx3ON[] = {0xF0, 0x52, 0x00, 0x63, 0x31, 0x02, 0x00, 0x01, 0x00, 0xF7};
uint8_t fx3OFF[] = {0xF0, 0x52, 0x00, 0x63, 0x31, 0x02, 0x00, 0x00, 0x00, 0xF7};

USB Usb;
USBH_MIDI  Midi(&Usb);
uint16_t pid, vid;

#include "OneButton.h"
OneButton button1(25, true);   //này nút câu vô chân nào thì mình để là chân đó, nếu ít nút hơn thì xoá bớt
OneButton button2(26, true); 
OneButton button3(27, true); 
bool isOn=false;
void setup() {
  Serial.begin(115200);
  button1.attachClick(click1);
  button2.attachClick(click2);
  button3.attachClick(click3);
  vid = pid = 0;
  if (Usb.Init() == -1) {
    while (1);
  }
}

void loop() {
  Usb.Task();
  MIDI_poll();       
  button1.tick();    //nếu ít nút hơn thì xoá bớt
  button2.tick();
  button3.tick();
}

void PC(uint8_t n) {
  uint8_t buf[2];
  buf[0] = 0xc0;
  buf[1] = n;
  Midi.SendData(buf);
}

void click1() {  
  if (isOn) {
    Midi.SendSysEx(edit, sizeof(edit));
    Midi.SendSysEx(fx1OFF, sizeof(fx1OFF));
    isOn=false;
  } else {
    Midi.SendSysEx(edit, sizeof(edit));
    Midi.SendSysEx(fx1ON, sizeof(fx1ON));
    isOn=true;
  }
}
void click2() {  
  if (isOn) {
    Midi.SendSysEx(edit, sizeof(edit));
    Midi.SendSysEx(fx2OFF, sizeof(fx2OFF));
    isOn=false;
  } else {
    Midi.SendSysEx(edit, sizeof(edit));
    Midi.SendSysEx(fx2ON, sizeof(fx2ON));
    isOn=true;
  }
}
void click3() {  
  if (isOn) {
    Midi.SendSysEx(edit, sizeof(edit));
    Midi.SendSysEx(fx3OFF, sizeof(fx3OFF));
    isOn=false;
  } else {
    Midi.SendSysEx(edit, sizeof(edit));
    Midi.SendSysEx(fx3ON, sizeof(fx3ON));
    isOn=true;
  }
}
void MIDI_poll() {
  char buf[20];
  if (Midi.idVendor() != vid || Midi.idProduct() != pid) {
    vid = Midi.idVendor();
    pid = Midi.idProduct();
    sprintf(buf, "VID:%04X, PID:%04X", vid, pid);
    Serial.println(buf);
  }
}
