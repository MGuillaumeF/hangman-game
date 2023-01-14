import { createWriteStream, statSync } from "fs";
import { get } from "https";
import { cursorTo } from "readline";

import zl from "zip-lib";

// to save full size of zip
let fullSize = 0;

const url =
  "https://www.codesynthesis.com/download/odb/2.4/odb-2.4.0-i686-windows.zip";

// erase screen
console.clear();

console.info("".padStart(50, "*"));
console.info("Download ODB depedency".padStart(40, " "));
console.info("".padStart(50, "*"));
console.info("\n\nDownload file from url", url);

const req = get(url, (res) => {
  const outputfFile = "odb-2.4.0-i686-windows.zip";
  const writeStream = createWriteStream(outputfFile);

  let percent = "0";
  res.on("data", () => {
    const newPercent = ((statSync(outputfFile).size / fullSize) * 100).toFixed(
      0
    );
    if (percent !== newPercent) {
      percent = newPercent;

      cursorTo(process.stdout, 0);
      process.stdout.write(`${percent} %`.padStart(6, " ").padStart(50, "="));
    }
  });

  res.pipe(writeStream);

  writeStream.on("finish", () => {
    writeStream.close();
    const stats = statSync(outputfFile);

    console.info(
      `\n\n => The file "${outputfFile}" is downloaded, his size on disk : ${stats.size} octets`
    );

    zl.extract(outputfFile, process.env.ODB_DOWNLOAD_DEST || "C:\\tools").then(
      function () {
        console.log("done");
      },
      function (err) {
        console.log(err);
      }
    );
  });
});

req.on("response", function (data) {
  fullSize = parseInt(data.headers["content-length"]);
});

req.on("error", function (e) {
  console.error("Error: ", e.message);
  process.exit(1);
});
