FROM debian:latest

WORKDIR /opt/app

RUN apt-get update && \
  apt-get install git make cmake build-essential -y

RUN git clone https://github.com/oktal/pistache.git && \
  cd pistache && \
  git submodule update --init


RUN cd pistache && \
  mkdir build && \
  cd build && \
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release .. &&\
  make && \
  sudo make install
