<script setup lang="ts">
import { onMounted, ref, watch } from 'vue'
import { isWebview, Config, App } from '@/utils'
import { useDark } from '@vueuse/core'

const lock = ref(true)

const defaultColor = '#0b0f14';
const setting = ref({
  bgcolor: defaultColor,
  topmost: true,
  opacityable: false,
  opacity: 50,
  title:'',
})

useDark({
  onChanged() {
    setTimeout(() => {
      setDark(isDark(setting.value.bgcolor))
      document.documentElement.style.setProperty('--background-color', setting.value.bgcolor);
    }, 500)
  }
})

const loading = ref(isWebview)
onMounted(() => {
  App.on('setting', (data) => {
    setting.value = data;
    loading.value = false;
  });
  App.on('lock', (data) => {
    lock.value = data;
  })
})

function isDark(bgcolor: string) {
  // 通过 hex 获取 rgb
  const R = parseInt(bgcolor.slice(1, 3), 16)
  const G = parseInt(bgcolor.slice(3, 5), 16)
  const B = parseInt(bgcolor.slice(5, 7), 16)

  return 5 * G + 2 * R + B < 8 * 128
}

function setDark(isDark: boolean) {
  document.documentElement.classList.toggle('dark', isDark)
}

function colorPicker() {
  if (!setting.value.bgcolor) {
    setting.value.bgcolor = useDark().value ? '#0b0f14' : '#ffffff'
  }
  Config.bgcolor(setting.value.bgcolor);
}

watch(
  () => setting.value.bgcolor,
  (val) => {
    document.documentElement.style.setProperty('--background-color', val);
    setDark(isDark(val))
  }
)
</script>

<template>
  <el-container v-show="!loading" @mouseup="App.move(false)" direction="vertical" class="h-full" :class="{ 'mouse-lock': lock }">
    <header
      :class="{ 'border-none': lock && !setting.title}"
      class="sticky top-0 z-10 flex justify-between px-2 py-1 bg-white border-b border-current border-dashed header bg-opacity-10"
    >
      <span class="flex items-center space-x-2 w-full hidden-lock">
        <el-tooltip placement="bottom" content="移动便签">
          <font-awesome-icon
            class="cursor-move"
            @mousedown="App.move(true)"
            :icon="['fas', 'up-down-left-right']"
          />
        </el-tooltip>
        <el-input class="no-border" v-model="setting.title" placeholder="标题" @change="(value: string)=>Config.title(value)"/>
      </span>
      <span class="hidden-unlock font-bold">{{ setting.title }}</span>
      <span class="flex items-center space-x-2 hidden-lock">
        <el-tooltip placement="bottom" content="更改主题色">
          <span><el-color-picker size="small" v-model="setting.bgcolor" @change="colorPicker" /></span>
        </el-tooltip>
        <el-tooltip placement="bottom" content="半透明便签">
          <div
            :class="{ 'border-dashed': setting.opacityable }"
            class="inline-block w-4 h-4 border-2 border-current rounded-full cursor-pointer"
            @click="Config.opacityable((setting.opacityable = !setting.opacityable))"
          ></div>
        </el-tooltip>
        <el-tooltip v-if="lock" placement="bottom" content="开启鼠标穿透">
          <font-awesome-icon
            :icon="['fas', 'lock']"
            @click="Config.lock(lock = !lock)"
            class="cursor-pointer"
          />
        </el-tooltip>
        <el-tooltip v-else placement="bottom" content="关闭鼠标穿透">
          <font-awesome-icon
            :icon="['fas', 'unlock']"
            @click="Config.lock(lock = !lock)"
            class="cursor-pointer"
          />
        </el-tooltip>
        <el-tooltip placement="bottom" content="置顶便签">
          <font-awesome-icon
            class="origin-center cursor-pointer"
            :class="{ 'rotate-45': setting.topmost }"
            :icon="['fas', 'thumbtack']"
            @click="Config.top(setting.topmost = !setting.topmost)"
          />
        </el-tooltip>
        <el-tooltip placement="bottom" content="暂时关闭便签">
          <font-awesome-icon :icon="['fas', 'xmark']" @click="App.close()" class="cursor-pointer" />
        </el-tooltip>
      </span>
    </header>
    <RouterView />
  </el-container>
  <img v-if="loading" id="preload" src="/logo.png" />
</template>

<style lang="scss" scoped>
.header, .el-container {
  background-color: var(--background-color);
}

.mouse-lock {
  &.el-container {
    background-color: transparent;
    .header {
      background-color: transparent;
    }
  }
}
</style>
