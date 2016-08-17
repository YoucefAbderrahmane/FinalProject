const addon = require('../../../scheduler/cpp_node_conn/build/Release/test');

// console.log(addon.add(2,3)); // 'world'

addon((msg) => {
  console.log(msg); // 'hello world'
});