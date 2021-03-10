<template>
  <div class="main" id="app" v-bind:style="{ backgroundImage: 'url(' + image + ')' }">
    <HelloWorld msg="Welcome to Your Vue.js App"/>
  </div>
</template>

<script>
import HelloWorld from '../src/components/HelloWorld'

export default {
  name: 'App',
  components: {
    HelloWorld
  },
  data() {
    return {
      image: 'https://cdn.wallpapersafari.com/15/16/ahibRT.jpg'
    }
  },
}
</script>

<style>
html, body {
  height: 100%;
  width: 100%;
  margin: 0;
  padding: 0;
}

#app {
  display: flex;
  width: 100%;
  height: 100%;
  justify-content: center;
  align-items: center;
  background-position: center;
  background-repeat: no-repeat;
  background-size: cover;
  opacity: 1.5;
  
}





</style>
