<script setup lang="ts" name="Note">
import { ref, watch } from 'vue'
import { marked } from 'marked'
import { send } from '@/utils/message';

const props = defineProps<{
  modelValue: Note;
}>()

const emit = defineEmits<{
  'update:modelValue': [val: Note];
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
    @click="data.finish = !data.finish" 
    class="m-2 rounded border-2 px-2 py-1 cursor-pointer relative border-current"
    :class="{ 
      'border-blue-400': data.finish,
      'text-blue-400': data.finish, 
    }"
  >
    <el-checkbox @click.stop="" class="absolute top-0 left-2" v-model="data.finish"> &nbsp; </el-checkbox>
    <del class="markdown-body" v-if="data.finish" v-html="marked(data.content)"></del>
    <span class="markdown-body" v-else  v-html="marked(data.content)"></span>
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
</style>
