/* Arduino JPEGCamera Library
 * Copyright 2010 SparkFun Electronic
 * Written by Ryan Owens
 * Modified by arms22
 * Ported to mbed by yamaguch
 */

#include "JPEGCamera.h"

#define min(x, y) ((x) < (y)) ? (x) : (y)

const int RESPONSE_TIMEOUT = 500;
const int DATA_TIMEOUT = 1000;

JPEGCamera::JPEGCamera(PinName tx, PinName rx) : Serial(tx, rx) {
    baud(38400);
    state = READY;
}

bool JPEGCamera::setPictureSize(JPEGCamera::PictureSize size, bool doReset) {
    char buf[9] = {0x56, 0x00, 0x31, 0x05, 0x04, 0x01, 0x00, 0x19, (char) size};
    int ret = sendReceive(buf, sizeof buf, 5);
    if (ret == 5 && buf[0] == 0x76) {
        if (doReset)
            reset();
        return true;
    } else
        return false;
}

bool JPEGCamera::isReady() {
    return state == READY;
}

bool JPEGCamera::isProcessing() {
    return state == PROCESSING;
}


bool JPEGCamera::takePicture() {
    char buf[5] = {0x56, 0x00, 0x36, 0x01, 0x00};
    int ret = sendReceive(buf, sizeof buf, 5);

    return ret == 5 && buf[0] == 0x76;
}

bool JPEGCamera::takePicture(char *filename) {
    if (state == READY) {
        fp = fopen(filename, "wb");
        if (fp != 0) {
            if (takePicture()) {
                imageSize = getImageSize();
                address = 0;
                state = PROCESSING;
            } else {
                fclose(fp);
                printf("takePicture(%s) failed", filename);
                state = ERROR;
            }
        } else {
            printf("fopen() failed");
            state = ERROR;
        }
    }
    return state != ERROR;
}

bool JPEGCamera::processPicture() {
    if (state == PROCESSING) {
        if (address < imageSize) {
            char data[1024];
            int size = readData(data, min(sizeof(data), imageSize - address), address);
            int ret = fwrite(data, size, 1, fp);
            if (ret > 0)
                address += size;
            if (ret == 0 || address >= imageSize) {
                stopPictures();
                fclose(fp);
                wait(0.1); // ????
                state = ret > 0 ? READY : ERROR;
            }
        }
    }

    return state == PROCESSING || state == READY;
}

bool JPEGCamera::reset() {
    char buf[4] = {0x56, 0x00, 0x26, 0x00};
    int ret = sendReceive(buf, sizeof buf, 4);
    if (ret == 4 && buf[0] == 0x76) {
        wait(4.0);
        state = READY;
    } else {
        state = ERROR;
    }
    return state == READY;
}

bool JPEGCamera::stopPictures() {
    char buf[5] = {0x56, 0x00, 0x36, 0x01, 0x03};
    int ret = sendReceive(buf, sizeof buf, 5);

    return ret == 5 && buf[0] == 0x76;
}

int JPEGCamera::getImageSize() {
    char buf[9] = {0x56, 0x00, 0x34, 0x01, 0x00};
    int ret = sendReceive(buf, sizeof buf, 9);

    //The size is in the last 2 characters of the response.
    return (ret == 9 && buf[0] == 0x76) ? (buf[7] << 8 | buf[8]) : 0;
}

int JPEGCamera::readData(char *dataBuf, int size, int address) {
    char buf[16] = {0x56, 0x00, 0x32, 0x0C, 0x00, 0x0A, 0x00, 0x00,
                    address >> 8, address & 255, 0x00, 0x00, size >> 8, size & 255, 0x00, 0x0A
                   };
    int ret = sendReceive(buf, sizeof buf, 5);

    return (ret == 5 && buf[0] == 0x76) ? receive(dataBuf, size, DATA_TIMEOUT) : 0;
}

int JPEGCamera::sendReceive(char *buf, int sendSize, int receiveSize) {
    while (readable()) getc();

    for (int i = 0; i < sendSize; i++) putc(buf[i]);

    return receive(buf, receiveSize, RESPONSE_TIMEOUT);
}

int JPEGCamera::receive(char *buf, int size, int timeout) {
    timer.start();
    timer.reset();

    int i = 0;
    while (i < size && timer.read_ms() < timeout) {
        if (readable())
            buf[i++] = getc();
    }

    return i;
}