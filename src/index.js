const { spawn } = require("child_process");
const { NativeProcess } = require("./NativeProcess");

const p = new NativeProcess(
  "/Applications/Calculator.app/Contents/MacOS/Calculator"
);
console.log(p.process.pid);
try {
  const window = p.getMainWindow();
  console.log(
    `x: ${window.x},
y: ${window.y},
width: ${window.width},
height: ${window.height}`
  );
  const windows = p.getWindows();
  console.log(windows);
} catch (e) {
  console.error(e);
}
setTimeout(() => p.close(), 2000);
