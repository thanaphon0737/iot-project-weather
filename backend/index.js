const express = require("express");
const app = express();
const cors = require("cors");
const bodyParser = require("body-parser");
const server = require('http').Server(app);
const ip = '206.189.158.45';
const port = 7777
const io = require('socket.io')(server, {
    cors: {
        origin: `http://${ip}:8080`,
        methods: ["GET", "POST"]
    }
});
app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());
app.use(cors());
const nodeEnv = process.env.NODE_ENV || 'development'
app.get('/', (req, res) => {
    res.json({ 'Hello world': 1, nodeEnv });
})
require('dotenv').config()
const firebase = require("firebase/app");
require("firebase/database");
const firebaseConfig = {
    apiKey: process.env.apiKey,
    authDomain: process.env.authDomain,
    databaseURL: process.env.databaseURL,
    projectId: process.env.projectId,
    storageBucket: process.env.storageBucket,
    messagingSenderId: process.env.messagingSenderId,
    appId: process.env.appId,
    measurementId: process.env.measurementId
};

firebase.initializeApp(firebaseConfig);

io.on("connection", socket => {
    console.log("connected by " + socket.id);


    // Initialize Firebase

    let ref = firebase.database().ref("room")
    ref.on('value', function (snapshot) {
        snapshot.forEach(function (childSnapshot) {
            let childData = childSnapshot.val();
            
            let temp = []
            temp.push(childData.Data[0])
            temp.push(childData.Data[1])
            temp.push(childData.Data[2])
            temp.push(Number(childData.Data[3].toFixed(2)))
            let datasent = {
                Data: temp
            }
            io.sockets.emit('data', datasent)
        });
    });
})

server.listen(port, () => {
    console.log(`Server is running.. on port ${port}`)
})




