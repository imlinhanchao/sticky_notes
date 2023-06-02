import { library } from "@fortawesome/fontawesome-svg-core";
import { FontAwesomeIcon } from "@fortawesome/vue-fontawesome";
import { fas } from "@fortawesome/free-solid-svg-icons";
import { far } from "@fortawesome/free-regular-svg-icons";
import { fab } from "@fortawesome/free-brands-svg-icons";
import ElementPlus from "element-plus";
import "element-plus/dist/index.css";
import "element-plus/theme-chalk/dark/css-vars.css";
import "./themes/style.scss";

import { createApp } from 'vue'
import { createPinia } from 'pinia'

import App from './App.vue'
import router from './router'

library.add(fas)
library.add(far)
library.add(fab)

const app = createApp(App)
app.use(createPinia())
app.component("font-awesome-icon", FontAwesomeIcon)
app.use(router);
app.use(ElementPlus).mount('#app')
