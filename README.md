# libaktualizr-demo-app

This is a sample application for integrating libaktualizr. The application is intented to be built as a part of a yocto build. The corresponding yocto layer can be found at <https://github.com/advancedtelematic/meta-libaktualizr-demo>.
This project uses git submodules. You should either clone it with `--recurse-submodules` flag or run `git submodule update --init --recursive` later.

To build and run the application locally:
```bash
mkdir build && cd build
cmake ..
make libaktualizr-demo-app
cp /path/to/credentials.zip ./credentials.zip
src/libaktualizr-demo-app -c ../aktualizr/config/sota-local.toml
```

## What does this do?

libaktualizr provides API calls to perform the various steps that are necessary for checking for updates, validating the files downloaded, and so on. This demo app is a simple command line app that acts as a kind of REPL for aktualizr. It takes a normal aktualizr config file and credentials file, registers/provisions with the server if it's not already been done, and then waits for commands.

The commands it can accept are all of the commands from the `aktualizr` class that are needed to check for, download, and install updates and campaigns:

* SendDeviceData
* CheckUpdates
* Download
* Install
* CampaignCheck
* CampaignAccept
* Pause
* Resume
* Abort

You can see a description of what each command does in the [aktualizr class reference in libaktualizr's Doxygen docs.](https://advancedtelematic.github.io/aktualizr/class_aktualizr.html).

## Step-by-step guide to the code

As this is intended only as a demo app, we hope the code should be fairly straightforward to understand. There are only three functions, each described below.

### parse_options

This is basic command line option parsing. Consult the [Boost docs](https://www.boost.org/doc/html/program_options.html) for more details.

### process_event

Aktualizr uses Boost's [Signals2 library](https://www.boost.org/doc/html/signals2.html) for event handling, to deliver status messages about update operations. API documentation of the possible events that can be sent is available in the [Doxygen docs for libaktualizr](https://advancedtelematic.github.io/aktualizr/namespaceevent.html). This method takes an event handler and logs events it receives on that handler to stdout.

### main

In main, all that is left to do is create our aktualizr object, initialize it using the config file specified, and then listen for commands on stdin.

## License

This code is licensed under the [Mozilla Public License 2.0](LICENSE), a copy of which can be found in this repository. All code is copyright HERE Europe B.V., 2019-2020.

We require that contributors accept the terms of Linux Foundation's [Developer Certificate of Origin](https://developercertificate.org/). Please see the [contribution instructions of aktualizr](https://github.com/advancedtelematic/aktualizr/blob/master/CONTRIBUTING.md) for more information.
