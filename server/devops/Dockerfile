FROM node:10 as build
WORKDIR /usr/app

RUN apt-get update \
   && apt-get install -y cmake

COPY package*.json ./
RUN npm install

COPY . .

RUN npm run transpile
RUN cd core \
    && cmake . \
    && cmake --build . --target seam_carving

EXPOSE 3000
