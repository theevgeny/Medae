# 🎮 Medae
> A game based on the concept of no built-in content.\
> Content is created by developers and uploaded to the server,\
> which then distributes resources and data to the client.

# 🛣️ Road Map
> [ ] - Base network (ping and connect with encryption and file sending)\
> [ ] - UI #13\
> [ ] - Renderer on OpenGL\
> [ ] - EnTT base components\
> [ ] - JoltPhysics integration\
> [ ] - Game loop with dt\
> [ ] - First WASM API

# 🚀 Getting Started
### Prerequisites 
> - CMake 3.20+
> - C++20 compatible compiler (GCC 11+, Clang 13+, MSVC 2019 v14.28+)
### Building the Project(Arch Linux)
```bash
git clone https://github.com/theevgeny/Medae.git
cd Medae

yay -S entt
sudo pacman -S cmake git boost glfw openal glm spdlog zstd tomlplusplus

make build # or
# make buildmt THREADS=N
# N is thread count to compile
```


# 🤝 Contributing
> Fork the repository.\
> Create a feature branch (git checkout -b feature/awesome-feature).\
> Commit your changes (git commit -m 'Add some feature').\
> Push to the branch (git push origin feature/awesome-feature).\
> Open a Pull Request.


# 📝 License
> This project is licensed under the MIT license.\
> See the LICENSE file for details.


### Used external libraries
| Library | License |
| - | - |
| GLAD | MIT |
| GLM | MIT |
| GLFW | ZLIB |
| EnTT | MIT |
| Jolt Physics | MIT |
| Boost | BSL |
| OpenAL | LGPL |
| zstd | BSD |
| spdlog | MIT |
| tomlplusplus | MIT |
| libsodium | ISC |
| DiligentEngine | Apache 2.0 |