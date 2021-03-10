<template>
  <div class="hello">
    <li class="subheader">
      temperature:
      <div class="temperature">{{ temperature }} ํC</div>
    </li>
    <li class="subheader">
      <div class="count">timer| {{ hours}}:{{mins}}:{{secs}}</div>
    </li>
    <li class="subheader">
      <div class="room">Room| {{ room }}</div>
    </li>
  </div>
</template>

<script>
import io from "socket.io-client";
// import firebase from "firebase/app"
export default {
  name: "HelloWorld",
  data() {
    return {
      socket: {},
      count: 0,
      room: "",
      temperature: 0,
      hours:0,
      mins:0,
      secs:0,
    };
  },
  created() {},
  computed: {},
  mounted() {
    let ipglobal = '206.189.158.45';
    this.socket = io(`http://${ipglobal}:7777`);
    this.socket.on("data", (data) => {
      const datashow = data.Data;
      this.count = datashow[0];
      this.room = datashow[2];
      this.temperature = datashow[3];
      let seconds = this.count*15;
      this.hours = Math.floor(seconds / 3600);
      this.mins =  Math.floor((seconds / 60) % 60);
      this.secs =  Math.floor(seconds % 60);
    });
  },
  watch: {},
  beforeDestroy() {
    this.socket.off("data");
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.subheader {
  font-family: "Proza Libre", sans-serif;
  color: #ffffff;
  font-weight: 200;
}
.temperature {
  font-size: 100px;
}
ul {
  list-style-type: none;
  padding: 0;
}
li {
  list-style-type: none;
  margin: 0 10px;
}
h1 {
  font-family: "Proza Libre", sans-serif;
  color: #ffffff;
  font-weight: 300;
}

</style>
