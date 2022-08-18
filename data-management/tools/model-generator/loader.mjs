import { readFileSync, existsSync } from "fs";
import path from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

export function load(templateName, parameters) {
  const templatePath = path.resolve(__dirname, "templates", `${templateName}.tlat`);
  let templateContent = '';
  if (existsSync(templatePath)) {
    templateContent = readFileSync(templatePath).toString();
    const matches = templateContent.match(/^.*\{\{\s*(\S+)\s*\}\}.*$/gm);
    let usedVars = []
    if (matches) {
    usedVars = matches.slice(1)
    console.info("variables to replace are :", ...matches.slice(1));
} else {
   console.debug("no variables found");
}
    
    const providedVars = Object.keys(parameters);
    for (const usedVar of usedVars) {
      if (providedVars.includes(usedVar)) {
        const re = new RegExp(`/^(.*)\\{\\{\\s*${usedVar}\\s*\\}\\}(.*)$/`, "gm");
        templateContent = templateContent.replace(re, `$1${parameters[usedVar]}$2`);
      } else {
        console.error("The parameter ", usedVar, " is not provided to the template");
        process.exit(1)
      }
    }

    for (const providedVar of providedVars) {
      if(!usedVars.includes(providedVar)) {
        console.warn("The parameter ", providedVar, " is not used on the template");
      }
    }
    
  } else {
    console.error("template not found :", templatePath);
    process.exit(1);
  }
  return templateContent;
}
