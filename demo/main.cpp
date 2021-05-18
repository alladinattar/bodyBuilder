#include <boost/program_options.hpp>
#include <builder.hpp>

#include "iostream"
using namespace boost::program_options;

int main(int argc, char **argv) {
  options_description desc("Usage: builder [options]");
  std::string task_type;
  desc.add_options()
      ("help", "выводит вспомогательное сообщение")
      ("config", value<std::string>()->default_value("Debug"),  "указывает конфигурацию сборки (по умолчанию Debug)")
      ("install", "добавляет этап установки (в директорию _install)")
      ("pack", "добавляем этап упаковки (в архив формата tar.gz)")


      ("timeout", value<int>()->default_value(-1), "указывает время ожидания (в секундах)");
  variables_map vm;
  try {
    parsed_options parsed = command_line_parser(argc, argv).options(desc).allow_unregistered().run();
    store(parsed, vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }

    std::string config = vm["config"].as<std::string>();
    bool install = false;
    bool pack = false;
    uint32_t timeout = vm["timeout"].as<int32_t>();;

    if (vm.count("install"))
      install = true;

    if (vm.count("pack"))
      pack = true;
    try {
      builder bldsr = builder(config, install, pack, timeout);
      bldsr.startBuild();
    }
    catch (std::exception &ex) {
      std::cout << ex.what() << std::endl;
      return 1;
    }
  }
  catch (std::exception &ex) {
    std::cout << ex.what() << std::endl;
    return 1;
  }

}