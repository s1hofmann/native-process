const { spawn } = require("child_process");
const Process = require("./Process");

const calc = spawn("/Applications/VLC.app/Contents/MacOS/VLC");
console.log(calc.pid);
const p = new Process();
try {
  const window = p.getMainWindow(calc.pid);
  console.log(
    `x: ${window.x}, y: ${window.y}, width: ${window.width}, height: ${
      window.height
    }`
  );
} catch (e) {
  console.error(e);
}
