const { NativeProcessBase } = require("bindings")("nut-native-bridge.node");

class NativeProcess extends NativeProcessBase {}

module.exports = {
  NativeProcess
};
