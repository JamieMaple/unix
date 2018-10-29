async function async1() {
  console.log('a')
  await async2()
  console.log('b')
}

async function async2() {
  console.log('c')
}
console.log('d')
setTimeout(() => {
  console.log('e')
})
async1()
new Promise((res) => {
  console.log('f')
  res()
}).then(() => {
  console.log('g')
})
console.log('h')
