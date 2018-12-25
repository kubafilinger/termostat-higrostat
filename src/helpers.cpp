#include "helpers.h"

String convertMillisToTime() {
    int hours = millis() / (1000 * 60 * 60);
    int days = hours / 24;

    hours -= days * 24;

    return String(String(days) + "d " + String(hours) + "h");
}

String getDate() {
    long seconds = millis() / 1000;
    short hours = seconds / (60 * 60);

    seconds -= hours * 60 * 60;

    short minutes = seconds / 60;

    seconds -= minutes * 60;

    String date = String(String(hours) + ":" + String(minutes) + ":" + String(seconds));

    return date;
}