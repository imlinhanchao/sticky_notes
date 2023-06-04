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