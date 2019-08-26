# libaktualizr-demo-app

This is a sample application for integrating libaktualizr. The application is intented to be built as a part of a yocto build. The corresponding yocto layer can be found at <https://github.com/advancedtelematic/meta-libaktualizr-demo>.

To build and run the application locally:
```bash
# mkdir build && cd build
# cmake ..
# make libaktualizr-demo-app
# cp /path/to/credentials.zip ./credentials.zip
# src/libaktualizr-demo-app -c ../aktualizr/config/sota-local.toml
```
