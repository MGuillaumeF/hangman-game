
import { parseString } from "xml2js";
import { readFileSync } from "fs";
import path from "path";
const xml = readFileSync(path.resolve(__dirname, "../model.xml"));
parseString(xml, function (err, result) {
    console.info("xml parsing result", JSON.stringify(result, null, 4));
});
