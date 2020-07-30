#include "custom-secondary.h"

#include <boost/algorithm/hex.hpp>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

#include "crypto/crypto.h"
#include "utilities/fault_injection.h"
#include "utilities/utils.h"

// This is a placeholder for the secondary installation procedure.
// It reads the whole file into memory to calculate size and hash,
// which is usually not desirable in a real use-case. 
bool custom_install(std::ifstream& str) {
  std::string data((std::istreambuf_iterator<char>(str)),
                       std::istreambuf_iterator<char>());
  std::cout << "Successfully installed the update for the custom secondary" << std::endl;
  std::cout << "Update length: " << data.length() << std::endl;
  std::cout << "Update hash: " << boost::algorithm::hex(Crypto::sha256digest(data)) << std::endl;
  return true;
}

// This is the method where the actuall installation should happen.
data::InstallationResult CustomSecondary::install(const Uptane::Target& target) {
  auto str = secondary_provider_->getTargetFileHandle(target);
  if (custom_install(str)) {
    Utils::writeFile(sconfig.target_name_path, target.filename());
    Utils::writeFile(sconfig.target_size_path, target.length());
    Utils::writeFile(sconfig.target_hash_path, target.sha256Hash());
    return data::InstallationResult(data::ResultCode::Numeric::kOk, "");
  }

  return data::InstallationResult(data::ResultCode::Numeric::kInstallFailed, "A custom error string");
}

// This method should return a currently installed version for the Secondary.
// Normally it can be queried from the Secondary itself, but here we just rely
// on the values previously stored in the `install` call.
bool CustomSecondary::getFirmwareInfo(Uptane::InstalledImageInfo& firmware_info) const {
  std::string content;

  if (!boost::filesystem::exists(sconfig.target_name_path)) {
    firmware_info.name = std::string("noimage");
  } else {
    firmware_info.name = Utils::readFile(sconfig.target_name_path.string());
  }
  if (!boost::filesystem::exists(sconfig.target_hash_path)) {
    firmware_info.hash = std::string("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
  } else {
    firmware_info.hash = Utils::readFile(sconfig.target_hash_path.string());
  }
  if (!boost::filesystem::exists(sconfig.target_size_path)) {
    firmware_info.len = 0;
  } else {
    firmware_info.len = std::stoi(Utils::readFile(sconfig.target_size_path.string()));
  }

  return true;
}

