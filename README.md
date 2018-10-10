# gun-runner
A game with running and gunning

# Roadmap
[+] Done

[-] Currently in progess

[x] Pending


- Occlusion Culling [-]
  - Software Occlusion Culling [-]
    - Z-Buffer Rasterizer [-]
    - Occlusion test [-]
    - Static meshes [-]
    - Dynamic meshes [x]
  - Frustum Culling [x]
    - World grid [x]
    - Entities bounding spheres [x]
    - Culling test [x]

# Build

Create build directory.
```
cd ~/gun_runner
mkdir build
```

Run cmake on build directory.
```
cd build
cmake -G "Unix Makefiles" ../
make
```
