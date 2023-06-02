<script setup lang="ts">
import { ref } from 'vue';

const lock = ref(false);
</script>

<template>
  <header>
    <span>
      <font-awesome-icon :icon="['fas', 'up-down-left-right']" />
    </span>
    <span>
      <div class="circle"></div>
      <font-awesome-icon v-if="lock" :icon="['fas', 'lock']" />
      <font-awesome-icon v-else :icon="['fas', 'unlock']" />
      <font-awesome-icon class="origin-center rotate-45" :icon="['fas', 'thumbtack']" />
      <font-awesome-icon :icon="['fas', 'xmark']" />
    </span>
  </header>
  <RouterView />
</template>

<style lang="scss" scoped>
</style>
