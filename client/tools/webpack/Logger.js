/* eslint-disable @typescript-eslint/no-var-requires */
const fs = require("fs");

const availableLogLevels = ["DEBUG", "LOG", "INFO", "WARN", "ERROR"];

class Logger {
  static #instance = null;
  #level = "INFO";
  #location = "./template.log";
  #appenders = [
    (now, level, theme, ...message) => {
      // default logger to print in standard output
      console[level.toLowerCase()](now, level, theme, ...message);
    },
    (now, level, theme, ...message) => {
      // default logger to print in file
      fs.appendFileSync(
        this.#location,
        [now, level, theme, ...message, "\r\n"].join(";")
      );
    }
  ];
  /**
   * default constructor
   */
  constructor() {
    // empty
  }
  static getInstance = () => {
    if (this.#instance === null) {
      this.#instance = new Logger();
    }
    return this.#instance;
  };

  /**
   * method to print with logger
   * @param {string} level The level of trace
   * @param {string} theme The theme of trace
   * @param  {...any} message The message array
   */
  #trace = (level, theme, ...message) => {
    if (!availableLogLevels.includes(level)) {
      throw Error(
        `The level "${level}" is not supported by logger, please use one of them levels ${availableLogLevels.join(
          ", "
        )}`
      );
    }
    const now = new Date().toLocaleString();
    for (const appender of this.#appenders) {
      appender(now, level, theme, ...message);
    }
  };

  /**
   * method to change logger level
   * @param {string} level
   */
  setLevel = (level) => {
    this.#level = level;
  };

  /**
   * method to change location of log
   * @param {string} location
   */
  setLocation = (location) => {
    this.#location = location;
  };

  /**
   * method to print debug log
   * @param {string} theme The theme of log
   * @param  {...any} message The message array
   */
  debug = (theme, ...message) => {
    if (this.#level === "DEBUG") {
      this.#trace("DEBUG", theme, ...message);
    }
  };
  /**
   * method to print information log
   * @param {string} theme The theme of log
   * @param  {...any} message The message array
   */
  info = (theme, ...message) => {
    if (["DEBUG", "INFO"].includes(this.#level)) {
      this.#trace("INFO", theme, ...message);
    }
  };
  /**
   * method to print warning log
   * @param {string} theme The theme of log
   * @param  {...any} message The message array
   */
  warn = (theme, ...message) => {
    if (this.#level !== "ERROR") {
      this.#trace("WARN", theme, ...message);
    }
  };
  /**
   * method to print error log
   * @param {string} theme The theme of log
   * @param  {...any} message The message array
   */
  error = (theme, ...message) => {
    this.#trace("ERROR", theme, ...message);
  };
}

module.exports = Logger;
