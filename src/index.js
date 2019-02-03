const { spawn } = require("child_process");
const NativeProcess = require("./NativeProcess");

const calc = spawn("/Applications/Calculator.app/Contents/MacOS/Calculator");
console.log(calc.pid);
const p = new NativeProcess();
setTimeout(() => {
  try {
    const window = p.getMainWindow(calc.pid);
    console.log(
      `x: ${window.x},
y: ${window.y},
width: ${window.width},
height: ${window.height}`
    );
  } catch (e) {
    console.error(e);
  }
calc.kill();
}, 200);
