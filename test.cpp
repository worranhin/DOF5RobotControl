#include "D5Robot.h"

int main() {

  std::string port = "COM3";
  std::string natorID = "usb:id:7547982319";
  D5R::D5Robot robot(port.c_str(), natorID, D5R::ID_01, D5R::ID_02);

  if (!robot.IsInit()) {
    ERROR_("Failed to init robot!");
    return -1;
  }
}