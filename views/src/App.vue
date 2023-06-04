<script setup lang="ts">
import { onMounted, ref, watch } from 'vue'
import { isWebview, listen, send } from '@/utils/message'
import { useDark } from '@vueuse/core'

const lock = ref(false);

const setting = ref({
  bgcolor: useDark().value ? '#000000' : '#ffffff',
  topmost: true,
  opacityable: false,
  opacity: 50,
})

const loading = ref(isWebview);
onMounted(() => {
  listen((event, data) => {
    switch(event) {
      case 'setting':
        setting.value = data
        loading.value = false;
        break
    }
  })
})


function isDark(bgcolor: string)
{
  // 通过 hex 获取 rgb
  const R = parseInt(bgcolor.slice(1, 3), 16);
  const G = parseInt(bgcolor.slice(3, 5), 16);
  const B = parseInt(bgcolor.slice(5, 7), 16);

	return (((5 * G) + (2 * R) + B) < (8 * 128));
}

function setDark(isDark: boolean) {
  document.querySelector('html')?.classList.toggle('dark', isDark);
}

function colorPicker() {
  if (!setting.value.bgcolor) {
    setting.value.bgcolor = useDark().value ? '#0b0f14' : '#ffffff';
  }
  send('bgcolor', setting.value.bgcolor);
}

watch(() => setting.value.bgcolor, (val) => {
  document.body.style.backgroundColor = val;
  setDark(isDark(val));
})

</script>

<template>
  <el-container v-show="!loading" @mouseup="send('move', false)" direction="vertical">
    <header 
      :style="{ backgroundColor: setting.bgcolor }" 
      class="header flex justify-between px-2 py-1 border-b bg-opacity-10 bg-white sticky top-0 z-10 border-current border-dashed"
    >
      <span class="flex items-center space-x-2">
        <font-awesome-icon 
          class="cursor-move"
          @mousedown="send('move', true)"
        :icon="['fas', 'up-down-left-right']" />
      </span>
      <span class="flex items-center space-x-2">
        <el-color-picker size="small" v-model="setting.bgcolor" @change="colorPicker"/>
        <div 
          :class="{ 'border-dashed': setting.opacityable }"
          class="w-4 h-4 cursor-pointer inline-block border-current border-2 rounded-full"
          @click="send('opacityable', setting.opacityable = !setting.opacityable)" 
        >
        </div>
        <font-awesome-icon 
          v-if="lock" 
          :icon="['fas', 'lock']"
          @click="send('lock', lock = !lock)" 
          class="cursor-pointer"
        />
        <font-awesome-icon 
          v-else 
          :icon="['fas', 'unlock']" 
          @click="send('lock', lock = !lock)" 
          class="cursor-pointer"
        />
        <font-awesome-icon
          class="origin-center cursor-pointer"
          :class="{ 'rotate-45': setting.topmost }"
          :icon="['fas', 'thumbtack']"
          @click="send('top', setting.topmost = !setting.topmost)"
        />
        <font-awesome-icon 
          :icon="['fas', 'xmark']" 
          @click="send('close')"
          class="cursor-pointer"
        />
      </span>
    </header>
    <RouterView />
  </el-container>
  <img v-if="loading" id="preload" src="/logo.png" />
</template>

<style lang="scss" scoped>
.header {
  background: var(--background-color);
}
</style>
