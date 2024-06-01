FROM ubuntu:noble

RUN apt update
RUN apt-get install -y cmake build-essential pipx pkg-config
RUN apt-get install -y libgoogle-glog-dev libsodium-dev libopus-dev libvpx-dev libconfig-dev libgtest-dev
RUN apt-get install -y yarnpkg nodejs npm

RUN pipx install conan
RUN /root/.local/bin/conan profile detect --force

WORKDIR /app
COPY . /app/

WORKDIR /app/build
RUN rm -rf *
RUN /root/.local/bin/conan install ..  --build=missing
RUN  cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release -DRUN_YARN_BUILD=ON
RUN cmake --build .
COPY build/bin/config.json bin/config.json
RUN sed -i 's/127.0.0.1/0.0.0.0/g' bin/config.json


WORKDIR /app/build/bin
ENTRYPOINT /app/build/bin/chatapp
