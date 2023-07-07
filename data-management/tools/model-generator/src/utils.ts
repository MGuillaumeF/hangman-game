/**
 * To convert lower case first char to upercase
 * @param str The string to convert
 * @returns The string with capitalise first char
 */
export function toCapitalize(str: string): string {
  return `${str.charAt(0).toUpperCase()}${str.slice(1)}`;
}

/**
 * To convert snake_case string to camelCase
 * @param str The string to convert
 * @returns The string cCamelCase
 */
export function snakeCaseToCamelCase(str: string): string {
  const words = str.split("_");
  return `${words[0]}${words.slice(1).map(toCapitalize).join("")}`;
}

/**
 * To convert snake_case string to UperCamelCase
 * @param str The string to convert
 * @returns The string UperCamelCase
 */
export function snakeCaseToUpperCamelCase(str: string): string {
  const words = str.split("_");
  return words.map(toCapitalize).join("");
}
