#ifndef JAMMER_CONTROLLER_H
#define JAMMER_CONTROLLER_H

#include <M5StickCPlus.h>

void initJammer();
void startJammer();
void stopJammer();
void renderJammerUI();
void handleJammerSelect();
void loopJammer();

#endif