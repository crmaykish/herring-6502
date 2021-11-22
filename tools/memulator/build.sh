#!/bin/sh
# Open a Docker container and build the Pico project

docker run --user colin -v /home/colin/Workspace:/home/colin/Workspace -it picodev bash /home/colin/Workspace/herring-6502/tools/memulator/build_internal.sh