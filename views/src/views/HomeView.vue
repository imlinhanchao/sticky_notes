<script setup lang="ts">
import { computed, ref } from 'vue'
import { marked } from 'marked'

const data = ref([
  {
    id: 1,
    finish: false,
    content: `# 完成登录界面1
1. 草草草草
2. 顶顶顶顶
[123456](5555)
    `
  },
  {
    id: 2,
    finish: false,
    content: '完成登录界面2,完成登录界面,完成登录界面，完成登录界面，完成登录界面'
  },
  {
    id: 3,
    finish: false,
    content: '完成登录界面3'
  },
  {
    id: 4,
    finish: true,
    content: '完成登录界面4'
  }
])

// 将 finish 排在后面
const sortData = computed(() => {
  const sortd = [...data.value];
  return sortd.sort((a, b) => {
    if (a.finish && !b.finish) {
      return 1
    }
    if (!a.finish && b.finish) {
      return -1
    }
    return 0
  });
});

const content = ref('');
function add() {
  data.value.push({
    id: new Date().getTime(),
    finish: false,
    content: content.value
  });
  content.value = ''
}
</script>

<template>
  <el-main>
    <ul class="my-2">
      <li 
        @click="d.finish = !d.finish" 
        v-for="(d, i) in sortData" 
        :key="i" 
        class="m-2 rounded border-2 px-2 py-1 cursor-pointer relative"
        :class="{ 
          'border-blue-400': d.finish,
          'text-blue-400': d.finish, 
        }"
      >
        <el-checkbox class="absolute top-0 left-2" v-model="d.finish"> &nbsp; </el-checkbox>
        <del class="markdown-body" v-if="d.finish" v-html="marked(d.content)"></del>
        <span class="markdown-body" v-else  v-html="marked(d.content)"></span>
      </li>
      <li 
        class="m-2 rounded border-2 cursor-pointer relative"
      >
        <el-input 
          type="textarea" 
          :autosize="{
            minRows: 1
          }" 
          class="w-full" 
          style="
          --el-input-border-color: transparent;
          --el-input-hover-border-color: transparent;
          --el-input-focus-border-color: transparent;
          "
          @keydown.enter.ctrl.exact="add"
          v-model="content"
        />
        <el-button link class="absolute top-1 left-2">
          <font-awesome-icon :icon="['fas', 'plus']" />
        </el-button>
      </li>
    </ul>
  </el-main>
</template>

<style lang="scss" scoped>
.el-main {
  padding: 0;
}
.el-textarea {
  :deep(textarea) {
    text-indent: 1.5em;
  }
}

.markdown-body {
  > :first-child {
    text-indent: 24px;
  }
}
</style>
