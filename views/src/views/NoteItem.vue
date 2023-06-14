<script setup lang="ts" name="Note">
import { ref, watch } from 'vue'
import { marked } from 'marked'
import { send } from '@/utils/message';

const props = defineProps<{
  modelValue: Note;
}>()

const emit = defineEmits<{
  'update:modelValue': [val: Note];
  'remove': [];
}>()

const data = ref<Note>(props.modelValue || new Note())

watch(() => props.modelValue, (val) => {
  data.value = val
})

watch(() => data.value, (val) => {
  emit('update:modelValue', val)
})

watch(() => data.value.finish, (val) => {
  send('update', data.value);
})

marked.use({
  silent: true
});

const newContent = ref('');
function edit() {
  newContent.value = data.value.content;
  data.value.editable = true;
}

function update() {
  data.value.content = newContent.value;
  data.value.editable = false;
  send('update', data.value);
}

function cancel() {
  data.value.editable = false;
  newContent.value = '';
}

</script>

<script lang="ts">
export class Note {
  id: number = 0;
  finish: boolean = false;
  content: string = '';
  editable?: boolean;

  constructor(content='') {
    this.id = new Date().getTime();
    this.content = content;
  }
}
</script>

<template>
  <li 
    class="m-2 rounded border-2 cursor-pointer relative border-current note-item"
    :class="{ 
      'opacity-50': data.finish, 
      'px-2 py-1': !data.editable
    }"
    
  >
    <div @click="data.finish = !data.finish" v-if="!data.editable" class="border-inherit">
      <span class="absolute top-0 left-2">
        <el-checkbox @click.stop="" v-model="data.finish"> &nbsp; </el-checkbox>
      </span>
      <del class="markdown-body" v-if="data.finish" v-html="marked(data.content)"></del>
      <span class="markdown-body" v-else  v-html="marked(data.content)"></span>
      <span 
        class="toolbar absolute right-2 space-x-2 text-xs px-2 z-10 rounded-br rounded-bl border-2 hidden" 
        style="background-color: var(--background-color);bottom: -19px; border-top-color: var(--background-color);"
      >
      <font-awesome-icon class="drag-handle cursor-move" :icon="['fas', 'up-down-left-right']"/>
        <font-awesome-icon class="cursor-pointer" :icon="['far', 'pen-to-square']" @click.stop="edit"/>
        <font-awesome-icon class="cursor-pointer" :icon="['far', 'trash-can']" @click.stop="emit('remove')"/>
      </span>
    </div>
    <template v-else>
      <el-input 
          type="textarea" 
          :autosize="{
            minRows: 1
          }" 
          class="w-full" 
          @keydown.enter.ctrl.exact="update"
          @keydown.esc="cancel"
          v-model="newContent"
        />
        <el-button link class="absolute top-1 left-1 text-lg" @click.stop="update">
          <font-awesome-icon :icon="['fas', 'check']" />
        </el-button>
    </template>
  </li>
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

.note-item {
  border-color: var(--el-text-color-primary);
  &:hover {
    .toolbar {
      display: inline-block;
      border-color: inherit
    }
  }
}
</style>
