<script setup lang="ts" name="Home">
import { computed, onMounted, ref } from 'vue'
import draggable from 'vuedraggable';
import NoteItem from './NoteItem.vue'
import { Note, App } from '@/utils';

onMounted(() => {
  App.on('data', (data) => {
    dataList.value = data
  })
  App.init()
})

const dataList = ref<Note[]>([])

// 将 finish 排在后面
const sortData = computed(() => {
  const sortd = [...dataList.value]
  return sortd.sort((a, b) => {
    if (a.finish && !b.finish) {
      return 1
    }
    if (!a.finish && b.finish) {
      return -1
    }
    return 0
  })
})

const content = ref('')
function add() {
  if (!content.value) return
  const data = new Note(content.value)
  dataList.value.push(data)
  content.value = ''
  Note.add(data)
}

function remove(item: Note) {
  const index = dataList.value.findIndex((a) => a.id == item.id)
  if (index > -1) {
    dataList.value.splice(index, 1)
    Note.remove(item)
  }
}

function hide() {
  App.hide()
}

function clear() {
  Note.clear()
}

function onSort({ moved }: any) {
  let { oldIndex, newIndex } = moved;
  oldIndex = dataList.value.findIndex((a) => a.id == sortData.value[oldIndex].id);
  newIndex = dataList.value.findIndex((a) => a.id == sortData.value[newIndex].id);
  const data = dataList.value[oldIndex];
  dataList.value.splice(oldIndex, 1);
  dataList.value.splice(newIndex, 0, data);
  Note.updateAll(dataList.value)
}
</script>

<template>
  <el-main class="h-full">
    <draggable 
      class="my-2"
      :model-value="sortData"
      group="note"
      handle=".drag-handle"
      tag="ul"
      itemKey="id"
      animation="20"
      ghostClass="drag-ghost"
      @change="onSort"
    >
      <template #item="{ element: row }">
        <NoteItem
          :model-value="row"
          @update:model-value="(d: Note) => (dataList[dataList.findIndex((a) => d.id == a.id)] = d)"
          @remove="remove(row)"
        />
      </template>
    </draggable>
    <ul>
      <li class="m-2 rounded border-2 cursor-pointer relative border-current">
        <el-input
          type="textarea"
          :autosize="{
            minRows: 1
          }"
          class="w-full"
          @keydown.enter.ctrl.exact="add"
          v-model="content"
        />
        <el-button @click="add" link class="absolute top-1 left-1 text-lg">
          <font-awesome-icon :icon="['fas', 'plus']" />
        </el-button>
      </li>
    </ul>
  </el-main>
  <el-footer class="flex mx-2 items-center">
    <el-button class="w-full" type="primary" @click="hide">
      <font-awesome-icon :icon="['fas', 'eye-slash']" />&nbsp;隐藏显示
    </el-button>
    <el-popconfirm title="删除后将无法找回，是否确认完全删除?" @confirm="clear">
      <template #reference>
        <el-button class="w-full" type="danger">
          <font-awesome-icon :icon="['fas', 'trash']" />&nbsp;完全删除
        </el-button>
      </template>
    </el-popconfirm>
  </el-footer>
</template>

<style lang="scss" scoped>
.el-main,
.el-footer {
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
