/**
 * @file      DisplayRotation.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <WiFi.h>

#define MAX 80
#define PORT 5555
#define SERVER_IP "192.168.0.40" 

const char* ssid = "Mejdi@Telkos"; 
const char* password = "00000000";

WiFiClient client;

bool isPmuIRQ = false;
uint8_t rotation = 2;

void func()
{
    char buff[MAX];
    int n;
    for (;;) {
        memset(buff, 0, sizeof(buff));
        Serial.print("Enter the string: ");
        n = 0;
        while ((buff[n++] = Serial.read()) != '\n' && n < MAX)
            ;
        client.write((uint8_t*)buff, n);
        memset(buff, 0, sizeof(buff));
        client.read((uint8_t*)buff, sizeof(buff));
        Serial.print("From Server: ");
        Serial.println(buff);
        if ((strncmp(buff, "exit", 4)) == 0) {
            Serial.println("Client Exit...");
            break;
        }
    }
}

void setRotation()
{
    watch.setRotation(++rotation);
    rotation %= 4;
    updateLvglHelper();
    watch.setWaveform(0, 10);
    watch.run();
}

void up_button_handler(lv_event_t *e)
{
    Serial.println("UP button clicked");
}

void left_button_handler(lv_event_t *e)
{
    Serial.println("LEFT button clicked");
}

void right_button_handler(lv_event_t *e)
{
    Serial.println("RIGHT button clicked");
}

void down_button_handler(lv_event_t *e)
{
    Serial.println("DOWN button clicked");
}

void setup(void)
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    WiFi.begin(ssid, password);
    if (!client.connect(SERVER_IP, PORT)) {

    } else {
        Serial.println("Connected to server");
        func();
    }

    watch.attachPMU([]() {
        isPmuIRQ = true;
    });

    watch.setRotation(rotation);

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_t *btn3 = lv_btn_create(lv_scr_act());
    lv_obj_t *btn4 = lv_btn_create(lv_scr_act());

#if LV_VERSION_CHECK(9,0,0)
    lv_obj_add_event(btn1, up_button_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event(btn2, left_button_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event(btn3, right_button_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event(btn4, down_button_handler, LV_EVENT_CLICKED, NULL);
#else
    lv_obj_add_event_cb(btn1, up_button_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn2, left_button_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn3, right_button_handler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn4, down_button_handler, LV_EVENT_CLICKED, NULL);
#endif

    lv_obj_set_width(btn1, LV_PCT(40));   //up
    lv_obj_set_height(btn1, LV_PCT(40));
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -70);

    lv_obj_set_width(btn2, LV_PCT(40));   //left
    lv_obj_set_height(btn2, LV_PCT(40));
    lv_obj_align(btn2, LV_ALIGN_CENTER, -70, 0);

    lv_obj_set_width(btn3, LV_PCT(40));   //right
    lv_obj_set_height(btn3, LV_PCT(40));
    lv_obj_align(btn3, LV_ALIGN_CENTER, 70, 0);

    lv_obj_set_width(btn4, LV_PCT(40));   //down
    lv_obj_set_height(btn4, LV_PCT(40));
    lv_obj_align(btn4, LV_ALIGN_CENTER, 0, 70);

    lv_obj_t *label1;
    label1 = lv_label_create(btn1);
    lv_label_set_text(label1, "UP");
    lv_obj_center(label1);

    lv_obj_t *label2;
    label2 = lv_label_create(btn2);
    lv_label_set_text(label2, "LEFT");
    lv_obj_center(label2);

    lv_obj_t *label3;
    label3 = lv_label_create(btn3);
    lv_label_set_text(label3, "RIGHT");
    lv_obj_center(label3);

    lv_obj_t *label4;
    label4 = lv_label_create(btn4);
    lv_label_set_text(label4, "DOWN");
    lv_obj_center(label4);
}

void loop()
{
    if (isPmuIRQ) {
        isPmuIRQ = false;
        watch.readPMU();
        if (watch.isPekeyShortPressIrq()) {
            setRotation();
        }
        watch.clearPMU();
    }

    lv_point_t point;
    lv_indev_t *indev = lv_indev_get_next(NULL);
    if (indev) {
#if LV_VERSION_CHECK(9,0,0)
        if (lv_indev_get_state(indev) == LV_INDEV_STATE_PRESSED) {
#else
        if (indev->proc.state == LV_INDEV_STATE_PRESSED) {
#endif
            lv_indev_get_point(indev, &point);
        }
    }
    lv_task_handler();
    delay(5);
}
