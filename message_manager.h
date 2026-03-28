#pragma once

#include <Arduino.h>

struct BuddyMessage {
  const char* line1;
  const char* line2;
  int faceIndex;  // 0=happy 1=wink 2=love 3=sad 4=blush 5=excited
};

class MessageManager {
public:
  MessageManager();

  const BuddyMessage& current();
  void next();
  int  count();

private:
  int _index;
  static const BuddyMessage _messages[];
  static const int _count;
};
