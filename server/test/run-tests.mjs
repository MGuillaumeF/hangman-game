import { spawn } from "child_process";
import { dirname, resolve } from "path";
import { fileURLToPath } from "url";
import waitOn from "wait-on";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

console.info("INFO", "start server");
const runServer = spawn("./HangmanGame", [], {
  cwd: resolve(
    __dirname,
    process.platform === "win32"
      ? "../../bin/server/bin/Debug"
      : "../../bin/server/bin"
  )
});

let stdOutServer = '';
runServer.stdout.on("data", (data) => {
  let rows = `${data}`.split(/\r?\n/);
  stdOutServer += '[COUT] ' + rows.join('\n[COUT] ');
  stdOutServer = stdOutServer.replace(/(\[COUT\]\s){2,}/g, '[COUT] ');
});

runServer.stderr.on("data", (data) => {
  let rows = `${data}`.split(/\r?\n/);
  stdOutServer += '[CERR] ' + rows.join('\n[CERR] ');
  stdOutServer = stdOutServer.replace(/(\[CERR\]\s){2,}/g, '[CERR] ');
});

runServer.on("close", (code) => {
  console.info("INFO", "sever close with code", code);
  console.log('******** START OUTPUT SERVER PROCESS ********');
  console.log(stdOutServer);
  console.log('********* END OUTPUT SERVER PROCESS *********');
  if (!isNaN(code) && code > 0) {
    process.exit(1);
  }
});

const opts = {
  resources: ["http://localhost:8080/"],
  delay: 1000, // initial delay in ms, default 0
  interval: 100, // poll interval in ms, default 250ms
  simultaneous: 1, // limit to 1 connection per resource at a time
  timeout: 30000, // timeout in ms, default Infinity
  tcpTimeout: 1000, // tcp timeout in ms, default 300ms
  window: 1000, // stabilization time in ms, default 750ms
  validateStatus: function (status) {
    return status >= 200 && status < 500; // default if not provided
  }
};

// Usage with async await
try {
  console.log("*** wait server state change to started ***");
  await waitOn(opts);

  console.info("INFO", "start tests");
  const runTests = spawn("ctest", ["--verbose"], {
    cwd: resolve(__dirname, "../../bin")
  });

  let stdOutTests = '';
  runTests.stdout.on("data", (data) => {
    let rows = `${data}`.split(/\r?\n/);
    stdOutTests += '[COUT] ' + rows.join('\n[COUT] ');
    stdOutTests = stdOutTests.replace(/(\[COUT\]\s){2,}/g, '[COUT] ');
  });

  runTests.stderr.on("data", (data) => {
    let rows = `${data}`.split(/\r?\n/);
    stdOutTests += '[CERR] ' + rows.join('\n[CERR] ');
    stdOutTests = stdOutTests.replace(/(\[CERR\]\s){2,}/g, '[CERR] ');
  });

  runTests.on("close", (code) => {
    console.log('******** START OUTPUT TEST PROCESS ********');
    console.log(stdOutTests);
    console.log('********* END OUTPUT TEST PROCESS *********');
    console.info("INFO", "tests close with code", code);
    runServer.kill();
    if (!isNaN(code) && code > 0) {
      process.exit(1);
    }
  });
} catch (err) {
  console.error("ERROR", err);
}
