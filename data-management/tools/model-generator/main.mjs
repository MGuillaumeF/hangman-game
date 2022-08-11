
import { parseString } from "xml2js";
import { readFileSync } from "fs";
import path from "path";
import {fileURLToPath} from 'url';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const xml = readFileSync(path.resolve(__dirname, "../model.xml"));
parseString(xml, function (err, result) {
    console.info("xml parsing result", JSON.stringify(result, null, 4));
});
