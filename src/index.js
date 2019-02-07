const { spawn } = require("child_process");
const { NativeProcess } = require("./NativeProcess");

const p = new NativeProcess(
  "/Applications/Insomnia.app/Contents/MacOS/Insomnia"
);
console.log(p.process.pid);
try {
  const window = p.getMainWindow();
  console.log(window);
  console.log(window.y);
  console.log(window.x);
  console.log(window.width);
  console.log(window.height);

  const windows = p.getWindows();
  console.log(windows);
  for (w of windows) {
    console.log(w.title);
  }
} catch (e) {
  console.error(e);
}
setTimeout(() => p.close(), 2000);
