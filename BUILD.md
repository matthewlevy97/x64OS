# Build Instructions
1. Install Docker : [docker.com](https://docs.docker.com/get-docker/)
2. Run `./d whoami`
    * When prompted, enter **Y** to build the Docker container
3. Build kernel
    * `./d make`
4. Run kernel normally
    * `./d run`
5. Run with a debugger
    * Terminal 1: `./d qemu`
    * Terminal 2: `./d gdb`

# Why Docker
The main reason Docker is used in the build process is to ensure that development of the kernel can be done on any OS while keeping the same coding environment. This allows developers on use any device they want and still know that the kernel compiled on once machine wil compile that exact same way on another. Furthermore, it helps eliminiate problems during the build process as there should be no conflicting dependencies / all dependencies should be resolved inside the container.
