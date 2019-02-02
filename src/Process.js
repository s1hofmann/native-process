const { NativeProcess } = require("bindings")("native-process.node");

module.exports = class Process extends NativeProcess {
};
