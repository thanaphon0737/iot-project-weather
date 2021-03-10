FROM node:14-alpine
ENV NODE_ENV=production
WORKDIR /app
COPY . /app
RUN npm install 
COPY . .
CMD ["node", "index.js"]