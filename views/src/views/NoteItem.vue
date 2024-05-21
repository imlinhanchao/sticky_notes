<!-- eslint-disable vue/no-setup-props-destructure -->
<script setup lang="ts" name="Note">
import { ref, watch } from 'vue'
import { marked } from 'marked'
import { Note } from '@/utils';

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

watch(() => data.value.finish, () => {
  Note.update(data.value);
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
  Note.update(data.value);
}

function cancel() {
  data.value.editable = false;
  newContent.value = '';
}

function addTask() {
  Note.makeTask(data.value);
}

</script>
<template>
  <li 
    class="relative m-2 cursor-pointer note-item"
  >
  <div class="absolute w-full h-full border-2 border-current rounded"   :class="{ 
       'opacity-50': data.finish,
      'px-2 py-1': !data.editable
    }"/>
    <div @click="data.finish = !data.finish" v-if="!data.editable" class="border-inherit" :class="{ 
      'px-2 py-1': !data.editable
    }">
      <span class="absolute top-0 left-2"  :class="{ 'opacity-50': data.finish}">
        <el-checkbox @click.stop="" v-model="data.finish"> &nbsp; </el-checkbox>
      </span>
      <div :class="{ 'opacity-50': data.finish}">
        <del class="markdown-body" v-if="data.finish" v-html="marked(data.content)"></del>
        <span class="markdown-body" v-else  v-html="marked(data.content)"></span>
      </div>
    
      <span 
        class="absolute z-10 hidden text-xs toolbar right-2 bottom-[-15px]" 
        style="background-color: var(--background-color); border-top-color: var(--background-color);"
      >
      <div class="absolute w-full h-full border-2 rounded-bl rounded-br pointer-events-none toolbar" style="border-top-color: var(--background-color);"  :class="{ 'opacity-50': data.finish}"/>
      <div class="z-10 px-2 space-x-2">
        <font-awesome-icon class="p-1 rounded-sm cursor-pointer hover:bg-white/25 drag-handle" v-if="!data.finish" :icon="['far', 'calendar']" @click.stop="addTask" />
        <font-awesome-icon class="p-1 rounded-sm cursor-pointer hover:bg-white/25 drag-handle" :class="{ 'opacity-50': data.finish}" :icon="['fas', 'up-down-left-right']"/>
        <font-awesome-icon class="p-1 rounded-sm cursor-pointer hover:bg-white/25" :class="{ 'opacity-50': data.finish}" :icon="['far', 'pen-to-square']" @click.stop="edit"/>
        <font-awesome-icon class="p-1 rounded-sm cursor-pointer hover:bg-white/25" :class="{ 'opacity-50': data.finish}" :icon="['far', 'trash-can']" @click.stop="emit('remove')"/>
      </div>
      
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
        <el-button link class="absolute text-lg top-1 left-1" @click.stop="update">
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
