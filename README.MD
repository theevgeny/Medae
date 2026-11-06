# 🎮 Medae
![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)
![C++](https://img.shields.io/badge/C%2B%2B-23-blue.svg?style=flat-square)
![CMake](https://img.shields.io/badge/CMake-3.20%2B-blue.svg?style=flat-square)
![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows-lightgrey.svg?style=flat-square)
![WebAssembly](https://img.shields.io/badge/WebAssembly-Wasmtime%20%7C%20Wasm3-purple.svg?style=flat-square)
> A game based on the concept of no built-in content.\
> Content is created by developers and uploaded to the server,\
> which then distributes resources and data to the client.

## 📖 About the Project
**Medae** is a modular, data-driven game engine and runtime designed around the idea that
*the engine ships empty* — no built-in assets, scenes, or gameplay logic. Instead, all
content lives on the server and is streamed to clients on demand. This makes the engine
ideal for building highly dynamic, live-service style games where content can be added,
patched, or replaced without shipping a new client build.

Open [Wiki](https://github.com/theevgeny/Medae/wiki)

### Core principles
- **Zero built-in content** — the client is a universal runtime; the world comes from the server.
- **Server-authoritative distribution** — resources, manifests, and metadata are pushed as binary data.
- **Extensibility via WASM** — gameplay logic is loaded as sandboxed WebAssembly modules (JIT through `wasmtime`).
- **Composable architecture** — clean separation between networking, rendering, UI, ECS, and physics layers.
- **Performance-first** — delta-timestep game loop, binary packet formats, and GPU-friendly resource pipeline.

# 🛣️ Road Map

- [x] Base network layer (UDP, packet struct, encryption, ack/nack, file transfer)
- [x] UI system template (canvas, panel, label, handlers) — *without rendering*
- [x] Asset management (managers, handles, loaders)
- [ ] Renderer core (context, buffers, shaders, textures)
- [ ] Resource pipeline: load → cache → handle → GPU resource
- [ ] UI integration to graphics — *all the "magic" of UI lives here*
- [ ] Base input system (keyboard, mouse)
- [ ] Game loop with fixed timestep
- [ ] WASM API (basic runtime: memory, host calls, module registration)
- [ ] WASM support — **JIT via wasmtime only** (AOT/interpreter later)
- [ ] Server ↔ client connection + identity:
  - [ ] 256-bit unique player ID and 256-bit auth key
  - [ ] Changeable nickname
- [ ] Basic handshake: identify → auth → session token
- [ ] Packet transformation layer:
  - [ ] Serialization "normal format → binary" on the server
  - [ ] Binary format parser on the client
- [ ] Manifest packets (resource list + metadata)
- [ ] Streaming resource transfer (binary, chunks, resume/retry over ack/nack)
- [ ] Scene tree / scene graph — *world rendering is hardcoded onto the scene*
  - [ ] Per-object shader selection (the only customization point)
- [ ] **Closing the foundational WASM API**: game loop + network + connect
- [ ] EnTT ECS: base components (transform, renderable, collider, input)
- [ ] ECS systems: render sorting, transform update, network sync
- [ ] Physics (basic: AABB / capsule, gravity, collisions)
- [ ] Make client UI for connect screen, settings screen, main screen
- [ ] Scene management / entity lifecycle (spawn/despawn, scene streaming)
- [ ] Sound engine
- [ ] Translation system (strings, language switching)
- [ ] Keybind configuration (UI + config persistence)

# 🚀 Getting Started
### Prerequisites 
> - CMake 3.20+
> - C++23 compatible compiler (GCC 13+, Clang 16+, MSVC 2022 v17.4+)
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
Fork the repository.\
Create a feature branch (git checkout -b feature/awesome-feature).\
Commit your changes (git commit -m 'Add some feature').\
Push to the branch (git push origin feature/awesome-feature).\
Open a Pull Request.


# 📝 License
This project is licensed under the MIT license.\
See the LICENSE file for details.


# 📦 Dependencies

Medae is built on top of carefully selected, permissively-licensed open-source libraries.

### 🧩 Core & Infrastructure

| Library  | Purpose | License | Links |
| :------ | :------ | :-----: | :---- |
| **EnTT** | Header-only ECS (entity-component-system) | ![MIT](https://img.shields.io/badge/License-MIT-yellow.svg) | [GitHub](https://github.com/skypjack/entt) |
| **Jolt Physics**  | Deterministic 3D physics & collision detection | ![MIT](https://img.shields.io/badge/License-MIT-yellow.svg) | [GitHub](https://github.com/jrouwe/JoltPhysics) |
| **DiligentEngine**  | Cross-platform rendering abstraction (D3D12 / Vulkan / OpenGL) | ![Apache 2.0](https://img.shields.io/badge/License-Apache%202.0-blue.svg) | [GitHub](https://github.com/DiligentGraphics/DiligentEngine) |
| **Boost**  | Asio networking, filesystem, utilities | ![BSL-1.0](https://img.shields.io/badge/License-BSL--1.0-green.svg) | [boost.org](https://www.boost.org/) |

### 🌐 Security

| Library  | Purpose | License | Links |
| :------ | :------ | :-----: | :---- |
| **libsodium**  | Encryption library | ![ISC](https://img.shields.io/badge/License-ISC-lightgrey.svg) | [libsodium.org](https://libsodium.org/) |

### ⚙️ WebAssembly Runtimes

| Library  | Purpose | License | Links |
| :------ | :------ | :-----: | :---- |
| **Wasmtime**  | JIT WebAssembly runtime | ![Apache 2.0](https://img.shields.io/badge/License-Apache%202.0%20WITH%20LLVM--exception-blue.svg) | [GitHub](https://github.com/bytecodealliance/wasmtime) |
| **Wasm3**  | Interpreter WebAssembly runtime | ![MIT](https://img.shields.io/badge/License-MIT-yellow.svg) | [GitHub](https://github.com/wasm3/wasm3) |

### 🛠️ Utilities & Tooling

| Library  | Purpose | License | Links |
| :------ | :------ | :-----: | :---- |
| **spdlog**  | Fast header-only C++ logging | ![MIT](https://img.shields.io/badge/License-MIT-yellow.svg) | [GitHub](https://github.com/gabime/spdlog) |
| **zstd**  | Fast lossless compression (resource streaming) | ![BSD-3](https://img.shields.io/badge/License-BSD--3--Clause-9cf.svg) | [GitHub](https://github.com/facebook/zstd) |
| **tomlplusplus**  | Header-only TOML parser (config & manifests) | ![MIT](https://img.shields.io/badge/License-MIT-yellow.svg) | [GitHub](https://github.com/marzer/tomlplusplus) |