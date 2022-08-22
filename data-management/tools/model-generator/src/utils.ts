/**
 *
 * @param str
 * @returns
 */
export function toCapitalize(str: string): string {
  return `${str.charAt(0).toUpperCase()}${str.slice(1)}`;
}

/**
 *
 * @param str
 * @returns
 */
export function snakeCaseToCamelCase(str: string): string {
  const words = str.split("_");
  return `${words[0]}${words.slice(1).map(toCapitalize).join("")}`;
}

/**
 *
 * @param str
 * @returns
 */
export function snakeCaseToUpperCamelCase(str: string): string {
  const words = str.split("_");
  return words.map(toCapitalize).join("");
}
