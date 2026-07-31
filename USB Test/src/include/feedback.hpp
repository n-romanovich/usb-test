#pragma once

#include "messages.hpp"

void collectFeedback(int dialogAnswer);
int displayFeedbackDialog();
bool checkMsgAlreadyShown();
void editRegistry();
void sendFeedback(std::string feedback);