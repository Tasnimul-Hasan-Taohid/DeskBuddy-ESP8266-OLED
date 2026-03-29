#include "message_manager.h"

const BuddyMessage MessageManager::_messages[] = {
  // line1,                  line2,                  faceIndex
  { "Good morning!",         "You got this today!",  0 },
  { "Take a deep",           "breath. You're ok.",   0 },
  { "I believe in",          "you! Always.",         2 },
  { "You're doing",          "amazing!",             5 },
  { "Don't forget",          "to smile today.",      1 },
  { "You matter",            "more than you know.",  2 },
  { "Rest is also",          "productive.",          0 },
  { "One step at",           "a time is fine.",      0 },
  { "You're my fav",         "person! Hehe.",        4 },
  { "Ohh Yeah",              "Yes Daddy",            5 },
  { "It's okay to",          "ask for help.",        0 },
  { "Your effort",           "doesn't go unseen.",   2 },
  { "Snack break?",          "show me your Dihh",    1 },
  { "Proud of you",          "for showing up.",      2 },
  { "Stretch a bit!",        "Your back matters.",   0 },
  { "Hard days make",        "you stronger.",        3 },
  { "Look how far",          "you've come!",         5 },
  { "Be kind to",            "yourself today.",      4 },
  { "You're enough,",        "just as you are.",     2 },
  { "Take it easy.",         "No rush at all.",      1 },
  { "Close your eyes",       "for 10 seconds.",      0 },
  { "Fuck You",              "Yes Fuck You!",        5 },
  { "Missing you...",        "just kidding lol.",    1 },
  { "Focus mode:",           "You can do it!",       5 },
  { "Good things",           "take time.",           0 },
};

const int MessageManager::_count = sizeof(_messages) / sizeof(_messages[0]);

MessageManager::MessageManager() : _index(0) {}

const BuddyMessage& MessageManager::current() {
  return _messages[_index];
}

void MessageManager::next() {
  _index = (_index + 1) % _count;
}

int MessageManager::count() {
  return _count;
}
