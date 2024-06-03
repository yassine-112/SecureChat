<div align="center">
    <h2>Secure Chat</h2>
    <p>Une application de messagerie décentralisée de pair-à-pair, avec chiffrement de bout en bout, basée sur le protocole TOX.</p>
</div>

## Équipe
-  
- 
- 
- 
- 
- MOARRAF Yassine



## Table des matières
<ul>
  <li>À propos du projet</li>
    <li>Captures d'écran</li>
    <li>Architecture générale de l'application</li>
    <li>Organisation du code source</li>
    <li>Mesures de sécurité</li>
    <li>Compilation et exécution</li>
    <li>Exécution à l'intérieur de Docker</li>
    <li>Vidéo de démonstration</li>
</ul>




## À propos du projet
P2P SecureChat est une application de messagerie instantanée axée sur la confidentialité et la sécurité, conçue pour permettre des communications chiffrées entre utilisateurs via un réseau peer-to-peer décentralisé.

## Captures d'écran
### Front Page: (Light theme)
<img src="docs/screenshots/1.png"/>

### Front Page: (Dark theme)
<img src="docs/screenshots/2.png"/>


### Chatting with people
<img src="docs/screenshots/3.png"/>


## Architecture générale de l'application
<img src="docs/system_diagram.drawio.png" />

### Languages utilisées
- Frontend: HTML, CSS, JS avec React JS
- Backend: C++
### Bibliothèques/Outils utilisées
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
  
## Organisation du code source

```
├── build:                 (Répertoire de construction, contient les fichiers générés lors de la compilation)
├── CMakeLists.txt         (Fichier principal de configuration de CMake)
├── CMakeUserPresets.json (Fichier de configuration des préréglages utilisateur pour CMake)
├── conanfile.txt          (Fichier de configuration pour Conan, un gestionnaire de paquets C++)
├── contrib                (Fichiers de configucation du programme)
├── c-toxcore              (Répertoire du code source de c-toxcore, une dépendance)
├── Dockerfile             (Fichier de configuration pour la création d'une image Docker)
├── docs                   (Documentation du projet)
├── include                (Répertoire contenant les fichiers d'en-tête)
├── libs                   (Répertoire contenant des bibliothèques externes)
├── README.md              (Fichier README contenant des informations sur le projet)
├── src                    (Répertoire contenant le code source principal en c++)
├── uml                    (Diagrammes UML du projet)
├── web_front              (Front-end web du projet)
```

## Mesures de sécurité
Étant donné l'utilisation de React JS dans le front-end, plusieurs mesures de sécurité sont intégrées pour contrer les attaques frontales telles que les attaques XSS (Cross-Site Scripting). React JS offre des mécanismes intégrés pour prévenir de telles attaques, notamment en utilisant le concept de Virtual DOM pour contrôler le rendu des éléments HTML. Cela garantit que toute insertion de données est traitée de manière sécurisée et échappe aux caractères potentiellement dangereux.

De plus, le serveur écoute uniquement sur localhost, ce qui signifie qu'aucun autre utilisateur ne peut accéder à l'instance de l'application d'un autre. Cela limite considérablement les risques de violation de sécurité en restreignant l'accès aux ressources de l'application à la machine locale. En conséquence, les interactions avec l'application sont sécurisées et isolées, renforçant ainsi la protection contre les intrusions et les exploitations malveillantes.

## Compilation et exécution
Les étapes à suivre pour compiler le code sur une machine Ubuntu 22

```bash
# Installation des outils de compilation
sudo apt-get install -y cmake build-essential pipx pkg-config

# Installation des dépendances
sudo apt-get install -y libgoogle-glog-dev libsodium-dev libopus-dev libvpx-dev libconfig-dev libgtest-dev

# Installation outils de compilation du frontend
sudo apt-get install -y yarnpkg nodejs npm

# Installation du gestionaire de paquet c++: Conan
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

## Exécution à l'intérieur de Docker

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
Uploading....
