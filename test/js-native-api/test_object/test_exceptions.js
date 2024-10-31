'use strict';
const common = require('../../common');

// Test
const { testExceptions, hasElement } = require(`./build/Debug/test_exceptions`);

const obj = {0: "a", 2: "c"};
console.log("Object:", obj);
for (let i = 0; i <= 3; ++i) {
  console.log(`Object has element ${i}:`, hasElement(obj, i));
}

const arr = [0,,,,4];
console.log("Array:", arr);
for (let i = 0; i <= 5; ++i) {
  console.log(`Array has element ${i}:`, hasElement(arr, i));
}

function throws() {
  console.error("Throwing");
  throw new Error('foobar');
}
testExceptions(new Proxy({}, {
  get: common.mustCallAtLeast(throws, 1),
  getOwnPropertyDescriptor: common.mustCallAtLeast(throws, 1),
  defineProperty: common.mustCallAtLeast(throws, 1),
  deleteProperty: common.mustCallAtLeast(throws, 1),
  has: common.mustCallAtLeast(throws, 1),
  set: common.mustCallAtLeast(throws, 1),
  ownKeys: common.mustCallAtLeast(throws, 1),
}));
