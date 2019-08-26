#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/signals2.hpp>

#include "config/config.h"
#include "logging/logging.h"
#include "primary/aktualizr.h"
#include "utilities/utils.h"

namespace bpo = boost::program_options;

bpo::variables_map parse_options(int argc, char *argv[]) {
  bpo::options_description description("A simple wrapper for libaktualizr");
  description.add_options()
      ("config,c", bpo::value<std::vector<boost::filesystem::path> >()->composing(), "configuration file or directory")
      ("help,h", "print help message")
      ("secondary-configs-dir", bpo::value<boost::filesystem::path>(), "directory containing seconday ECU configuration files")
      ("loglevel", bpo::value<int>(), "set log level 0-5 (trace, debug, info, warning, error, fatal)");

  bpo::variables_map vm;
  std::vector<std::string> unregistered_options;
  bpo::basic_parsed_options<char> parsed_options =
    bpo::command_line_parser(argc, argv).options(description).allow_unregistered().run();
  bpo::store(parsed_options, vm);
  bpo::notify(vm);
  unregistered_options = bpo::collect_unrecognized(parsed_options.options, bpo::include_positional);

  if (vm.count("help") != 0 || !unregistered_options.empty()) {
    std::cout << description << "\n";
    exit(EXIT_FAILURE);
  }
  return vm;
}

void process_event(const std::shared_ptr<event::BaseEvent> &event) {
  static std::map<std::string, unsigned int> progress;

  if (event->isTypeOf(event::DownloadProgressReport::TypeName)) {
    const auto download_progress = dynamic_cast<event::DownloadProgressReport *>(event.get());
    if (progress.find(download_progress->target.sha256Hash()) == progress.end()) {
      progress[download_progress->target.sha256Hash()] = 0;
    }
    const unsigned int prev_progress = progress[download_progress->target.sha256Hash()];
    const unsigned int new_progress = download_progress->progress;
    if (new_progress > prev_progress) {
      progress[download_progress->target.sha256Hash()] = new_progress;
      std::cout << "Download progress for file " << download_progress->target.filename() << ": " << new_progress
                << "%\n";
    }
  } else if (event->variant == "DownloadTargetComplete") {
    const auto download_complete = dynamic_cast<event::DownloadTargetComplete *>(event.get());
    std::cout << "Download complete for file " << download_complete->update.filename() << ": "
              << (download_complete->success ? "success" : "failure") << "\n";
    progress.erase(download_complete->update.sha256Hash());
  } else if (event->variant == "InstallStarted") {
    const auto install_started = dynamic_cast<event::InstallStarted *>(event.get());
    std::cout << "Installation started for device " << install_started->serial.ToString() << "\n";
  } else if (event->variant == "InstallTargetComplete") {
    const auto install_complete = dynamic_cast<event::InstallTargetComplete *>(event.get());
    std::cout << "Installation complete for device " << install_complete->serial.ToString() << ": "
              << (install_complete->success ? "success" : "failure") << "\n";
  } else if (event->variant == "UpdateCheckComplete") {
    const auto check_complete = dynamic_cast<event::UpdateCheckComplete *>(event.get());
    std::cout << check_complete->result.updates.size() << " updates available\n";
  } else {
    std::cout << "Received " << event->variant << " event\n";
  }
}

int main(int argc, char *argv[]) {
  logger_init();
  logger_set_threshold(boost::log::trivial::info);
  LOG_INFO << "demo-app starting";

  try {
    bpo::variables_map commandline_map = parse_options(argc, argv);
    Config config(commandline_map);

    Aktualizr aktualizr(config);

    auto f_cb = [](const std::shared_ptr<event::BaseEvent> event) { process_event(event); };
    boost::signals2::scoped_connection conn(aktualizr.SetSignalHandler(f_cb));

    aktualizr.Initialize();

    const char *cmd_list = "Available commands: SendDeviceData, CheckUpdates, Download, Install, CampaignCheck, CampaignAccept, Pause, Resume, Abort";
    std::cout << cmd_list << std::endl;

    std::vector<Uptane::Target> current_updates;
    std::string buffer;
    while (std::getline(std::cin, buffer)) {
      std::vector<std::string> words;
      boost::algorithm::split(words, buffer, boost::is_any_of("\t "), boost::token_compress_on);
      std::string &command = words[0];
      std::string &param = words[1];
      boost::algorithm::to_lower(command);
      if (command == "senddevicedata") {
        aktualizr.SendDeviceData().get();
      } else if (command == "checkupdates") {
        auto result = aktualizr.CheckUpdates().get();
        current_updates = result.updates;
      } else if (command == "download") {
        aktualizr.Download(current_updates).get();
      } else if (command == "install") {
        aktualizr.Install(current_updates).get();
        current_updates.clear();
      } else if (command == "campaigncheck") {
        aktualizr.CampaignCheck().get();
      } else if (command == "campaignaccept") {
        aktualizr.CampaignControl(param, campaign::Cmd::Accept);
      } else if (command == "pause") {
        aktualizr.Pause();
      } else if (command == "resume") {
        aktualizr.Resume();
      } else if (command == "abort") {
        aktualizr.Abort();
      } else if (!command.empty()) {
        std::cout << "Unknown command.\n";
        std::cout << cmd_list << std::endl;
      }
    }
    return EXIT_SUCCESS;
  } catch (const std::exception &ex) {
    LOG_ERROR << "Fatal error in demo-app: " << ex.what();
    return EXIT_FAILURE;
  }
}
