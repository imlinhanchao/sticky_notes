// @ts-ignore
export const isWebview = !!window?.chrome?.webview;

export function send(event: string, data: any=null ) {
  // @ts-ignore
  if (!isWebview) return;
  // @ts-ignore
  window.chrome.webview.postMessage(JSON.stringify({ event, data }));
}

export function listen(callback: (event: string, data: any) => void) {
  // @ts-ignore
  if (!isWebview) return;
  // @ts-ignore
  window.chrome.webview.addEventListener('message', (event) => {
    const { event: e, data } = event.data;
    callback(e, data);
  });
}

export class App {
  static listener: { [key: string]: ((data: any) => void)[] } = {};
  static init () {
    send('listen');
    listen((ev, data) => {
      if (this.listener[ev]) this.listener[ev].forEach(fn => fn(data))
    })
  }

  static hide () {
    send('hide')
  }

  static move (move: boolean) {
    send('move', move)
  }

  static close () {
    send('close')
  }

  static on(event: string, callback: (data: any) => void) {
    this.listener[event] = this.listener[event] || [];
    this.listener[event].push(callback)
  }

  static off(event: string, callback: (data: any) => void) {
    this.listener[event].splice(this.listener[event].indexOf(callback), 1)
  }
}

export class Config  {
  static bgcolor(color: string) {
    send('bgcolor', color)
  }

  static opacityable (value: boolean) {
    send('opacityable', value)
  }

  static lock (value: boolean) {
    send('lock', value)
  }

  static top (value: boolean) {
    send('top', value)
  }
}

export class Note {
  id: number = 0;
  finish: boolean = false;
  content: string = '';
  editable?: boolean;

  constructor(content = '') {
    this.id = new Date().getTime();
    this.content = content;
  }

  static add (data: Note) {
    send('add', data)
  }

  static update (data: Note) {
    send('update', data)
  }

  static remove (data: Note) {
    send('remove', data)
  }

  static clear () {
    send('clear')
  }

  static updateAll (notes: Note[]) {
    send('update_all', notes)
  }
}
