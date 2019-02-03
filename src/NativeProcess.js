const { NativeProcessBase } = require("bindings")("native-process.node");

module.exports = class NativeProcess extends NativeProcessBase {
};
