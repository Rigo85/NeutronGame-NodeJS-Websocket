var NativeMinimax

if (process.env.DEBUG) {
    NativeMinimax= require('../../build/Debug/NativeMinimax.node')
} else {
    NativeMinimax= require('../../build/Release/NativeMinimax.node')
}

module.exports = NativeMinimax
