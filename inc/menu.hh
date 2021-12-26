#pragma once

#include <string>

#include "console.hh"
#include "platform.hh"

static const std::string menu_main[]{
    "Add a student",     "Find a student",    "Import from database",
    "Show the database", "Sync the database", "Account settings",
    "Exit and Save",
};

static const std::string menu_find[]{
    "Find student name",
    "Find student number",
    "Find student score",
    "Return to main menu",
};

static const std::string menu_user[]{
    "Show my scores",
    "Account settings",
    "Loging out and Exit",
};

static const std::string settings[]{
    "Change my password",
    "Return to main menu",
};

static const std::string manage_user[]{
    "Modify user infomation",
    "Delete this user",
    "Cancel and Return",
};

static auto menu(const std::string *str, size_t num) -> void {
  printf(SGR_BLACK_BACKGROUND SGR_WHITE_FOREGROUND "\n");
  CLEAR();

  printf(SGR_GREEN_BACKGROUND "%50c\n " SGR_RESET_ALL
                              "%48c" SGR_GREEN_BACKGROUND " \n " SGR_RESET_ALL,
         ' ', ' ');

  for (size_t i{}; i < num; ++i) {
    std::string tmp(42 - str[i].size(), ' ');
    printf("%3c%lu. %s%s" SGR_GREEN_BACKGROUND " \n " SGR_RESET_ALL, ' ',
           (i + 1) % num, str[i].c_str(), tmp.c_str());
  }

  printf("%48c" SGR_GREEN_BACKGROUND
         " \n%50c\n" SGR_BLACK_BACKGROUND SGR_WHITE_FOREGROUND,
         ' ', ' ');
}
