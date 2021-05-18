// Copyright 2020 Your Name <your_email>
#include <async++.h>

#include <builder.hpp>
namespace bp = boost::process;

void builder::startBuild() {
  if (timeout_ != -1) {
    auto timer = async::spawn([this] {
      std::cout << "Start build" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(timeout_));
      if (process_.running()) {
        std::cout << "Time is exceed" << std::endl
                  << "Terminating building" << std::endl;
        process_.terminate();
      }
    });
  }
  bool success = executeCommand(
      "-H. -B_builds -DCMAKE_INSTALL_PREFIX=_install "
      "-DCMAKE_BUILD_TYPE=" +
      config_);
  if (!success) return;

  success = executeCommand("--build _builds");
  if (!success) return;

  if (install_ && pack_) {
    success = executeCommand("--build _builds --target install");
    if (!success) return;

    success = executeCommand("--build _builds --target package");
    if (!success) return;

  } else if (install_ && !pack_) {
    success = executeCommand("--build _builds --target install");
    if (!success) return;

  } else if (!install_ && pack_) {
    success = executeCommand("--build _builds --target package");
    if (!success) return;
  }
}

bool builder::executeCommand(const std::string& arguments) {
  bp::ipstream stream;
  auto cmake_path = boost::process::search_path("cmake");

  std::cout << std::endl
            << std::endl
            << "Executing command : " << std::endl
            << arguments << std::endl
            << std::endl;

  bp::child child(cmake_path.string() + " " + arguments,
                  boost::process::std_out > stream);
  process_ = std::move(child);

  for (std::string line; process_.running() && std::getline(stream, line);) {
    std::cout << line << std::endl;
  }

  process_.wait();

  auto exit_code = process_.exit_code();

  if (exit_code != 0) {
    std::cout << std::to_string(exit_code) + " exit code. Exiting ..."
              << std::endl;
    return false;
  } else {
    return true;
  }
}
