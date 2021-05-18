// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_
#include "boost/process.hpp"
#include "iostream"
#include <async++.h>
class builder{
 private:
  std::string config_;
  bool install_;
  bool pack_;
  int timeout_;
  boost::process::child process_;
 public:
  builder(std::string config, bool install, bool pack, int timeout):config_(config), install_(install), pack_(pack), timeout_(timeout){};
  void startBuild();
  bool executeCommand(std::string arguments);

};
#endif // INCLUDE_HEADER_HPP_
