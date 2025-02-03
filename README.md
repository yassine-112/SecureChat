<div align="center">
    <h2>Secure Chat</h2>
    <p>A decentralized peer-to-peer messaging application with end-to-end encryption, based on the TOX protocol.</p>
</div>

## Table des matières
<ul>
  <li>About the project</li>
  <li>Screenshots</li>
  <li>Overall architecture of the application</li>
  <li>Source code structure</li>
  <li>Compilation and execution</li>
  <li>Running inside Docker</li>
</ul>




## About the project
P2P SecureChat is an instant messaging application focused on privacy and security, designed to enable encrypted communications between users through a decentralized peer-to-peer network.

## Screenshots
### Front Page: (Light theme)
<img src="docs/screenshots/1.png"/>

### Front Page: (Dark theme)
<img src="docs/screenshots/2.png"/>


### Chatting with people
<img src="docs/screenshots/3.png"/>


## Overall architecture of the application
<img src="docs/system_diagram.drawio.png" />

### Languages used
- Frontend: HTML, CSS, JS avec React JS
- Backend: C++
### Libraries/Tools used
- Frontend:
  * <a href="https://ant.design/">Ant Design</a> (A React Components libraries)
  * Web APIs (<a href="https://developer.mozilla.org/en-US/docs/Web/API/WebSocket">Websocket</a>, <a href="https://developer.mozilla.org/en-US/docs/Web/API/Window/localStorage">localstorage</a>)
  * <a href="https://yarnpkg.com/">yarn</a> (package manager)
  * <a href="https://vitejs.dev/">vite</a> (React project boilerplate code generator).
- Backend:
  * <a href="https://drogon.org/">Drogon</a> (HTTP / Websocket backend)
  * <a href="https://github.com/TokTok/c-toxcore">Tox</a> (Peer 2 Peer communication layer)
  * <a href="https://github.com/atollk/copper">Copper</a> (Implements a queue used by the event loop)
  * <a href="https://github.com/google/glog">GoogleLog</a> (For logging)
  
## Source code structure

```
├── build                  (Build directory, contains files generated during compilation)
├── CMakeLists.txt         (Main configuration file for CMake)
├── CMakeUserPresets.json  (User presets configuration file for CMake)
├── conanfile.txt          (Configuration file for Conan, a C++ package manager)
├── contrib                (Configuration files for the program)
├── c-toxcore              (Source code directory for c-toxcore, a dependency)
├── Dockerfile             (Configuration file for creating a Docker image)
├── docs                   (Project documentation)
├── include                (Directory containing header files)
├── libs                   (Directory containing external libraries)
├── README.md              (README file containing information about the project)
├── src                    (Directory containing the main source code in C++)
├── uml                    (UML diagrams of the project)
├── web_front              (Web front-end of the project)
```

## Compilation and execution
Les étapes à suivre pour compiler le code sur une machine Ubuntu 22

```bash
# Installation des outils de compilation
sudo apt-get install -y cmake build-essential pipx pkg-config

# Installation des dépendances
sudo apt-get install -y libgoogle-glog-dev libsodium-dev libopus-dev libvpx-dev libconfig-dev libgtest-dev

# Installation outils de compilation du frontend
sudo apt-get install -y yarnpkg nodejs npm

# Installation du gestionnaire de paquets c++: Conan
pipx install conan

# Ajout du conan dnas le $PATH
export PATH=${PATH}:${HOME}/.local/bin/

# Initialisation du Conan
conan profile detect --force

# Téléchargement du repo
git clone --depth=1  --recurse-submodules https://github.com/m-elhamlaoui/projet-web-p2p_tox_team

# Compilation du backend
cd projet-web-p2p_tox_team
mkdir build
cd build
conan install ..  --build=missing
cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release -DRUN_YARN_BUILD=ON
cmake --build .
cp ../contrib/config.json bin/config.json

# Compilation du frontend
cd ../web_front
yarnpkg install
yarnpkg run build

# Execution du programme
cd ../build/bin
./chatapp
```

### Naviguez vers: <a href="http://127.0.0.1:8080">http://127.0.0.1:8080</a>

## Running inside Docker

Vérifiez s'il y a suffisamment d'espace disponible sur votre machine, puis exécutez 'docker build' à l'intérieur du dépôt

```bash
# Téléchargement du repo
git clone --depth=1  --recurse-submodules https://github.com/m-elhamlaoui/projet-web-p2p_tox_team
cd projet-web-p2p_tox_team

sudo docker build . -t chatapp
# Attendez toutes les étapes (ça prend presque 30 minutes)
# Lancez l'image créée avec un mapping entre le port 8080 du conteneur et le port 8080 de la machine hôte
sudo docker run -p 8080:8080 chatapp
```
### Naviguez vers: <a href="http://127.0.0.1:8080">http://127.0.0.1:8080</a>


## Vidéo de démonstration
Voire moodle
