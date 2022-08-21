/**
 *
 * @param {*} str
 * @returns
 */
export function toCapitalize(str) {
  return `${str.charAt(0).toUpperCase()}${str.slice(1)}`;
}

/**
 *
 * @param {*} str
 * @returns
 */
export function snakeCaseToCamelCase(str) {
  const words = str.split("_");
  return `${words[0]}${words.slice(1).map(toCapitalize).join("")}`;
}

/**
 *
 * @param {*} str
 * @returns
 */
export function snakeCaseToUpperCamelCase(str) {
  const words = str.split("_");
  return words.map(toCapitalize).join("");
}
