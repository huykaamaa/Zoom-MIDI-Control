#include <usbh_midi.h>
#include <usbhub.h>
USB Usb;
USBH_MIDI  Midi(&Usb);
uint16_t pid, vid;

#include "OneButton.h"
OneButton button1(25, true);   //này nút câu vô chân nào thì mình để là chân đó, nếu ít nút hơn thì xoá bớt
OneButton button2(26, true); 
OneButton button3(27, true); 

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

void click1() {  //này là nhất nút 1 sẽ gởi midi tới zoom
  PC(0); // 0 là present đầu tiên cứ vậy tăng lên
}
void click2() {  //này là nhất nút 2 sẽ gởi midi tới zoom
  PC(5);
}
void click3() {  //tương tự
  PC(10);
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
