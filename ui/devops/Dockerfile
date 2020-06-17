FROM node as build
WORKDIR /usr/app

COPY package*.json ./
RUN npm install

COPY . .

RUN npm run prod


FROM nginx as run

COPY --from=build /usr/app/config.json  /usr/share/nginx
COPY --from=build /usr/app/public/  /usr/share/nginx/html

COPY ./devops/nginx.conf /etc/nginx/conf.d/default.conf
