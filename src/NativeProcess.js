const { NativeProcessBase } = require("bindings")("nut-native-bridge.node");
const { spawn } = require("child_process");

class NativeProcess extends NativeProcessBase {
  constructor(processName) {
    super();
    this.processName = processName;
    this.process = spawn(processName);

    this.open.bind(this);
    this.close.bind(this);
    this.kill.bind(this);
    this.getMainWindow.bind(this);
  }

  open(force = false) {
    if (this.process) {
      if (!force) {
        throw new Error(`There's already an associated process: ${this.process.pid}. Close this process first, or set 'force' paramter to 'true'.`);
      } else {
        this.kill();
      }
    }
    this.process = spawn(this.processName);
  }

  close() {
    if (this.process) {
      this.process.kill();
    }
  }

  kill() {
    if (this.process !== null) {
      this.process.kill("SIGINT");
    }
  }

  getMainWindow() {
    if (this.process) {
      return super.getMainWindow(this.process.pid);
    }
    throw new Error(`No open process.`);
  }

  getWindows() {
    if (this.process) {
      return super.getWindows(this.process.pid);
    }
    throw new Error(`No open process.`);
  }
}

module.exports = {
  NativeProcess
};
