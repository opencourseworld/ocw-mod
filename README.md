# OpenCourseWorld client mod

A mod for Mario Maker 2 that allows connecting to OCW servers.
Based on [exlaunch](https://github.com/shadowninja108/exlaunch) by shadowninja108.

# Building
The best way to build is with Docker. This is cross-compilation for the Switch, and we need some very specific libraries and python stuff set up. Simply `./build.sh` or:

```
docker compose run --rm build make
```

This should result in a file `deploy/ocw-mod.zip`, which can be extracted to your SD card or virtual SD card to apply the mod.
