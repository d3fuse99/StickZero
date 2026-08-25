#ifndef DEAUTH_CONTROLLER_H
#define DEAUTH_CONTROLLER_H

#include <M5StickCPlus.h>
#include <vector>

void initDeauth();
void startDeauthAttack();
void stopDeauthAttack();
void scanNetworks();
void selectTarget(int index);
void renderDeauthUI();
void handleDeauthSelect();
void loopDeauth();

#endif