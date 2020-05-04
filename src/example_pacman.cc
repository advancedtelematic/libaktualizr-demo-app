#include <memory>
#include <string>

#include "package_manager/ostreemanager.h"
#include "package_manager/packagemanagerfactory.h"

#define PACKAGE_MANAGER "ostree-user"

class OstreeUserPacman : public OstreeManager {
 public:
  OstreeUserPacman(const PackageConfig &pconfig, const BootloaderConfig &bconfig,
                     const std::shared_ptr<INvStorage> &storage, const std::shared_ptr<HttpInterface> &http)
      : OstreeManager(pconfig, bconfig, storage, http) {}

  std::string name() const override { return PACKAGE_MANAGER; }
  std::string getCurrentHash() const override { return "aaaa"; }
};

AUTO_REGISTER_PACKAGE_MANAGER(PACKAGE_MANAGER, OstreeUserPacman);
