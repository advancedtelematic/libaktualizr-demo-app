#include <memory>
#include <string>

#include "libaktualizr/packagemanagerfactory.h"
#include "libaktualizr/packagemanagerinterface.h"

#include "storage/invstorage.h"

#define CUSTOM_PACKAGE_MANAGER_NAME "pacman-example"

class PackageManagerExample : public PackageManagerInterface {
 public:
  PackageManagerExample(const PackageConfig &pconfig, const BootloaderConfig &bconfig,
                     const std::shared_ptr<INvStorage> &storage, const std::shared_ptr<HttpInterface> &http)
      : PackageManagerInterface(pconfig, bconfig, storage, http) {}

  std::string name() const override { return CUSTOM_PACKAGE_MANAGER_NAME; }
  Json::Value getInstalledPackages() const override;

  Uptane::Target getCurrent() const override;

  data::InstallationResult install(const Uptane::Target &target) const override;
  data::InstallationResult finalizeInstall(const Uptane::Target &target) override;
};

// libaktualizr will automatically pick this implementation of package manager,
// if pacman.type = CUSTOM_PACKAGE_MANAGER_NAME is specified in the config

// NOLINTNEXTLINE
AUTO_REGISTER_PACKAGE_MANAGER(CUSTOM_PACKAGE_MANAGER_NAME, PackageManagerExample);

// This call allows to return a list of installed packages,
// which will be visible in DDV on web UI
Json::Value PackageManagerExample::getInstalledPackages() const {
  Json::Value packages(Json::arrayValue);
  Json::Value package;
  package["name"] = "custom-package";
  package["version"] = "1.0";
  packages.append(package);
  return packages;
}

// This method should return a currently installed version for the Primary.
// If there was a previously installed update, its version will be stored
// by the SotaUptaneClient. On the first boot it is up to the implementation
// of the PackageManager to query it, or report as "unknown".
Uptane::Target PackageManagerExample::getCurrent() const {
  boost::optional<Uptane::Target> current_version;

  storage_->loadPrimaryInstalledVersions(&current_version, nullptr);

  return current_version ? *current_version : Uptane::Target::Unknown();
}

// This is the method where the actuall installation should happen.
// See aktualizr/include/libaktualizr/types.h for the list of possible return codes.
data::InstallationResult PackageManagerExample::install(const Uptane::Target& target) const {
  (void)target;

  std::cout << "Successfully installed " << target.filename() << " with an example Package Manager\n";
  return data::InstallationResult(data::ResultCode::Numeric::kOk, "Installing package was successful");
}

// This method will be called on the next libaktualizr initialization,
// e.g. after a reboot, if the `install` call returned kNeedCompletion result code.
// This is a normal workflow for the case when a reboot is required to apply an update.
data::InstallationResult PackageManagerExample::finalizeInstall(const Uptane::Target& target) {
  (void)target;
  return data::InstallationResult(data::ResultCode::Numeric::kOk, "Installing package was successful");

}

