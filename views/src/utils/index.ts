// @ts-ignore
export const isWebview = !!window?.chrome?.webview;

/**
 * 发送事件到客户端
 * @param event 事件
 * @param data 数据
 */
export function send(event: string, data: any=null ) {
  // @ts-ignore
  if (!isWebview) return;
  // @ts-ignore
  window.chrome.webview.postMessage(JSON.stringify({ event, data }));
}

/**
 * 监听客户端返回数据
 * @param callback 监听函数
 * @returns 
 */
export function listen(callback: (event: string, data: any) => void) {
  // @ts-ignore
  if (!isWebview) return;
  // @ts-ignore
  window.chrome.webview.addEventListener('message', (event) => {
    const { event: e, data } = event.data;
    callback(e, data);
  });
}

/**
 * 便签操作类
 */
export class App {
  static listener: { [key: string]: ((data: any) => void)[] } = {};
  /**
   * 初始化，进入便签时调用
   */
  static init () {
    send('listen');
    listen((ev, data) => {
      if (this.listener[ev]) this.listener[ev].forEach(fn => fn(data))
    })
  }

  /**
   * 隐藏便签
   */
  static hide () {
    send('hide')
  }

  /**
   * 触发/停止移动窗口
   * @param move 是否移动
   */
  static move (move: boolean) {
    send('move', move)
  }

  /**
   * 关闭便签
   */
  static close () {
    send('close')
  }

  /**
   * 监听客户端事件
   * @param event 事件名，比如 setting：设置项目更新，lock：鼠标穿透开关，data：便签数据更新
   * @param callback 回调
   */
  static on(event: string, callback: (data: any) => void) {
    this.listener[event] = this.listener[event] || [];
    this.listener[event].push(callback)
  }

  /**
   * 关闭客户端监听
   * @param event 事件名
   * @param callback 回调
   */
  static off(event: string, callback: (data: any) => void) {
    this.listener[event].splice(this.listener[event].indexOf(callback), 1)
  }
}

/**
 * 设置操作类
 */
export class Config  {
  /**
   * 设置背景色
   * @param color 背景颜色
   */
  static bgcolor(color: string) {
    send('bgcolor', color)
  }

  /**
   * 开启关闭半透明
   * @param value 是否可透明
   */
  static opacityable (value: boolean) {
    send('opacityable', value)
  }

  /**
   * 开启/关闭鼠标穿透
   * @param value 是否开启鼠标穿透
   */
  static lock (value: boolean) {
    send('lock', value)
  }

  /**
   * 开启/关闭窗口置顶
   * @param value 是否置顶
   */
  static top (value: boolean) {
    send('top', value)
  }

  /**
   * 设置便签标题
   * @param value 便签标题
   */
  static title(value:string) {
    send('title', value)
  }
}

/**
 * 便签项定义与操作类
 */
export class Note {
  /**
   * 便签id
   */
  id: number = 0;
  /**
   * 是否完成
   */
  finish: boolean = false;
  /**
   * 便签内容
   */
  content: string = '';
  /**
   * 是否可编辑
   */
  editable?: boolean;

  constructor(content = '') {
    this.id = new Date().getTime();
    this.content = content;
  }

  /**
   * 添加便签项
   * @param data 便签内容
   */
  static add (data: Note) {
    send('add', data)
  }

  /**
   * 更新便签项
   * @param data 便签项
   */
  static update (data: Note) {
    send('update', data)
  }

  /**
   * 删除便签项
   * @param data 便签项
   */
  static remove (data: Note) {
    send('remove', data)
  }

  /**
   * 清空便签项
   */
  static clear () {
    send('clear')
  }

  /**
   * 批量更新便签项，通常用于排序
   * @param notes 便签项列表
   */
  static updateAll (notes: Note[]) {
    send('update_all', notes)
  }
}
