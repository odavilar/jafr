#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <linux/input.h>
#include <linux/joystick.h>

char *axis_names[ABS_MAX + 1] = {"X", "Y", "Z", "Rx", "Ry", "Rz", "Throttle", "Rudder", "Wheel", "Gas", "Brake", "?", "?", "?", "?", "?","Hat0X", "Hat0Y", "Hat1X", "Hat1Y", "Hat2X", "Hat2Y", "Hat3X", "Hat3Y","?", "?", "?", "?", "?", "?", "?" };

char *button_names[KEY_MAX - BTN_MISC + 1] = {"Btn0", "Btn1", "Btn2", "Btn3", "Btn4", "Btn5", "Btn6", "Btn7", "Btn8", "Btn9", "?", "?", "?", "?", "?", "?",	"LeftBtn", "RightBtn", "MiddleBtn", "SideBtn", "ExtraBtn", "ForwardBtn", "BackBtn", "TaskBtn", "?", "?", "?", "?", "?", "?", "?", "?",	"Trigger", "ThumbBtn", "ThumbBtn2", "TopBtn", "TopBtn2", "PinkieBtn", "BaseBtn", "BaseBtn2", "BaseBtn3", "BaseBtn4", "BaseBtn5", "BaseBtn6", "BtnDead","BtnA", "BtnB", "BtnC", "BtnX", "BtnY", "BtnZ", "BtnTL", "BtnTR", "BtnTL2", "BtnTR2", "BtnSelect", "BtnStart", "BtnMode", "BtnThumbL", "BtnThumbR", "?","?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?","WheelBtn", "Gear up" };

#define NAME_LENGTH 128

int ps3()
{
int fd, i;
unsigned char axes = 2;
unsigned char buttons = 2;
int version = 0x000800;
char name[NAME_LENGTH] = "Unknown";
uint16_t btnmap[KEY_MAX - BTN_MISC + 1];
uint8_t axmap[ABS_MAX + 1];

if ((fd = open("/dev/input/js0", O_RDONLY)) < 0) {
		perror("jstest");
		return 1;
	}

ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGAXES, &axes);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);
	ioctl(fd, JSIOCGAXMAP, axmap);
	ioctl(fd, JSIOCGBTNMAP, btnmap);

/*
 * Event interface, single line readout.
 */

		int *axis;
		char *button;
		struct js_event js;

		axis = (int*)calloc(axes, sizeof(int));
		button = (char*)calloc(buttons, sizeof(char));

		while (1) {
			if (read(fd, &js, sizeof(struct js_event)) != sizeof(struct js_event)) {
				perror("\njstest: error reading");
				return 1;
			}

			switch(js.type & ~JS_EVENT_INIT) {
			case JS_EVENT_BUTTON:
				button[js.number] = js.value;
				break;
			case JS_EVENT_AXIS:
				axis[js.number] = js.value;
				break;
			}

			printf("\r");

			if (axes) {
				printf("Axes: ");
				for (i = 0; i < axes; i++)
					printf("%2d:%6d ", i, axis[i]);
			}

			if (buttons) {
				printf("Buttons: ");
				for (i = 0; i < buttons; i++)
					printf("%2d:%s ", i, button[i] ? "on " : "off");
			}

			fflush(stdout);
		}
}
