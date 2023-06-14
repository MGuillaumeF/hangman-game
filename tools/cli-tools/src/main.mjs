import { createWriteStream, statSync } from "fs";
import { get } from "https";
import { basename } from "path";
import { cursorTo } from "readline";

import zl from "zip-lib";

function downloadFile(url, outputFilename) {
  return new Promise((resolve, reject) => {
    // to save full size of zip
    let fullSize = 0;

    // erase screen
    console.clear();

    console.info("".padStart(50, "*"));
    console.info(`Download ${basename(url)}`.padStart(40, " "));
    console.info("".padStart(50, "*"));
    console.info("\n\nDownload file from url", url);

    const req = get(url, (res) => {
      const writeStream = createWriteStream(outputFilename);

      let percent = "0";
      res.on("data", () => {
        const newPercent = (
          (statSync(outputFilename).size / fullSize) *
          100
        ).toFixed(0);
        if (percent !== newPercent) {
          percent = newPercent;

          cursorTo(process.stdout, 0);
          process.stdout.write(
            `${percent} %`.padStart(6, " ").padStart(50, "=")
          );
        }
      });

      res.pipe(writeStream);

      writeStream.on("finish", () => {
        writeStream.close();
        const stats = statSync(outputFilename);

        console.info(
          `\n\n => The file "${outputFilename}" is downloaded, his size on disk : ${stats.size} octets`
        );

        resolve();
      });
    });

    req.on("response", function (data) {
      fullSize = parseInt(data.headers["content-length"]);
    });

    req.on("error", function (e) {
      reject(e);
    });
  });
}

(async () => {
  const [order, ...args] = process.argv.slice(2);

  try {
    if (order === 'download') {
      const [url, outputFilename] = args;
      await downloadFile(url, outputFilename);
    } if (order === 'extract') {
      const [archiveName, outputPath] = args;
      await zl.extract(archiveName, outputPath);
    } 
  } catch (e) {
    console.error("Error: ", e.message);
    process.exit(1);
  }
})();
