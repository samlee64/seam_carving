FROM node:10 as build
WORKDIR /usr/app

COPY package*.json ./
RUN npm install

COPY . .
RUN npm run transpile

FROM node:10 as run
WORKDIR /usr/app

COPY package*.json ./
RUN npm ci --only=production

COPY --from=build /usr/app/lib  /usr/app/lib
COPY --from=build /usr/app/config.docker.json /usr/app/config.json
EXPOSE 3000

CMD ["node", "lib/api/main.js"]
