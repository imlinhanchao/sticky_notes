<script setup lang="ts" name="Home">
import { computed, onMounted, ref } from 'vue'
import NoteItem, { Note } from './NoteItem.vue';
import { listen, send } from '@/utils/message';

onMounted(() => {
  listen((event, data) => {
    switch(event) {
      case 'data':
        dataList.value = data;
        break;
    }
  })
  send('listen');
})

const dataList = ref<Note[]>([
])

// 将 finish 排在后面
const sortData = computed(() => {
  const sortd = [...dataList.value];
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
  if (!content.value) return;
  const data = new Note(content.value);
  dataList.value.push(data);
  content.value = '';
  send('add', data);
}
</script>

<template>
  <el-main>
    <ul class="my-2">
      <NoteItem 
        v-for="(d, i) in sortData" 
        :key="i" 
        :model-value="d"
        @update:model-value="d => dataList[
          dataList.findIndex(a => d.id == a.id)
        ] = d"
      />
      <li 
        class="m-2 rounded border-2 cursor-pointer relative border-current"
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
        <el-button link class="absolute top-1 left-1 text-lg">
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
