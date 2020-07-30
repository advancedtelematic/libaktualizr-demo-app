#ifndef CUSTOM_SECONDARY_H_
#define CUSTOM_SECONDARY_H_

#include <string>

#include "managedsecondary.h"
#include "libaktualizr/types.h"

class CustomSecondaryConfig : public Primary::ManagedSecondaryConfig {
  public:
    CustomSecondaryConfig() : ManagedSecondaryConfig("custom") {}
    CustomSecondaryConfig(const Json::Value& json_config): ManagedSecondaryConfig("custom") {
      ecu_serial = json_config["ecu_serial"].asString();
      ecu_hardware_id = json_config["ecu_hardware_id"].asString();
      full_client_dir = json_config["full_client_dir"].asString();
      ecu_private_key = json_config["ecu_private_key"].asString();
      ecu_public_key = json_config["ecu_public_key"].asString();
      target_name_path = json_config["target_name_path"].asString();
      target_size_path = target_name_path.string() + ".size";
      target_hash_path = target_name_path.string() + ".hash";
      metadata_path = json_config["metadata_path"].asString();
    }

    boost::filesystem::path target_size_path;
    boost::filesystem::path target_hash_path;
};

class CustomSecondary : public Primary::ManagedSecondary {
  public:
    explicit CustomSecondary(CustomSecondaryConfig sconfig_in) : ManagedSecondary(sconfig_in), sconfig(sconfig_in) {}
    ~CustomSecondary() override = default;

    data::InstallationResult install(const Uptane::Target& target) override;

    std::string Type() const override { return "custom"; }
    bool ping() const override { return true; }

  protected:
    CustomSecondaryConfig sconfig;

  private:
    bool getFirmwareInfo(Uptane::InstalledImageInfo& firmware_info) const override;
};

#endif  // CUSTOM_SECONDARY_H_
