import { existsSync, readFileSync } from "fs";
import { dirname, resolve } from "path";
import { fileURLToPath } from "url";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

/**
 *
 * @param {*} templateName
 * @param {*} parameters
 * @returns
 */
export function load(templateName, parameters) {
  const templatePath = resolve(
    __dirname,
    "../",
    "templates",
    `${templateName}.tlat`
  );
  let templateContent = "";
  if (existsSync(templatePath)) {
    templateContent = readFileSync(templatePath).toString();
    const matches = templateContent.matchAll(/\{\{\s*(\S{1,255})\s*\}\}/gm);
    let usedVars = [];
    if (matches) {
      usedVars = [].concat(
        ...Array.from(matches).map((match) => match.slice(1))
      );
      console.info("variables to replace are :", ...usedVars);
    } else {
      console.debug("no variables found");
    }

    const providedVars = Object.keys(parameters);
    for (const usedVar of usedVars) {
      if (providedVars.includes(usedVar)) {
        const re = new RegExp(`\\{\\{\\s*${usedVar}\\s*\\}\\}`, "gm");
        templateContent = templateContent.replace(re, parameters[usedVar]);
      } else {
        throw Exception(
          `The parameter "${usedVar}" is not provided to the template`
        );
      }
    }

    for (const providedVar of providedVars) {
      if (!usedVars.includes(providedVar)) {
        console.warn(
          "The parameter ",
          providedVar,
          " is not used on the template"
        );
      }
    }
  } else {
    throw Exception(`template not found at path "${templatePath}"`);
  }
  return templateContent;
}
