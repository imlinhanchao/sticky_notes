<script setup lang="ts">
import { ref } from 'vue'

const lock = ref(false)
const wndTop = ref(false)
</script>

<template>
  <el-container direction="vertical">
    <header class="flex justify-between px-2 py-1 border-b bg-opacity-10 bg-black">
      <span>
        <font-awesome-icon :icon="['fas', 'up-down-left-right']" />
      </span>
      <span class="flex items-center space-x-2">
        <div class="w-4 h-4 inline-block border-dashed border-current border-2 rounded-full"></div>
        <font-awesome-icon v-if="lock" :icon="['fas', 'lock']" />
        <font-awesome-icon v-else :icon="['fas', 'unlock']" />
        <font-awesome-icon
          class="origin-center"
          :class="{ 'rotate-45': wndTop }"
          :icon="['fas', 'thumbtack']"
        />
        <font-awesome-icon :icon="['fas', 'xmark']" />
      </span>
    </header>
    <RouterView />
  </el-container>
</template>

<style lang="scss" scoped></style>
