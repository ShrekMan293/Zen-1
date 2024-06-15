#include "mobo.h"

mobo_t controller = { (pm)0 };
mobo_t storage = { (pm)0 };
mobo_t audio = { (pm)0 };
mobo_t graphics = { (pm)0 };
mobo_t timer = { (pm)0 };
mobo_t rtc = { (pm)0 };
mobo_t usb = { (pm)0 };
mobo_t rng = { (pm)0 };
//mobo_t network = { (pm)0 };
mobo_t scu = { (pm)0 };

void motherboardMain() {

}

void initComponents() {
	controller.info.power = RUNNING;
	controller.info.base = 0x0000;
	controller.info.max = 0x1000;
	controller.info.DMA = 0;
	controller.thread = std::thread(motherboardMain);

	storage.info.power = IDLE;
}