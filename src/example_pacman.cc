#include <memory>
#include <string>

#include "package_manager/packagemanagerfactory.h"
#include "package_manager/packagemanagerinterface.h"

#define PACKAGE_MANAGER_EXAMPLE "example-pacman"

class PackageManagerExample : public PackageManagerInterface {
 public:
  PackageManagerExample(const PackageConfig &pconfig, const BootloaderConfig &bconfig,
                     const std::shared_ptr<INvStorage> &storage, const std::shared_ptr<HttpInterface> &http)
      : PackageManagerInterface(pconfig, bconfig, storage, http) {}

  std::string name() const override { return PACKAGE_MANAGER_EXAMPLE; }
  Json::Value getInstalledPackages() const override;

  Uptane::Target getCurrent() const override;

  data::InstallationResult install(const Uptane::Target &target) const override;
  data::InstallationResult finalizeInstall(const Uptane::Target &target) override;
};

AUTO_REGISTER_PACKAGE_MANAGER(PACKAGE_MANAGER_EXAMPLE, PackageManagerExample);

Json::Value PackageManagerExample::getInstalledPackages() const {
  Json::Value packages(Json::arrayValue);
  Json::Value package;
  package["name"] = "custom-package";
  package["version"] = "1.0";
  packages.append(package);
  return packages;
}

Uptane::Target PackageManagerExample::getCurrent() const {
  boost::optional<Uptane::Target> current_version;

  storage_->loadPrimaryInstalledVersions(&current_version, nullptr);

  return current_version ? *current_version : Uptane::Target::Unknown();
}

data::InstallationResult PackageManagerExample::install(const Uptane::Target& target) const {
  (void)target;

  return data::InstallationResult(data::ResultCode::Numeric::kOk, "Installing package was successful");
}

data::InstallationResult PackageManagerExample::finalizeInstall(const Uptane::Target& target) {
  (void)target;
  return data::InstallationResult(data::ResultCode::Numeric::kOk, "Installing package was successful");

}

